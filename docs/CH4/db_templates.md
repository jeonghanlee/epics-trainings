# Database Templates and Substitution

Manually defining records for multiple similar devices or channels, like the 32 channels of our TC-32 temperature simulator, quickly becomes tedious and error-prone. Imagine writing 32 identical `ai` records, only changing a channel number or port name! This is where EPICS database templates (`.template` files) and substitution files (`.substitution` files) become indispensable tools.

This section will show you how to define a generic record structure once in a `.template` file, use a `.substitution` file to specify the variations needed for each instance (like different channel numbers or PV names), and use the EPICS build system (`Db/Makefile`) to automatically generate a complete `.db` database file containing all the necessary records. This generated - expanding - `.db` file is then loaded by the IOC startup script (`st.cmd`).

By the end of this section, you will understand the workflow for using templates and substitutions to efficiently configure large numbers of similar records.

## Lesson Overview

In this lesson, you will learn to:

* Understand the roles of `.template`, `.substitution`, and `Db/Makefile` in the process of generating database files.
* Create a StreamDevice protocol file (`.proto`) for parsing a specific data format (using the TC-32 emulator as an example).
* Create a reusable database template file (`.template`) with macros to define a generic record structure for a single device channel.
* Create a substitution file (`.substitution`) to specify the macro values needed to instantiate the template for multiple channels (e.g., all 32 channels of the TC-32).
* Modify your application's `Db/Makefile` to define rules that process the template and substitution files into a single generated `.db` file.
* Build your EPICS IOC application to execute the Makefile rules and generate the `.db` file.
* Update your IOC startup script (`st.cmd`) to load the *generated* `.db` file and configure the necessary Asyn port.
* Verify using CA client tools that multiple channel records have been loaded correctly and are receiving data from the simulator.

## Requirements Recap

This lesson assumes you have:

* Completed the previous sections, including running the TC-32 emulator.
* Access to your EPICS IOC application source directory (e.g., `jeonglee-DemoApp`) and `iocBoot` directory.
* Familiarity with basic EPICS build concepts (`make`).

## Demonstrating the Inefficient Method: Loading Templates Manually

To understand the value of database templates and substitution files processed at build time, let's first see how tedious it would be to configure 32 channels using techniques we've covered so far, specifically by repeatedly loading a template file directly in the IOC startup script.

## Generate IOC based on the Second IOC `jeonglee-Demo`

While I am working on the simulator, I changed the folder and repository name as `EPICS-IOC-demo`. Thus this is the **Case 3** example in [Expanding the Frist IOC](../CH2/addioctofirst.md)

```shell
$ bash tools/generate_ioc_structure.bash -l training -p jeonglee-Demo -f EPICS-IOC-demo

...
>> We are now creating a folder with >>> EPICS-IOC-demo <<<
>> If the folder is exist, we can go into EPICS-IOC-demo
>> in the >>> /home/jeonglee/gitsrc <<<
>> Entering into /home/jeonglee/gitsrc/EPICS-IOC-demo
>> makeBaseApp.pl -t ioc
jeonglee-Demo exists, not modified.
>>> Making IOC application with IOCNAME training-jeonglee-Demo and IOC ioctraining-jeonglee-Demo
>>>
>> makeBaseApp.pl -i -t ioc -p jeonglee-Demo
>> makeBaseApp.pl -i -t ioc -p training-jeonglee-Demo
Using target architecture linux-x86_64 (only one available)
>>>

>>> IOCNAME : training-jeonglee-Demo
>>> IOC     : ioctraining-jeonglee-Demo
>>> iocBoot IOC path /home/jeonglee/gitsrc/EPICS-IOC-demo/iocBoot/ioctraining-jeonglee-Demo

...

>> leaving from /home/jeonglee/gitsrc/EPICS-IOC-demo
>> We are in /home/jeonglee/gitsrc
```

You sucessfully create the new IOC instance based on the same IOC application.


