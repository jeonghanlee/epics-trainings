# Your First ALS-U EPICS IOC

## Lesson Overview

In this lesson, you will learn how to:
* Generate and execute the IOC using the ALS-U IOC template generator
* Understand two important files in your IOC folders and their relationships
* Understand a few EPICS IOC building commands

To build an EPICS Input Output Controller (IOC), there are plenty of ways to do so. However, we would like to maintain a consistent method for building our EPICS IOCs. This way, we can easily understand how other IOCs, which may be developed by different team members, work and are organized.

From my own experience, it is cumbersome to remember all the `dbd` files and relevant dependent library files required to build an IOC. Therefore, we have the ALS-U EPICS template generator, which is referred to as `tools` in our GitLab project.

## Prerequisites

Users may need to have SSH access to the ALS GitLab repository to clone the following repositories.

## Download the `tools` into your local machine

The following `clone` command one can download the latest version of `tools`

```shell
$ git clone ssh://git@git-local.als.lbl.gov:8022/alsu/tools.git
```

## `generate_ioc_structure.bash`

This script is developed to reduce the manual workflow in EPICS IOC Development Guide (AL-1451-7926). There are two mandatory options, such as **Device Name (APPNAME)** and **LOCATION**. Two options should be defined according to the IOC Name Naming Convention documents [1].

## First example

Now we would like to create an IOC with `mouse` as the **APPNAME** and `home` as **LOCATION**. Note that we use the following assumption that the `APPNAME` is the same as an IOC application name. However, in the reality, sometime, it is difficult to keep that assumption consistently. We will cover that case later. 

```shell
$ source ~/epics/1.1.1/debian-12/7.0.7/setEpicsEnv.bash
$ bash tools/generate_ioc_structure.bash -l home -p mouse
$ cd mouse
mouse $ make
mouse $ cd iocBoo/iochome-mouse
iochome-mouse $ ./st.cmd
```

We have more scenarios which `tools` covers, however we can focus on a simple example here.

## Folders

At this point, there are three important folders: `configure`, `iocBoot`, and `mouseApp`. Let's explore them one by one.

### A Configuration Files Folder `configure`


This folder contains several predefined configuration files that work well within the standard EPICS building system.
In `configure`, `RELEASE` and `CONFIG_SITE` are the files most often opened and updated. Please open `configure/CONFIG_SITE` first to review its contents.
With the ALS-U EPICS environment and how to deploy the EPICS IOC within the ALS-U production environment, we rarely update the `CONFIG_SITE`.

In this folder, you will primarily care about the `RELEASE` file, as the template-generated configuration files will likely not need modification.

Please open the `RELEASE` file. You will see the following templated generated content:

```makefile
...
# Variables and paths to dependent modules:
MODULES = $(EPICS_BASE)/../modules
#MYMODULE = $(MODULES)/my-module

# If using the sequencer, point SNCSEQ at its top directory:

#ALS_IOCSH = $(MODULES)/iocsh

#ASYN = $(MODULES)/asyn
#SNCSEQ = $(MODULES)/seq
#MODBUS = $(MODULES)/modbus
#SNMP = $(MODULES)/snmp
#STD = $(MODULES)/std
#CALC = $(MODULES)/calc
#AUTOSAVE = $(MODULES)/autosave
#RECCASTER = $(MODULES)/recsync
#### STREAM = $(MODULES)/stream
# ALS-U use "StreamDevice" as a directory name
# STREAM = $(MODULES)/StreamDevice
#RETOOLS = $(MODULES)/retools
#CAPUTLOG = $(MODULES)/caPutLog
#### devIocStats : EPICS community iocStats
#### STREAM = $(MODULES)/stream
# ALS-U use "StreamDevice" as a directory name
# STREAM = $(MODULES)/StreamDevice
#RETOOLS = $(MODULES)/retools
#CAPUTLOG = $(MODULES)/caPutLog
#### devIocStats : EPICS community iocStats
#devIocStats = $(MODULES)/iocStats
#### IOCSTATS : ALS specific iocStats
#IOCSTATS = $(MODULES)/iocStatsALS
#MEASCOMP = $(MODULES)/measComp
#SSCAN=$(MODULES)/sscan
#BUSY=$(MODULES)/busy
#SCALER=$(MODULES)/scaler
#MCA=$(MODULES)/mca
#ETHER_IP=$(MODULES)/ether_ip
#
### ALS-U Site Specific Modules
### VACUUM Modules
#RGAMV2=$(MODULES)/rgamv2
#UNIDRV=$(MODULES)/unidrv
#QPC=$(MODULES)/qpc
### Instrumentation
#EVENTGENERATORSUP=$(MODULES)/eventGeneratorSup
#BPMSUP=$(MODULES)/bpmSup
### FEED for LLRF
#FEED=$(MODULES)/feed
### PSC Modules
#PSCDRV=$(MODULES)/pscdrv
### MOTION Modules
#PMAC=$(MODULES)/pmac
#

### ALS-U Default Module
PVXS=$(MODULES)/pvxs

# EPICS_BASE should appear last so earlier modules can override stuff:
EPICS_BASE = /home/jeonglee/epics/1.1.1/debian-12/7.0.7/base

...
```

This file contains EPICS module dependencies. You only need to remove the `#` symbol before the required modules for your own IOC application. Your `EPICS_BASE` was defined when you generated this IOC using the tools. Thus, you can ignore the `EPICS_BASE` defined in the RELEASE file.

We will revisit this file with a practical exercise later in the guidebook.


### Application Source Folder `mouseApp/src`

Even if you have no source code in the `src` folder, you should modify the `Makefile` according to the IOC module dependencies.


```makefile
TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=============================
# Build the IOC application

PROD_IOC = mouse
# mouse.dbd will be created and installed
DBD += mouse.dbd

Common_DBDs += base.dbd
Common_DBDs += system.dbd

Common_SRCs +=

ifneq ($(ASYN),)
Common_DBDs += asyn.dbd
Common_DBDs += drvAsynIPPort.dbd
Common_DBDs += drvAsynSerialPort.dbd
Common_LIBs += asyn
endif

ifneq ($(MODBUS),)
Common_DBDs += modbusSupport.dbd
Common_LIBs += modbus
endif

ifneq ($(SNMP),)
Common_DBDs += devSnmp.dbd
Common_LIBs += devSnmp
SYS_PROD_LIBS += netsnmp
endif

...
```
However, in most cases, you don't need to understand this in great detail, as the `Makefile` contains reasonable definitions for each module name, synced with several module name variables in `RELEASE`. It defines their dependent `dbd` files, module library files, and sometimes system libraries as well.

## A few useful commands

Here are some useful commands for the EPICS IOC build make rules.

```shell
$ make
$ make clean
$ make distclean
```

You can see the difference in your IOC folder among `make`, `make clean`, and `make distclean` by using `tree` or `ls` commands.
