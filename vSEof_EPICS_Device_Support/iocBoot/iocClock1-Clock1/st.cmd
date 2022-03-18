#!../../bin/linux-x86_64/Clock1
#-
< envPaths
#-
cd "${TOP}"
#-
epicsEnvSet("DB_TOP",          "$(TOP)/db")
#-epicsEnvSet("IOCSH_LOCAL_TOP", "$(TOP)/iocsh")
epicsEnvSet("IOCSH_TOP",       "$(EPICS_BASE)/../modules/soft/iocsh")
#-
epicsEnvSet("ENGINEER",  "jeonglee")
epicsEnvSet("LOCATION",  "")
epicsEnvSet("WIKI", "")
#- 
epicsEnvSet("IOCNAME", "Clock1-Clock1")
epicsEnvSet("IOC", "iocClock1-Clock1")
#-
epicsEnvSet("P", "AAAA:")
epicsEnvSet("R", "BBBB:")

dbLoadDatabase "dbd/Clock1.dbd"
Clock1_registerRecordDeviceDriver pdbbase

dbLoadRecords("db/aiSecond.db", "user=$ENGINEER")

cd "${TOP}/iocBoot/${IOC}"

iocInit

