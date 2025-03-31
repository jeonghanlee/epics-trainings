# Your First ALS-U EPICS IOC

This chapter provides your first hands-on experience creating an Input/Output Controller (IOC) using the standardized ALS-U EPICS environment tools. You will clone the template generator repository, use the `generate_ioc_structure.bash` script to create a basic IOC skeleton based on a name and location, explore the fundamental configuration files found in the configure and mouseApp directories (specifically `RELEASE` and `Makefile`), and learn the essential commands (`make`, `./st.cmd`) to build and execute your first simple IOC instance.

## Lesson Overview

In this lesson, you will learn how to:
* Generate and execute the IOC using the ALS-U IOC template generator
* Understand the purpose of key generated folders (`configure`, `iocBoot`, `mouseApp`).
* Understand the role of two important files (`configure/RELEASE`, `mouseApp/src/Makefile`) and their relationship.
* Understand basic EPICS IOC build and execution commands (`make`, `st.cmd`, `make clean`, `make distclean`).

## Introduction to Using the Template Generator
To build an EPICS Input Output Controller (IOC), there are plenty of ways to do so. However, within ALS-U, we aim for a consistent method for building our EPICS IOCs using a standardized template. This consistency makes it much easier to understand, maintain, and collaborate on IOCs developed by different team members.

Remembering all the necessary database definition (`.dbd`) files and dependent library links required to build even a simple IOC can be cumbersome and error-prone. To address this, we use the ALS-U EPICS template generator (found in the tools repository on the ALS GitLab server), which automates the creation of a standard IOC structure with correctly configured Makefiles.

## Prerequisites

* SSH Access: Users need SSH access to the ALS GitLab repository to clone the tools repository. Contact the controls group if you need access.
* EPICS Environment: A working ALS-U EPICS environment (as set up in Chapter 1) must be available and sourced in your terminal.
* Basic Linux Skills: Familiarity with basic commands like `cd`, `ls`, `mkdir`, `source`, `bash`.
* Text Editor: Access to a text editor (nano, vi, emacs, etc.) for examining files.

## Download the `tools` into your local machine

First, ensure you have the template generator tools. Clone the tools repository from the ALS GitLab server into a suitable location (e.g., your home directory or a development workspace). You only need to do this once.

```shell
# Example: Clone into your home directory
$ cd ~
$ git clone ssh://git@git-local.als.lbl.gov:8022/alsu/tools.git
```
This will create a tools directory containing the generator script.

## The `generate_ioc_structure.bash` Script

Located within the tools repository, this script automates the creation of the standard ALS-U IOC directory structure, significantly reducing the manual workflow potentially outlined in older development guides.

It requires two mandatory options:

* `-p` <**APPNAME**>: The Device Name or primary application name for your IOC (e.g., `mouse`).
* `-l` <**LOCATION**>: The location identifier for your IOC (e.g., `home`).

These names **must** be chosen according to the official IOC Name Naming Convention document to maintain consistency across ALS-U.

## First example: Creating, Building, and Running the `mouse` IOC

Let's create a very simple example IOC using the generator. We will use `mouse` as the **APPNAME** and `home` as **LOCATION**. Note that we use the following assumption that the **APPNAME** is the same as an IOC application name. However, in the reality, sometime, it is difficult to keep that assumption consistently. We will cover that case later. 

1. Ensure Environment is Active: Open a terminal and source the desired ALS-U EPICS environment setup script:
```shell
# Use the correct path for your setup
$ source ~/epics/1.1.1/debian-12/7.0.7/setEpicsEnv.bash
```
2. Generate the IOC Structure: Run the script from the directory containing the tools directory (or provide the full path to the script).
```shell
# Assuming 'tools' directory is in your current path's parent or provide full path
$ bash tools/generate_ioc_structure.bash -l home -p mouse
# This will create a new directory named 'mouse'
```
3. Navigate into the IOC Directory:
```shell
$ cd mouse
```
4. Build the IOC: Use the make command. This invokes the EPICS build system, which reads the Makefiles, compiles necessary components (like support for standard records), links libraries, and installs the runtime files.
```shell
mouse $ make
```
Watch for any error messages during the build.

5. Navigate to the Boot Directory: The runnable IOC instance files are placed in a specific subdirectory within iocBoot. The naming convention is typically `ioc<LOCATION>-<APPNAME>`.

