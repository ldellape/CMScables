import os
import sys
import subprocess
import mysql.connector
from flask import Flask, render_template, request, jsonify, redirect
from cableDB.src.serverConf import MYSQL_HOST, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DB
from datetime import datetime

# Script configurations
mode = 2 # continuity + isolation
code = "./CMScables"
code_stat = "./statistics"
sOutput = "/home/ldellape/cableDB/static/data/"
FailedTest = "failedTEST.log"
TestToRun = {}

# *********************************************************** #
# functions from cableDB/src/                               * #
# *********************************************************** #
def get_table_rows(table):
    conn = mysql.connector.connect(host=MYSQL_HOST, user=MYSQL_USER, password=MYSQL_PASSWORD, database=MYSQL_DB)
    cursor = conn.cursor()

    # Get the relevant column (Name for PS_Module, Serial for others)
    if table == "PS_Module":
        cursor.execute(f"SELECT Name FROM `{table}`")
    else:
        cursor.execute(f"SELECT Serial FROM `{table}`")
    serials = [serial[0] for serial in cursor.fetchall()]

    # Close cursor and connection
    cursor.close()
    conn.close()

    return serials

def fetch_table_data(table_name, column_name=None, filter_value=None):
    conn = mysql.connector.connect(host=MYSQL_HOST, user=MYSQL_USER, password=MYSQL_PASSWORD, database=MYSQL_DB)
    cursor = conn.cursor()

    # Construct SQL query
    query = f"SELECT * FROM `{table_name}`"
    if column_name and filter_value:
        query += f" WHERE {column_name} = '{filter_value}'"

    # Execute MySQL query
    cursor.execute(query)

    # Fetch column names and rows
    column_names = [i[0] for i in cursor.description]
    rows = cursor.fetchall()

    # Close cursor and connection
    cursor.close()
    conn.close()
    return column_names, rows
# *********************************************************** #
# *********************************************************** #

def check_failed_test(testPath):
    with open(FailedTest, "r") as file:
        for line in file:
            if line.strip() == testPath:
                return True
    return False 


def update_DB(cursor, serial, table_name, report_path, root_path):
    command = f"""
    UPDATE `{table_name}`
    SET 
        TestReport = CASE
            WHEN TestReport IS NULL THEN JSON_ARRAY('{report_path}')
            ELSE JSON_ARRAY_APPEND(TestReport, '$', '{report_path}')
        END,
        ROOTfile = CASE
            WHEN ROOTfile IS NULL THEN JSON_ARRAY('{root_path}')
            ELSE JSON_ARRAY_APPEND(ROOTfile, '$', '{root_path}')
        END
    WHERE Serial = '{serial}';
    """
    cursor.execute(command)

# test con raw ma non root e report                      
def check_raw_only(serial, table_name):
    column_names, rows = fetch_table_data(table_name)
    raws_to_run = []  
    
    for row in rows:
        if row[column_names.index('Serial')] == serial:
            raw_value = row[column_names.index('TestRaw')]
            report_value = row[column_names.index('TestReport')]
            rootfiles_value = row[column_names.index('ROOTfile')]

            if raw_value and not report_value and not rootfiles_value:
                raw_files = raw_value.strip("[]").replace('"', '').split(',')
                raw_files = [raw.strip() for raw in raw_files]  
                
                for raw_file in raw_files:
                    raw_file_path = os.path.join(sOutput, serial, 'RAW', raw_file)
                    if os.path.exists(raw_file_path):
                        raws_to_run.append((raw_file, table_name)) 
    return raws_to_run

def getSerials():
    tables = ['Octopus', 'PP1-PP0', 'PS-PP1']
    for table in tables:
        serials = get_table_rows(table)
        
        for serial in serials:
            raws = check_raw_only(serial, table)
            if raws:
                if serial not in TestToRun:
                    TestToRun[serial] = []
                    for raw, table_name in raws:
                        input = serial + '/RAW/' + raw
                        if not check_failed_test(input): 
                            print("**************************************")
                            print(f"New {table_name} test found: {serial}")
                            print(input)
                            print("**************************************")
                            TestToRun[serial].extend(raws)
    return TestToRun


if __name__ == "__main__":
    conn = mysql.connector.connect(host=MYSQL_HOST, user=MYSQL_USER, password=MYSQL_PASSWORD, database=MYSQL_DB)
    cursor = conn.cursor()
     
    try:
        TestToRun = getSerials()
        if TestToRun and any(TestToRun.values()):
            for serial, raws in TestToRun.items():
                serial_directory = os.path.join(sOutput, serial)
                report_directory = os.path.join(serial_directory, "REPORT")
                root_directory = os.path.join(serial_directory, "ROOT")
                
                for raw, table_name in raws: 
                    inputPath = serial + '/RAW/' + raw
                    try:
                        os.chdir("/home/ldellape/CMScables/")
                        run_CMScables = [code, "--input", inputPath, "--mode", str(mode)]
                        subprocess.run(run_CMScables, check=True)
                        raw = raw.replace('.txt', '')    
                    except subprocess.CalledProcessError as e:
                        print("*******************************************************************")
                        print(f"Error in processing {table_name}, {serial}. Skip to the next test.")
                        print("*******************************************************************")
                        with open(FailedTest, "a") as file:
                            file.write(inputPath)
                        continue
                    subprocess.run(code_stat, check=True)
                    os.chdir("..")
                    
                    update_DB(cursor, serial, table_name, f"{raw}.pdf", f"{raw}.root")
                    '''
                    if "OT_MSC" in serial and table_name == "PP1-PP0":
                        update_DB(cursor, serial, 'PP1-PP0', f"{raw}.pdf", f"{raw}.root")
                    elif "OT_LIC" in serial and table_name == "PS-PP1":
                        update_DB(cursor, serial, 'PS-PP1', f"{raw}.pdf", f"{raw}.root")
                    elif "OT_OCT" in serial and table_name=="Octopus":
                        update_DB(cursor, serial, 'Octopus', f"{raw}.pdf", f"{raw}.root")
                    else:
                        print(f" Wrong serial number: {serial} or wrong table {table_name}")
                        sys.exit()
                    '''
                    
        else:
            date = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            print(f"No new Test. {date}")
    finally:
        conn.commit()
        cursor.close()
        conn.close()
