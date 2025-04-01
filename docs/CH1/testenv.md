# Your First Running EPICS IOC

In this chapter, you will run your very first EPICS Input/Output Controller (IOC) using the standard `softIocPVX` executable included in the environment. You will create a minimal database file defining a single Process Variable (PV) and then use essential EPICS command-line tools like caget, caput, and pvxget to interact with this PV over the network, demonstrating both Channel Access (CA) and the newer PV Access (PVA) protocols.

## Lesson Overview

In this lesson, you will learn how to do the following:
* Create a minimal EPICS database (`.db`) file.
* Run a simple IOC using `softIocPVX`.
* List PVs within the running IOC using `dbl`.
* Interact with PVs using `caget` and `pvxget` (Read).
* Interact with PVs using `caput` and `pvxput` (Write).

## Make an EPICS Database file

Please create the following EPICS database (`.db`) file with the name as `water.db`. This file defines a single Process Variable (PV).

```c
record(ao, "temperature:water")
{
    field(DESC, "Water temperature Setpoint") # Description
    field( EGU, "degC")                       # Engineering Units
    field( VAL, "0")                          # Initial Value
}
```

This file defines a record instance named `temperature:water`.
* `ao`: Specifies the record type is Analog Output.
* `DESC`: Stands for Description (a text string). 
* `EGU`: Defines the Engineering Units for the value.
* `VAL`: Holds the current value (and initial value) of the record.

Save this content as `water.db`.

## Run your first `softioc`

Now we start the `softIocPVX` executable (a generic IOC program included with EPICS base) using the database file we just created.

1. Open your first terminal window.
2. Ensure your EPICS enviornment is sourced:

```bash
# # In Terminal 1 (IOC)
$ source ~/epics/1.1.1/debian-12/7.0.7/setEpicsEnv.bash
```
3. Run `softIocPVX`, telling it to load (`-d`) your database file (`water.db`). Make sure you run this in the same directory where you saved `water.db`.
```bash
# In Terminal 1 (IOC)
$ softIocPVX -d water.db
# Expected Output (will vary slightly):
INFO: PVXS QSRV2 is loaded, permitted, and ENABLED.
Starting iocInit
############################################################################
## EPICS R7.0.7-github.com/jeonghanlee/EPICS-env
## Rev. R7.0.7-dirty
## Rev. Date Git: 2022-09-07 13:50:35 -0500
############################################################################
iocRun: All initialization complete
7.0.7 >
```
Leave this terminal running.

4. At the IOC shell prompt (`7.0.7 >`), type `dbl` (database list) and press Enter to see the PVs loaded:

```bash
# In Terminal 1 (IOC)
7.0.7 > dbl
temperature:water # Should list the PV you defined
7.0.7 >
```

## Play with EPICS command line tools

Now, let's interact with the running IOC from a different terminal.

1. Open a new, separate terminal window
2. Source the ALS-U EPICS environment with `disable` option, which suppress output messages in this new terminal as well: 
```bash
# In Terminal 2 (CA/PVA Clients)
$ source ~/epics/1.1.1/debian-12/7.0.7/setEpicsEnv.bash "disable"
```

3. Try reading the initial value and description using Channel Access (caget) and PV Access (pvxget): 

```bash
# In Terminal 2 (CA/PVA Clients)
$ caget temperature:water      # Reads the VAL field by default using CA
$ caget temperature:water.DESC # Reads the DESC field using CA
$ pvxget temperature:water     # Reads the value using PVA (structure output)
$ caput temperature:water 24
$ caget temperature:water
$ pvxget temperature:water
$ pvxput temperature:water 44
$ caget temperature:water
```

The commands `caget`, `pvxget`, and `caput` are simple EPICS command-line clients. `caget` and `caput` use the CA (Channel Access) network protocol, while `pvxget` (and `pvxput`) use the newer PVA (PV Access) protocol available in EPICS 7+. These network protocols are fundamental to how different EPICS components (IOCs, clients, services) communicate.