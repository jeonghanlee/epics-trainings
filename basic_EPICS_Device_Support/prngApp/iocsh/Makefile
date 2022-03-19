TOP = ../..
include $(TOP)/configure/CONFIG

# IOCSH += prng.iocsh
IOCSH += $(patsubst ../%, %, $(wildcard ../*.iocsh))

include $(TOP)/configure/RULES