```shell
mouse $ cd iocBoot/iochome-mouse
```

6. Run the IOC: Execute the startup script st.cmd. This script contains commands interpreted by the EPICS IOC shell to load configurations, databases (if any), start background processes, and initialize the IOC.
```shell
# Prompt shows you are inside the boot directory
iochome-mouse $ ./st.cmd
```
You should see EPICS startup messages, version information, and finally the IOC shell prompt (e.g., `7.0.7 >`), indicating the IOC is running. To stop it later, press `Ctrl+C` or type `exit` at the prompt.

(The tools script supports more complex scenarios, but this covers the fundamental generate-build-run cycle.)

## Exploring the Generated Folders

After running the generator and make, several directories are created. Let's examine the three most important ones for developers:`configure`, `iocBoot`, and `mouseApp`.

### A Configuration Files Folder (`configure`)

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

This file contains EPICS module dependencies. You only need to remove the `#` symbol before the required modules for your own IOC application. The `EPICS_BASE` environment variable (set when you sourced the environment) takes precedence over the value in this file during the generation of your ioc structure by a template generator. Therefore, for this workflow, you typically don't need to modify the `EPICS_BASE` line here. We will revisit this file with a practical exercise later in the guidebook.

### IOC Startup Folder (`iocBoot/iochome-mouse`)
This directory contains the files required to run a specific configured instance of your IOC application. The subdirectory name (`iochome-mouse`) combines the location (`home`) and application name (`mouse`).

* `st.cmd` (Startup Script): This is the main file executed to start the IOC. It's a script containing commands for the EPICS IOC shell. Its primary responsibilities include:

    * Loading the compiled database definitions (`dbLoadDatabase`).
    * Registering device and record support (`*registerRecordDeviceDriver`).
    * Loading specific Process Variable instances from database files (`dbLoadRecords` - none in this simple example).
    * Configuring hardware communication (none in this example).
    * Performing final initialization and starting IOC processing (`iocInit`). The template generator creates a basic `st.cmd` file that handles the essentials for starting a simple IOC. You will modify this file frequently as you add databases and hardware support.

### Application Source Folder (`mouseApp/src`)

This directory and its subdirectories (`Db`, `src`) contain the source code specific to your IOC application (`mouse` in this case).

* `mouseApp/Db/`: The conventional location for your database files (`.db`, `.proto`, `.template`, `.substitutions`). (We will add files here in later lessons).
* `mouseApp/src/`: The conventional location for your C/C++ source code files, if you write custom device support, sequence programs, or other code.
* `mouseApp/src/Makefile`: This Makefile handles compiling your custom C/C++ code (if any) and linking it with EPICS Base and required module libraries. It works closely with `configure/RELEASE`.

Examine the `mouseApp/src/Makefile`. Note the `ifneq` blocks:

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
* Automatic Dependencies: Similar to the overview step, these `ifneq ($(MODULE),)` blocks automatically include the necessary database definition files (`.dbd`) and link the required module libraries only if you have uncommented the corresponding `MODULE` variable in `configure/RELEASE`. This significantly simplifies managing build dependencies. For this first basic IOC, you don't need to modify this file because you haven't added any dependencies in `RELEASE` or any custom C code in `Common_SRCs`.

## A Few Useful Build Commands

Here are essential make commands used for building and cleaning EPICS IOCs, typically run from the top-level IOC directory (e.g., `mouse`):

* `make`: (Default target) Compiles changed source files, processes database definitions, links libraries, and installs the executable and runtime files into `bin/`, `lib/`, `dbd/`, `db/`. This is the command you use most often to build or update your IOC.
* `make clean`: Removes intermediate files generated during the build (like `.o` object files, dependency files). It generally leaves the final installed files in `bin/`, `lib/`, `db/`, `dbd/`. Use this when you want to force recompilation of source files.
* `make distclean`: Performs a much more thorough cleanup. It removes almost everything created by `make` and `make clean`, including the `bin`, `lib`, `dbd` directories and installed files in `db`. It aims to return the directory tree closer to its state immediately after generation or cloning. Use `distclean` if you suspect build problems caused by leftover files or want a completely fresh build from scratch.

You can observe the effects of these commands by examining the contents of your IOC folder (e.g., using `ls -lR` or `tree`) before and after running them.