TOP = ../..
include $(TOP)/configure/CONFIG

# IOCSH += Clock1.iocsh
IOCSH += $(patsubst ../%, %, $(wildcard ../*.iocsh))

include $(TOP)/configure/RULES

