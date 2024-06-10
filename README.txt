COSA SERVE (in lab D3 già installati):
root v6.X.X
pdftk server 

COME RUNNARE:
chmod u+rwx install.sh 
./install.sh
./CMScables


*************************************************************************************
COME INSERIRE NUOVI TEST DA PLOTTARE:
1- Il file .txt va copiato nella directory ./input/FULL_TEST_su_cavo_ps_pp1_V3/NOME_DEL_CAVO/NOME_DEL_TEST.txt
2- Il file .pdf da CEETIS va copiato in ./input/pdf_ceetis/
Il report finale è salvato in ./output/report/TIPO_DI_TEST/NOME_DEL_REPORT.pdf
Il nome del report finale è composto da NOME_DEL_TEST___DATA_DI_ESECUZIONE.pdf
Per produrre il report finale, cioè pdf con i plot + pdf da macchina per test togliere commento
ad ultima riga di "./include/plotting.h" (solo se si ha pdftk installato e nel PATH)
*************************************************************************************
COME SCEGLIERE TEST DA PLOTTARE:
1-all'avvio si naviga nella directory ./input/FULL_TEST_su_cavo_ps_pp1_V3 per scegliere quali cavi usare 
oppure:
2- aprire CMSCables.cpp e inserire i test manualmente (guarda riga 36 di main.cpp)
Se si scelgono più di un test il nome dei file output è quello del primo test + "with_#cavidaplottare"
*************************************************************************************


*************************************************************************************
TODO:
-aggiungere nome del report da terminale (??)
-plot resistenza vs tempo
*************************************************************************************




