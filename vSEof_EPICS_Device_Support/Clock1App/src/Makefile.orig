TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
# Build an IOC support library

LIBRARY_IOC += aiSecond

DBD += aiSecond.dbd
aiSecond_SRCS += devAiSecond.c
aiSecond_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# build an ioc application

PROD_IOC = Clock1

DBD += Clock1.dbd
# Clock1.dbd will be made up from these files:
Clock1_DBD += base.dbd
#include definitions for any other support applications needed
Clock1_DBD += aiSecond.dbd

# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
Clock1_SRCS += Clock1_registerRecordDeviceDriver.cpp
Clock1_SRCS_DEFAULT += Clock1Main.cpp
Clock1_SRCS_vxWorks += -nil-

#The following adds support from base/src/vxWorks
Clock1_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

#add a definition for each support application used by this application
Clock1_LIBS += aiSecond

#NOTES:
#      1)It is not possible to build sncExample both as a component of Clock1
#        and standalone. You must choose only one.
#      2)To build sncExample SNCSEQ must be defined in <top>/configure/RELEASE

#The following builds sncExample as a component of Clock1
#Clock1_SRCS += sncExample.stt
#Clock1_LIBS += seq pv

Clock1_LIBS += $(EPICS_BASE_IOC_LIBS)

#The following builds sncExample as a standalone application
#PROD_HOST += sncExample
#sncExample_SNCFLAGS += +m
#sncExample_SRCS += sncExample.stt
#sncExample_LIBS += seq pv
#sncExample_LIBS += $(EPICS_BASE_HOST_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

