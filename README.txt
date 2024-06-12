COSA SERVE:
root v6.X.X
python3 

COME RUNNARE:
chmod u+rwx install.sh 
./install.sh
./CMScables


*************************************************************************************
COME INSERIRE NUOVI TEST DA PLOTTARE:
1- Il file .txt va copiato nella directory ./input/FULL_TEST_su_cavo_ps_pp1_V3/NOME_DEL_CAVO/NOME_DEL_TEST.txt
2- Il file .pdf da CEETIS va copiato in ./input/pdf_ceetis/
Il report finale è salvato in ./output/report/NOME_DEL_REPORT.pdf
Il nome del report finale è composto da NOME_DEL_TEST___DATA_DI_ESECUZIONE.pdf
*************************************************************************************
COME SCEGLIERE TEST DA PLOTTARE:
1-all'avvio si naviga nella directory ./input/FULL_TEST_su_cavo_ps_pp1_V3 per scegliere quali cavi usare 
oppure:
2- aprire CMSCables.cpp e inserire i test manualmente (guarda riga 36 di CMScables.cpp)
Se si scelgono più di un test il nome dei file output è quello del primo test + data
*************************************************************************************


*************************************************************************************
TODO:
-aggiungere nome del report da terminale (serve??)
-plot resistenza vs tempo (Da aggiungere in coda al report o separato??)
- processare in automatico l'ultimo test creato nella directory di input 
*************************************************************************************




