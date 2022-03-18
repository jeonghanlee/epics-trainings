TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS BELOW HERE

# Use typed rset structure (see 3.16.1 release notes)
USR_CPPFLAGS += -DUSE_TYPED_RSET

# xxxRecord.h will be created from xxxRecord.dbd
DBDINC += xxxRecord

# Install xxxSupport.dbd into <top>/dbd
DBD += xxxSupport.dbd

# Build an IOC support library
LIBRARY_IOC += testIOCSupport

# Compile and add code to the support library
testIOCSupport_SRCS += xxxRecord.c
testIOCSupport_SRCS += devXxxSoft.c

# Link locally-provided code into the support library,
# rather than directly into the IOC application, that
# causes problems on Windows DLL builds
testIOCSupport_SRCS += dbSubExample.c
testIOCSupport_SRCS += devtestIOCVersion.c
testIOCSupport_SRCS += testIOCHello.c
testIOCSupport_SRCS += initTrace.c

testIOCSupport_LIBS += $(EPICS_BASE_IOC_LIBS)

# Auto-generate a header file containing a version string.
# Version comes from the VCS if available, else date+time.
GENVERSION = testIOCVersion.h
# Macro name
GENVERSIONMACRO = testIOCVERSION

# Build the IOC application
PROD_IOC = testIOC

# testIOC.dbd will be created and installed
DBD += testIOC.dbd

# testIOC.dbd will include these files:
testIOC_DBD += base.dbd
testIOC_DBD += xxxSupport.dbd
testIOC_DBD += dbSubExample.dbd
testIOC_DBD += devtestIOCVersion.dbd
testIOC_DBD += testIOCHello.dbd
testIOC_DBD += initTrace.dbd

# testIOC_registerRecordDeviceDriver.cpp derives from testIOC.dbd
testIOC_SRCS += testIOC_registerRecordDeviceDriver.cpp

# Build the main IOC entry point where needed
testIOC_SRCS_DEFAULT += testIOCMain.cpp
testIOC_SRCS_vxWorks += -nil-

# Link in the code from our support library
testIOC_LIBS += testIOCSupport

# To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file
ifneq ($(SNCSEQ),)
    # Build sncExample into testIOCSupport
    sncExample_SNCFLAGS += +r
    testIOC_DBD += sncExample.dbd
    # A .stt sequence program is *not* pre-processed:
    testIOCSupport_SRCS += sncExample.stt
    testIOCSupport_LIBS += seq pv
    testIOC_LIBS += seq pv

    # Build sncProgram as a standalone program
    PROD_HOST += sncProgram
    sncProgram_SNCFLAGS += +m
    # A .st sequence program *is* pre-processed:
    sncProgram_SRCS += sncProgram.st
    sncProgram_LIBS += seq pv
    sncProgram_LIBS += $(EPICS_BASE_HOST_LIBS)
endif

# Link QSRV (pvAccess Server) if available
ifdef EPICS_QSRV_MAJOR_VERSION
    testIOC_LIBS += qsrv
    testIOC_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
    testIOC_DBD += PVAServerRegister.dbd
    testIOC_DBD += qsrv.dbd
endif

# Finally link IOC to the EPICS Base libraries
testIOC_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD EXTRA GNUMAKE RULES BELOW HERE

# Explicit dependency needed for generated header file
devtestIOCVersion$(DEP): $(COMMON_DIR)/$(GENVERSION)