### The StreamDevice Protocol (`tc32.proto`)
First, we need a StreamDevice protocol file that defines how the IOC should read data from the TC-32 emulator. Based on the emulator's output format (`CHXX: <TEMP>\n`), we can define a simple protocol to extract the floating-point temperature value.

Navigate to your application's `Db` directory (e.g., `jeonglee-DemoApp/Db/`).

```shell
# Navigate to your application's Db directory
$ cd jeonglee-DemoApp/Db
$ vi tc32.proto
```
Add the following content:

```bash
# Protocol for parsing TC-32 emulator output
# The emulator sends "CHXX: <TEMP>\n"
get_temp
{
    # The 'in' directive defines the input pattern.
    # \$1 is replaced by the argument passed from the INP field (the channel number).
    # %f matches a floating-point number.
    in "CH\$1: %f";
}
```
* **Explanation:** This protocol defines a single command, `get_temp`. The `in` pattern `CH\$1: %f` tells StreamDevice to look for incoming lines that start with "CH", followed by the argument passed to the protocol (which we will set to the channel number, e.g., "01"), a colon, a space, and then expects a floating-point number (`%f`). It will extract this floating-point number.

### The Database Template (`temperature.template`)

Next, ensure you have the template file `temperature.template` in your application's Db directory. This file defines the structure of a single `ai` record for one temperature channel, using macros as placeholders.

Create the file `temperature.template` in your application's `Db` directory (e.g., `jeonglee-DemoApp/Db/`).

```shell
$ vi temperature.template
```

Add the following content:

```c
# Template for a single TC-32 temperature channel record
# An Analog Input record to receive temperature data
record(ai, "$(P)$(R)CH$(CH)")
{
  field(DESC, "TC temperature at Channel $(CH)")     # Description using the channel macro
  field(DTYP, "stream")                              # Use the 'stream' device support
  field( INP, "@tc32.proto get_temp($(CH)) $(PORT)") # Reference protocol, command, args, and port
  field( EGU, "$(EGU)")                              # Units
  field(SCAN, "I/O Intr")                            # Interrupt-Driven Scan
}
```
* **Explanation:** This defines a single `ai` record structure. Its PV name uses `$(P)`, `$(R)`, and `$(CH)` macros. The SCAN field set to `I/O Intr` means the record will process whenever new data arrives for its channel via StreamDevice.


### Building the IOC Application

Before loading these files, rebuild your IOC application. Navigate to your `${TOP}` folder and run `make`. This ensures the build system is aware of the new `.proto` and `.template` files and copies them to the correct location (`${TOP}/db/`).

```shell
$ make
```

You can check that the files were copied:
```shell
$ ls db/
# You should see tc32.proto and temperature.template listed along with other files
```

### Update `st.cmd` (The Inefficient Way)

Now, let's demonstrate the inefficient way to configure all 32 channels using the `temperature.template`. Navigate to your IOC instance's boot directory (e.g., `iocBoot/ioctraining-jeonglee-Demo/`) and edit the `st.cmd` file.


```shell
# Navigate to your iocBoot directory
$ cd iocBoot/ioctraining-jeonglee-Demo/
$ vi st.cmd
```

Here's the revised `st.cmd`:

