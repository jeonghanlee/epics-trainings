# Managing Multiple Devices in `st.cmd`

Now that we have learned how to create reusable configuration snippets using `.iocsh files` in [Working with `iocsh`: Script Files and Commands](iocsh_basics.md) and how to run multiple instances of our extended TCP simulator on different ports [Update the TCP Simulator](iocsimulator2.md), we can combine these concepts.

This section focuses on configuring a single IOC to communicate with multiple identical devices simultaneously. We will achieve this by leveraging the `.iocsh` snippet file developed earlier, calling it multiple times within the main `st.cmd` file using `iocshLoad`, passing different parameters (macros) for each device instance. We will focus here on managing the communication setup (like Asyn ports) efficiently using this technique.

## Recap: The Reusable `.iocsh` Snippet

Recall the `.iocsh` script file we worked with (e.g., `simulator_device.iocsh` or `training_device.iocsh` from the Section 1 exercise). It encapsulates the steps needed to configure one device connection, including setting up the Asyn port and loading associated database records. It accepts parameters via macros passed through `iocshLoad`, such as `PREFIX`, `DEVICE`, `PORT_NAME`, `PORT`, `HOST`, `DATABASE_TOP`. Assuming this snippet loads records like `Cmd` (`stringout`) and `Cmd-RB` (`stringin`) from a database file (like `training.db`), these PVs will be created relative to the `PREFIX` and `DEVICE` macros.


## Configuring Multiple Instances in `st.cmd`

To manage multiple devices, we simply call `iocshLoad` multiple times in our main `st.cmd`, providing a unique set of macro values for each call.

* **Define Unique Parameters**: For each device instance, define a unique set of parameters. Using `epicsEnvSet` can keep this organized. We need unique PV prefixes/device parts and unique Asyn port names and target TCP ports for each simulator.
* **Call `iocshLoad` Repeatedly**: Execute the iocshLoad command for your snippet file once per device instance.

## 
Example st.cmd Snippet (configuring 3 simulator instances):


```bash

# Ensure IOCSH_LOCAL_TOP and DB_TOP are defined (e.g., via epicsEnvSet)
# epicsEnvSet("IOCSH_LOCAL_TOP", "$(TOP)/iocsh")
# epicsEnvSet("DB_TOP", "$(TOP)/db")

# --- Configuration for Simulator Instance 1 ---
epicsEnvSet("PREFIX1",         "MYDEMO:")      # Main prefix
epicsEnvSet("DEVICE1",         "SIM1:")        # Unique device part
epicsEnvSet("ASYN_PORT_NAME1", "TCP1")         # Unique Asyn Port Name
epicsEnvSet("TARGET_PORT1",    "9399")         # TCP Port for simulator 1
# Load snippet for Instance 1
iocshLoad("$(IOCSH_LOCAL_TOP)/training_device.iocsh", "PREFIX=$(PREFIX1),DEVICE=$(DEVICE1),DATABASE_TOP=$(DB_TOP),PORT_NAME=$(ASYN_PORT_NAME1), PORT=$(TARGET_PORT1)")

# --- Configuration for Simulator Instance 2 ---
epicsEnvSet("PREFIX2",         "MYDEMO:")      # Main prefix
epicsEnvSet("DEVICE2",         "SIM2:")        # Unique device part
epicsEnvSet("ASYN_PORT_NAME2", "TCP2")         # Unique Asyn Port Name
epicsEnvSet("TARGET_PORT2",    "9400")         # TCP Port for simulator 2
# Load snippet for Instance 2
iocshLoad("$(IOCSH_LOCAL_TOP)/training_device.iocsh", "PREFIX=$(PREFIX2),DEVICE=$(DEVICE2),DATABASE_TOP=$(DB_TOP),PORT_NAME=$(ASYN_PORT_NAME2), PORT=$(TARGET_PORT2)")

# --- Configuration for Simulator Instance 3 ---
epicsEnvSet("PREFIX3",         "MYDEMO:")      # Main prefix
epicsEnvSet("DEVICE3",         "SIM3:")        # Unique device part
epicsEnvSet("ASYN_PORT_NAME3", "TCP3")         # Unique Asyn Port Name
epicsEnvSet("TARGET_PORT3",    "9401")         # TCP Port for simulator 3
# Load snippet for Instance 3
iocshLoad("$(IOCSH_LOCAL_TOP)/training_device.iocsh", "PREFIX=$(PREFIX3),DEVICE=$(DEVICE3),DATABASE_TOP=$(DB_TOP),PORT_NAME=$(ASYN_PORT_NAME3), PORT=$(TARGET_PORT3), ASYNTRACE=")
```

## Next Steps 

After setting up the `st.cmd` file as shown in the example above, the immediate next steps involve completing the script, running all components, and verifying communication for each instance:

1. Prepare Environment: You will need multiple terminal windows:
    * One (or more) for running the simulator instances.
    * One for running the EPICS IOC.
    * One for running Channel Access (CA) client commands (`caput`, `caget`).

2. Finalize `st.cmd`: Complete the startup script by adding the essential iocInit command after all `iocshLoad` calls .

3. Run the System:

    * Start Simulators (Terminal 1+): In the simulator terminal(s), start the three TCP simulator instances (`tcpserver.bash`), ensuring they listen on ports 9399, 9400, and 9401 respectively (refer to Section 2 for methods). Leave them running.

    * Start IOC (Terminal 2): In the IOC terminal, navigate to the IOC's boot directory (e.g., `iocBoot/iocYourIOCName`), ensure your EPICS environment is sourced, and execute the completed `st.cmd` file.

4. Verify Operation (Terminals 2 & 3):
    * Check IOC Logs (Terminal 2): Observe the IOC console output for messages indicating successful Asyn connections to `TCP1`, `TCP2`, and `TCP3`. Also, check for any errors during startup. Note that you might see Asyn trace messages related to `TCP3` because `ASYNTRACE=` was passed for that instance.

    * List PVs (Terminal 2): Once the IOC prompt appears (e.g., `7.0.7>`), use the `dbl` command to list loaded PVs. Verify that PVs for all three instances are present (e.g., `MYDEMO:SIM1:Cmd`, `MYDEMO:SIM1:Cmd-RB`, `MYDEMO:SIM2:Cmd`, etc., assuming `training.db` was loaded by the snippet).

    * Test Communication (Terminal 3): In the CA client terminal (ensure EPICS environment is sourced):
           
        * Test Instance 1: Send a command and read the reply using the PVs associated with `SIM1`:
        ```bash
        caput MYDEMO:SIM1:Cmd "Test1"
        caget MYDEMO:SIM1:Cmd-RB
        #Expected Reply: `MYDEMO:SIM1:Cmd-RB       Test1 (or specific handler response)
        ```
    * Test Instance 2: Repeat for `SIM2`:
        ```bash
        caput MYDEMO:SIM2:Cmd "Test2"
        caget MYDEMO:SIM2:Cmd-RB
        # Expected Reply: MYDEMO:SIM2:Cmd-RB       Test2 (or specific handler response)
        ```
    * Test Instance 3: Repeat for `SIM3`:
        ```
        caput MYDEMO:SIM3:Cmd "Test3"
        caget MYDEMO:SIM3:Cmd-RB
        # Expected Reply: MYDEMO:SIM3:Cmd-RB       Test3 (or specific handler response)
        ```
    Successfully sending commands and receiving the expected replies for each instance confirms that the multi-device configuration, using this specific `st.cmd` example, is working correctly.