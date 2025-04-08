# Working with `iocsh`: Script Files and Commands

In previous chapters, we configured our IOCs directly within the main startup script, typically `st.cmd`. While functional for simple IOCs, this approach can become difficult to manage as configurations grow more complex or when you need to configure multiple similar devices.

This section introduces a more modular and maintainable approach used within the ALS-U EPICS Environment: encapsulating specific configuration tasks into reusable **`iocsh`** script files (often saved with a `.iocsh` extension). These snippet files contain standard `iocsh` commands but are designed to be called from the main `st.cmd` using the `iocshLoad` command, allowing for parameterization via macros.

## Motivation: Why Use Snippet Files?

Creating reusable `iocsh` script files offers several advantages:

1.  **Modularity:** Breaks down complex configurations into smaller, manageable units.
2.  **Reusability:** The same snippet can be used multiple times with different parameters (e.g., configuring several identical devices).
3.  **Clarity:** Keeps the main `st.cmd` cleaner and focused on the overall IOC structure and loading necessary components.
4.  **Maintainability:** Changes to a specific device configuration only need to be made in one snippet file, rather than potentially multiple places in a large `st.cmd`.
5.  **Standardization:** Encourages consistent configuration patterns across different IOCs.

## Example: Refactoring `st.cmd`

Let's look at the example you provided, converting a direct configuration into one using a reusable snippet.

**Original Approach** (`st.cmd`)

Here, the Asyn port configuration and database loading are done directly in the main script:

```shell
## st.cmd
...

epicsEnvSet("DB_TOP", "$(TOP)/db")

epicsEnvSet("PREFIX_MACRO", "jeonglee:")
epicsEnvSet("DEVICE_MACRO", "myoffice:")

epicsEnvSet("ASYN_PORT_NAME", "LocalTCPServer")
epicsEnvSet("TARGET_HOST",    "127.0.0.1")
epicsEnvSet("TARGET_PORT",    "9399")
...
drvAsynIPPortConfigure("$(ASYN_PORT_NAME)", "$(TARGET_HOST):$(TARGET_PORT)", 0, 0, 0)
...
asynOctetSetInputEos("$(ASYN_PORT_NAME)", 0, "\n") 
asynOctetSetOutputEos("$(ASYN_PORT_NAME)", 0, "\n") 
...
dbLoadRecords("$(DB_TOP)/training.db", "P=$(PREFIX_MACRO),R=$(DEVICE_MACRO),PORT=$(ASYN_PORT_NAME)")

...

```

**Refactored Approach** (`st2.cmd` + `training_device.iocsh`)

Now, the main script (`st2.cmd`) defines some parameters and then calls a separate snippet file (`training_device.iocsh`) to perform the actual configuration.

* `st2.cmd`

```c
...
epicsEnvSet("DB_TOP", "$(TOP)/db")
epicsEnvSet("IOCSH_LOCAL_TOP",  "$(TOP)/iocsh")

epicsEnvSet("PREFIX_MACRO", "jeonglee:")
epicsEnvSet("DEVICE_MACRO", "myoffice:")
...
epicsEnvSet("ASYN_PORT_NAME",   "LocalTCPServer")

iocshLoad("$(IOCSH_LOCAL_TOP)/training_device.iocsh", "PREFIX=$(PREFIX_MACRO),DEVICE=$(DEVICE_MACRO),DATABASE_TOP=$(DB_TOP),PORT_NAME=$(ASYN_PORT_NAME)")
...
```

* `training_device.iocsh` in `jeonglee-DemoApp/iocsh`:
```c
####################################################################################################
############ START of training-device.iocsh ########################################################
#-- PREFIX         :
#-- DEVICE         : 
#-- DATABASE_TOP   :
#-- PORT_NAME      :
#-- HOST           : [Default] 127.0.0.1
#-- PORT           : [Default] 9399 
#-- ASYNTRACE      : [Default] ##-
#--#################################################################################################
#--#
#--#

#-- Configure the Asyn IP port using the parameters defined above
#-- drvAsynIPPortConfigure("portName", "host:port", priority, noAutoConnect, noProcessEos)
#-- priority=0 (default), noAutoConnect=0 (connect immediately), noProcessEos=0 (use Asyn default EOS processing)
drvAsynIPPortConfigure("$(PORT_NAME)", "$(HOST=127.0.0.1):$(PORT=9399)", 0, 0, 0)

#-- Configure End-of-String (EOS) terminators for the Asyn port layer
#-- These define how messages are delimited when reading from/writing to the port.
#-- Ensure these match the actual device/simulator protocol! (\n = newline, \r = carriage return)
#-- NOTE: While EOS can sometimes be defined within the StreamDevice protocol file (.proto),
#-- for long-term maintenance, it is often considered best practice to define port-specific
#-- behavior like EOS explicitly in the st.cmd file using Asyn commands.
#-- Input EOS (what character(s) mark the end of a message *received from* the device)
asynOctetSetInputEos("$(ASYN_PORT_NAME)", 0, "\n")
#-- Output EOS (what character(s) should be *appended to* messages *sent to* the device)
asynOctetSetOutputEos("$(ASYN_PORT_NAME)", 0, "\n")

$(ASYNTRACE=#--)asynSetTraceMask($(ASYN_PORT_NAME), -1, ERROR|FLOW|DRIVER)

#-- --- Load Database Records ---
dbLoadRecords("$(DATABASE_TOP)/training.db", "P=$(PREFIX),R=$(DEVICE),PORT=$(PORT_NAME)")
#-- --- End Record Load ---

############ END of training-device.iocsh ##########################################################
####################################################################################################
```

## Key Concepts Explained

Let's break down how this refactoring works:

### 1. Defining Snippet Location

In `st2.cmd`, `epicsEnvSet("IOCSH_LOCAL_TOP", "$(TOP)/iocsh")` defines a standard location for these reusable script files relative to the application top directory. This makes the `iocshLoad` paths cleaner.

### 2. The `iocshLoad` Command

This command is the core mechanism for executing commands from another file. Its basic syntax is:

`iocshLoad("path/to/snippet.iocsh", "MACRO1=VALUE1,MACRO2=VALUE2,...")`

* The first argument is the path to the snippet file. Using variables like `$(IOCSH_LOCAL_TOP)` makes paths relative and portable.
* The second argument is a comma-separated string of MACRO=VALUE pairs. These macros (`$(MACRO1)`, `$(MACRO2)`, etc.) become available for substitution wherever `$(MACRO)` appears within the loaded `snippet.iocsh` file.
* The `VALUE` part can itself be a literal string, an environment variable (`$(ENV_VAR)` set via `epicsEnvSet`), or even another macro defined earlier in the calling script. In the example, `PORT_NAME=$(ASYN_PORT_NAME)` uses the value of the `ASYN_PORT_NAME` environment variable to define the `PORT_NAME` macro for the snippet.

### 3. Snippet File (`*.iocsh`) Structure

* Parameters: Uses `$(MACRO)` syntax (e.g., `$(PORT_NAME)`, `$(PREFIX)`) to receive values passed via iocshLoad.
* Defaults: `$(MACRO=DEFAULT)` syntax provides a fallback value if the macro isn't passed (e.g., `$(HOST=127.0.0.1)`).
* Documentation: Clear comments explaining the purpose and required/optional macros are crucial for reusability.
* Conditional Logic: The `$(ASYNTRACE=#--)` trick provides simple conditional execution – if `ASYNTRACE` is defined in the iocshLoad call (even if empty, like `ASYNTRACE=`), the line runs; otherwise, it becomes a comment.

* Consistency Note: Inside `training_device.iocsh`, commands related to the Asyn port now consistently use the `$(PORT_NAME)` macro, which receives its value from the `iocshLoad` call. This ensures the snippet correctly references the port name it's supposed to configure.

## Useful EPICS IOC shell commands

Even when using snippet files, the underlying commands are standard `iocsh` commands. You can still interact with the running IOC using the shell for debugging:

* `dbl`: List loaded record types, simply EPICS PV (signal) list
* `dbpr("recordName", interestLevel)`: Print record (signal) details.
* `epicsEnvShow` : Print value of an environment variable, or all variables. 
* `help`: List available commands.
* `exit`: Exit the IOC shell (usually stops the IOC).

## Exercise: Refactor Your Simulator Configuration

Now, apply this technique to the IOC configuration you created in Chapter 3 for the TCP simulator:

1.  **Create Snippet File**: Create a new file, for example, `$(TOP)/jeonglee-DemoApp/iocsh/simulator_device.iocsh`.
2.  **Move Commands**: Identify the `drvAsynIPPortConfigure`, `asynOctetSet*Eos`, and `dbLoadRecords` commands related to your simulator in your Chapter 3 `st.cmd` file and move them into `simulator_device.iocsh`.
3.  **Parameterize**: Replace hardcoded values (like PV prefix, device name, port name, host, port) in the snippet file with `$(MACRO)` variables. Add documentation comments explaining the macros. Provide defaults for host (`127.0.0.1`) and port (`9399`).
4.  **Define Location**: In your main `st.cmd`, ensure `IOCSH_LOCAL_TOP` is defined (e.g., `epicsEnvSet("IOCSH_LOCAL_TOP", "$(TOP)/iocsh")`).
5.  **Modify `st.cmd`**: Remove the original commands you moved and add an `iocshLoad` command to call your new `simulator_device.iocsh`, passing the required macros (e.g., `PREFIX=$(MY_PREFIX)`, `DEVICE=$(MY_DEVICE_NAME)`, `PORT_NAME=$(SIM_PORT_NAME)` etc., using appropriate variable names from your `st.cmd`).
6. **Build**: After creating or modifying the `simulator_device.iocsh` file in your source directory, run `make` in your application's top-level directory. This command typically copies your `.iocsh` file from its source location (e.g., j`eonglee-DemoApp/iocsh/`) to the runtime iocsh folder (e.g., `$(TOP)/iocsh`) where the IOC expects to find it via `$(IOCSH_LOCAL_TOP)` or a similar path during startup.
7.  **Test**: Run the IOC from its runtime directory (e.g., `iocBoot/iocB46-182-jeonglee-Demo`). It should start and communicate with the simulator exactly as before, but now using the cleaner, modular structure.

## Considerations

As you noted, there are some challenges when adopting this approach, especially for beginners:

1.  **Initial Complexity:** Designing truly generic, reusable `iocsh` files that handle various device or system configurations requires careful planning and understanding of potential variations.
2.  **Variable/Macro Scope:** Keeping track of variable names and macro definitions across different files (`st.cmd`, `*.iocsh` snippets, database substitution files (`.substitutions`), database template files (`.template`)) can be challenging initially. Understanding where each variable/macro is defined and used is key. (Database templates/substitutions will be covered in Section [Database Templates and Substitution](db_templates.md)).

## Conclusion

Using `iocshLoad` and creating dedicated `*.iocsh` snippet files represents a best practice within the ALS-U EPICS Environment for managing IOC configurations. While it introduces some initial complexity compared to editing a single `st.cmd`, the long-term benefits in modularity, reusability, clarity, and maintainability are substantial, especially for complex systems. Mastering this technique is a key step towards developing robust and professional EPICS applications.