```shell
#!../../bin/linux-x86_64/jeonglee-Demo

< envPaths

epicsEnvSet("DB_TOP", "$(TOP)/db")

epicsEnvSet("STREAM_PROTOCOL_PATH", "$(DB_TOP)")

epicsEnvSet("PREFIX_MACRO", "MYDEMO:")
epicsEnvSet("DEVICE_MACRO", "TC32:")

epicsEnvSet("IOCNAME", "training-jeonglee-Demo")
epicsEnvSet("IOC", "ioctraining-jeonglee-Demo")

dbLoadDatabase "$(TOP)/dbd/jeonglee-Demo.dbd"
jeonglee_Demo_registerRecordDeviceDriver pdbbase

cd "${TOP}/iocBoot/${IOC}"

epicsEnvSet("ASYN_PORT_NAME", "LocalTCPServer")
epicsEnvSet("TARGET_HOST",    "127.0.0.1")
epicsEnvSet("TARGET_PORT",    "9399")
drvAsynIPPortConfigure("$(ASYN_PORT_NAME)", "$(TARGET_HOST):$(TARGET_PORT)", 0, 0, 0)

asynOctetSetInputEos("$(ASYN_PORT_NAME)",  0, "\n")
asynOctetSetOutputEos("$(ASYN_PORT_NAME)", 0, "\n")

# --- START: Manually loading the database template file for each channel ---
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=01")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=02")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=03")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=04")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=05")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=06")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=07")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=08")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=09")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=10")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=11")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=12")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=13")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=14")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=15")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=16")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=17")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=18")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=19")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=20")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=21")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=22")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=23")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=24")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=25")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=26")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=27")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=28")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=29")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=30")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=31")
dbLoadRecords("$(DB_TOP)/temperature.template", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME),EGU=Celsius,CH=32")
# --- END: Manually loading the database template file ---

iocInit

ClockTime_Report
```

### Running and Verification (Inefficient Method)

Now, run the simulator and the IOC to see this configuration in action.

1. **Start the TC-32 Emulator:** Open a terminal, navigate to your simulator directory, and start the emulator on the default port, 9399.

```shell
# Terminal 1 (Simulator)
$ cd simulator  # Adjust path as needed
$ ./tc32_emulator.bash
```
Leave this running.

2. **Start the IOC:** Open a new terminal, source your EPICS environment, navigate to your `iocBoot` directory, and run the updated `st.cmd`.

```shell
# Terminal 2 (IOC)
$ cd iocBoot/ioctraining-jeonglee-Demo/
$ ./st.cmd
```
Observe the IOC startup output. You should see messages indicating the Asyn port configuration and the loading of 32 records, one after another, as each `dbLoadRecords` command is executed. You can check the available signals via `dbl`.


3. **Monitoring PVs:** Open a new terminal, source your EPICS environment, and use CA client tools like `caget` or `camonitor` to verify the records were loaded and are receiving data. The PVs should be named `MYDEMO:TC32:CHXX` (based on the macros used in `st.cmd`).

```shell
# Terminal 3 (caget, camonitor)

# Check specific channels
$ caget MYDEMO:TC32:CH10
$ caget MYDEMO:TC32:CH10.EGU
$ caget MYDEMO:TC32:CH10.DESC

# Monitor a few channels to see data streaming
$ camonitor MYDEMO:TC32:CH13
$ camonitor -t sI MYDEMO:TC32:CH13 # Show timestamps by CA server incremental timestamps

# Check if all 32 PVs exist
$ caget MYDEMO:TC32:CH{01..32}
$ camonitor -t sI MYDEMO:TC32:CH{01..32}
```

You should see valid temperature data streaming for the monitored PVs, confirming that this method works for loading the records and interacting with the simulator.

### The Problem: Manual Configuration is Tedious

While the above method works, it's clear that manually adding and maintaining 32 separate `dbLoadRecords` lines in `st.cmd` for a single device like the TC-32 is highly inefficient.

This approach is clearly unsustainable for real-world systems with potentially hundreds or thousands of similar channels or devices. Any change to the record definition (e.g., adding an alarm field or changing units) would require finding and editing the parameter string in 32 separate `dbLoadRecords` lines. A simple typo in one line could lead to hard-to-debug errors.

Fortunately, EPICS provides a robust solution to this exact problem using database templates and substitution files processed primarily at build time, allowing us to define the structure once and instantiate it many times automatically. The resulting single database file is then loaded efficiently at IOC loading time with a single `dbLoadRecords` command.


## The Solution: Database Templates and Substitution Files

The efficient and standard EPICS method for handling configurations like the 32 channels of the TC-32 emulator involves using a `.template` file (which you've already created) in conjunction with a `.substitution` file and the EPICS build system (`Db/Makefile`). This process allows you to define the record structure once and create many instances automatically at build time.