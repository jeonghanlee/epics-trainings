#!../../bin/linux-x86_64/prng
#-
< envPaths
#-
cd "${TOP}"
#-
#-epicsEnvSet("EPICS_CA_ADDR_LIST","xxx.xxx.xxx.255")
#-epicsEnvSet("EPICS_CA_AUTO_ADDR_LIST","YES")
#-
epicsEnvSet("DB_TOP",          "$(TOP)/db")
#-epicsEnvSet("IOCSH_LOCAL_TOP", "$(TOP)/iocsh")
epicsEnvSet("IOCSH_TOP",       "$(EPICS_BASE)/../modules/soft/iocsh")
#-
epicsEnvSet("ENGINEER",  "jeonglee")
epicsEnvSet("LOCATION",  "")
epicsEnvSet("WIKI", "")
#- 
epicsEnvSet("IOCNAME", "prng-prng")
epicsEnvSet("IOC", "iocprng-prng")
#-
epicsEnvSet("P", "AAAA:")
epicsEnvSet("R", "BBBB:")

dbLoadDatabase "dbd/prng.dbd"
prng_registerRecordDeviceDriver pdbbase

#-
#- iocshLoad("$(IOCSH_TOP)/als_default.iocsh")
#- iocshLoad("$(IOCSH_TOP)/iocLog.iocsh",    "IOCNAME=$(IOCNAME), LOG_INET=$(LOG_DEST), LOG_INET_PORT=$(LOG_PORT)")
#-# Load record instances
#- iocshLoad("$(IOCSH_TOP)/iocStats.iocsh",  "IOCNAME=$(IOCNAME), DATABASE_TOP=$(DB_TOP)")
#- iocshLoad("$(IOCSH_TOP)/reccaster.iocsh", "IOCNAME=$(IOCNAME), DATABASE_TOP=$(DB_TOP)")
#- iocshLoad("$(IOCSH_TOP)/caPutLog.iocsh",  "IOCNAME=$(IOCNAME), LOG_INET=$(LOG_DEST), LOG_INET_PORT=$(LOG_PORT)")
#- iocshLoad("$(IOCSH_TOP)/autosave.iocsh", "AS_TOP=$(TOP),IOCNAME=$(IOCNAME),DATABASE_TOP=$(DB_TOP),SEQ_PERIOD=60")

#- access control list
#-asSetFilename("$(TOP)/access_securityprng-prng.acf")

cd "${TOP}/iocBoot/${IOC}"

#epicsEnvSet("PORT_1",    "$(IOCNAME)-1")
#epicsEnvSet("TC32_IP_1", "xxx.xxx.xxx.xxx")
#iocshLoad("$(IOCSH_LOCAL_TOP)/prng.iocsh", "P=$(P),R=$(R),DATABASE_TOP=$(DB_TOP),PORT=$(PORT_1),IPADDR=$(TC32_IP_1)")

iocInit

#- epicsEnvShow > /vxboot/PVenv/${IOC}.softioc
#- dbl > /vxboot/PVnames/${IOC}
#- iocshLoad("$(IOCSH_TOP)/after_iocInit.iocsh", "IOC=$(IOC),TRAGET_TOP=/vxboot")

## Start any sequence programs
#seq sncxxx,"user=jeonglee"
