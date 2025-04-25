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

### Update `st.cmd`

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

### Running and Verification

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


## Database Templates and Substitution Files

The efficient and standard EPICS method for handling configurations like the 32 channels of the TC-32 emulator involves using a `.template` file (which you've already created) in conjunction with a `.substitutions` file and the EPICS build system (`Db/Makefile`). This process allows you to define the record structure once and create many instances automatically at build time using the standard implicit rules provided by EPICS Base.

### The Substitution File (`TC-32.substitutions`)

The `.substitutions` file is where you define how the template should be instantiated. It provides the build system with a list of macro value sets to use when expanding the template file.

Examne `temperature.template` again to see the MACRO variables:

```c
record(ai,"$(P)$(R)CH$(CH)")
{
    field(DESC, "TC temperature at Channel $(CH)")
    field(DTYP, "stream")
    field( INP, "@tc32.proto get_temp($(CH)) $(PORT)")
    field( EGU, "$(EGU)")
    field(SCAN, "I/O Intr")
}
```
You can see macros `P`, `R`, `CH`, `PORT`, `EGU`. You can select which macros can be defined within the `.substitutions` file (build-time substitution) and which are left for `st.cmd` or `.iocsh` loading time.

In the `.substitutions` file below, you've chosen to define `EGU` as a global value that applies to all instances defined in the file, and you've provided specific values for the `CH` macro for each of the 32 channels. The `P`, `R`, and `PORT` macros are intentionally not given fixed values here, meaning they will be substituted later when the generated database file is loaded by the IOC.

Create the file `TC-32.substitutions` in your application's Db directory (e.g., `jeonglee-DemoApp/Db/`).

```shell
# Navigate to your application's Db directory
$ cd jeonglee-DemoApp/Db/
$ vi TC-32.substitutions
```
Add the following content:

```bash

global {EGU="Celsius"}

file "temperature.template"
{
# Define the pattern of macros expected by the template
# The order here must match the order of values provided below (only CH in this case)
pattern {CH}
# List the sets of macro values for each instance
# One line below per channel (32 lines for CH01 to CH32)
# Temperature inputs
{01}
{02}
{03}
{04}
{05}
{06}
{07}
{08}
{09}
{10}
{11}
{12}
{13}
{14}
{15}
{16}
{17}
{18}
{19}
{20}
{21}
{22}
{23}
{24}
{25}
{26}
{27}
{28}
{29}
{30}
{31}
{32}
}
```

* `global {EGU="Celsius"}`: Sets the value of the `$(EGU)` macro to "Celsius" for all records instantiated using this substitution file. This substitution happens during the build process.

* file `temperature.template`: This line specifies which template file the subsequent patterns and values should be applied to.

* `pattern {CH}`: This line defines the order of macros that will be substituted for each instance from the list below (`{...}`). In this case, only the `CH` macro is directly substituted from the list. This substitution happens during the build process.

* `{01}`, `{02}`, ..., `{32}`: Each of these lines provides the value for the `CH` macro for one instance of the template. The build tool will generate one set of records for each line here.

* The macros `P`, `R`, and `PORT` from the template are not included in the pattern list and are not in the global block. This means they will remain as `$(P)`, `$(R)`, and `$(PORT)` macros in the final `TC-32.db` file generated by the build process. Their values will be provided later when the `.db` file is loaded by `dbLoadRecords` in the `st.cmd`.


### Integrating with the Build System (`Db/Makefile`)

The EPICS build system uses the `Db/Makefile` to understand which database files need to be built and installed. For standard cases where a `.db` file has a corresponding `.template` and/or `.substitutions` file with the same prefix, simply listing the target `.db` file in the DB variable is sufficient to trigger the automatic generation and installation of that file by the EPICS build rules. 

You also need to list your source and protocol files using appropriate variables so the build system knows about them and copies them to the correct installation directory if necessary. We will revisit the subject later in more technical / maintaince aspects. 

Edit the `Db/Makefile` in your application's Db directory (e.g., `jeonglee-DemoApp/Db/Makefile)`.


```makefile
DB += TC-32.db

...
DB += $(patsubst ../%, %, $(wildcard ../*.template))
DB += $(patsubst ../%, %, $(wildcard ../*.substitutions))

...

DB += $(patsubst ../%, %, $(wildcard ../*.proto))

```

* `DB += TC-32.db`: This line lists the target database file `TC-32.db` in the DB variable. This informs the build system that `TC-32.db` is a file that should be built and installed.
* The subsequent lines using `DB += $(patsubst ../%, %, $(wildcard ...))` list source and configuration files (`.template`, `.substitutions`, `.proto`) in the DB variable. This is intended to ensure these files are also handled by the build system, likely resulting in them being copied to the final db directory.
* When you run make, the EPICS build system processes these instructions. The presence of `TC-32.db` in the DB variable, combined with the existence of `temperature.template` and `TC-32.substitutions` (which references the template), triggers the build system's underlying rules to process `temperature.template` using the substitutions in `TC-32.substitutions` and generate the `TC-32.db` file.

### Building the IOC Application

Now that you've created the `.substitution` file and updated the `Db/Makefile` to list the files, navigate to your application's top-level directory and run make.

```shell
# Navigate to your application's top directory
$ make
```

The make command, guided by the EPICS build rules and your `Db/Makefile`, will perform the necessary steps to build your application and process your database files. It will generate the `TC-32.db` file by processing `temperature.template` using `TC-32.substitutions` and place the generated file in your runtime DB directory (e.g., `${TOP}/db`). Your source files (`.template`, `.substitutions`, `.proto`) should also be copied to the db directory as per your Makefile instructions.

You can verify the generated file exists:

```shell
$ ls db/ 
# You should see TC-32.db listed along with other files
# You should also see temperature.template, TC-32.substitutions, and tc32.proto
```

You can also inspect the contents of `TC-32.db` with a text editor. You will see 32 record definitions (one for each line in your `.substitutions` file). Each record will have `CH` and `EGU` fields with the values "01" through "32" and "Celsius" respectively, but the `INP` field and the record name will still contain `$(P)`, `$(R)`, and `$(PORT)` macros, as these were not substituted by your `.substitutions` file's pattern.


### Update `st.cmd`

Now, let's demonstrate the way to configure all 32 channels using the `TC-32.db`. Navigate to your IOC instance's boot directory (e.g., `iocBoot/ioctraining-jeonglee-Demo/`), copy `st.cmd` to `st2.cmd`, and edit the `st2.cmd`


```shell
# Navigate to your iocBoot directory
$ cd iocBoot/ioctraining-jeonglee-Demo/
$ vi st2.cmd
```

```bash

#!../../bin/linux-x86_64/jeonglee-Demo

< envPaths

epicsEnvSet("DB_TOP", "$(TOP)/db")

epicsEnvSet("STREAM_PROTOCOL_PATH", "$(DB_TOP)")

# Define macros for the overall device/IOC
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

asynOctetSetInputEos("$(ASYN_PORT_NAME)", 0, "\n")
asynOctetSetOutputEos("$(ASYN_PORT_NAME)", 0, "\n")

dbLoadRecords("$(DB_TOP)/TC-32.db", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME)")

iocInit

ClockTime_Report
```

### Running and Verification

Now, let's see the efficient configuration in action. You will run the simulator, start the IOC using the updated `st2.cmd` file, and verify that the 32 temperature channel records have been loaded and are receiving data using CA client tools.

1. Start the TC-32 Emulator: Open a terminal, navigate to your simulator directory, and start the emulator on the chosen port (e.g., 9399).

```shell
# Terminal 1 (Simulator)
$ cd simulator # Adjust path as needed
$ ./tc32_emulator.bash
``` 

Leave this running.

2. Start the IOC: Open a new terminal, source your EPICS environment, navigate to your IOC instance's boot directory (e.g., jeonglee-DemoApp/iocBoot/ioctraining-jeonglee-Demo/), and run the updated `st2.cmd`.

```shell
# Terminal 2 (IOC)
$ cd jeonglee-DemoApp/iocBoot/ioctraining-jeonglee-Demo/ # Adjust path as needed
$ ./st2.cmd
```
Observe the IOC startup output. You should see messages indicating the Asyn port configuration and then the loading of records from `TC-32.db`. Use `dbl` in the IOC shell to list the loaded PVs and confirm the `MYDEMO:TC32:CHXX` records are present.

3. Verify Records and Data: Open a third terminal, source your EPICS environment, and use CA client tools like `caget` or `camonitor` to verify the records were loaded correctly and are receiving data from the simulator. The PVs should be named `MYDEMO:TC32:CHXX` (based on the macros defined in `st2.cmd` and the channel numbers fixed in the generated `.db`).

```shell
# Terminal 3 (caget, camonitor)
# Check specific channels and their fields
$ caget MYDEMO:TC32:CH10
$ caget MYDEMO:TC32:CH10.EGU # Should show "Celsius" from the global substitution
$ caget MYDEMO:TC32:CH10.DESC

# Monitor a few channels to see data streaming
$ camonitor MYDEMO:TC32:CH13
$ camonitor -t sI MYDEMO:TC32:CH13 # Show timestamps

# Check if all 32 PVs exist and monitor their values
# (This command might produce a lot of output)
$ caget MYDEMO:TC32:CH{01..32}
$ camonitor -t sI MYDEMO:TC32:CH{01..32}
```

You should see valid temperature data streaming for all 32 channels, confirming that this efficient method using templates and substitution files successfully configured all the necessary records via a single `dbLoadRecords` call on the generated `.db` file.


## Benefits of Database Templates and Substitution

As demonstrated, the template and substitution file workflow, integrated with the EPICS build system, provides significant advantages for configuring multiple similar records compared to manual methods:

* **Scalability:** Easily create hundreds or thousands of similar records by adding lines to a `.substitutions` file or even generating the `.substitutions` file programmatically.
* **Maintainability:** Update the structure of the records for all instances by editing a single `.template` file. Update common parameters (like `EGU`) for all instances by editing the global block in the `.substitutions` file.
* **Consistency:** Ensures all instances are configured identically based on the template and substitution rules, reducing configuration errors.
* **Readability:** The `st.cmd` file remains clean and short, focusing on loading the single generated database file and setting top-level macros, rather than containing repetitive record definitions or dbLoadRecords calls for every single record instance.
* **Build-Time Generation:** The heavy lifting of generating record instances happens during the build process (make), triggered by the appropriate Makefile rules, not at IOC startup (`iocInit`), potentially leading to faster startup times for very large configurations.
* **Reduced Errors:** Minimizes copy-paste errors inherent in manual methods.




## Questions for further understanding:

* Modify the `temperature.template` to add a simple alarm field (e.g., `HIGH` or `LOLO`) and rebuild the IOC.

* Explore the generated `TC-32.db` file. See how the template and substitution file were combined to create the final record definitions.

* Experiment with moving `$(P)`, `$(R)`, or `$(PORT)` from being load-time macros (provided in `st2.cmd`) to build-time macros (defined in the `.substitutions` file's pattern and list, potentially using global for common values). 

* Experiment with `dbLoadTemplate` in `st2.cmd` (WIP)



While `iocshLoad` with snippets is excellent for encapsulating sequences of iocsh commands (like configuring an Asyn port and then loading one database file or set of records), templates and substitution files processed at build time are the standard and most efficient method for generating large numbers of similar database records themselves from a common definition. Often, these techniques are used together: `iocshLoad` might configure the device connection and dbLoadRecords is then used within `st.cmd` to load a `.db` file (which was itself generated from a `.template`/`.substitutions`) associated with that connection.