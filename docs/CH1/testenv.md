# 1.2 Your First Running EPICS IOC
In this section, you will run your very first EPICS Input/Output Controller (IOC) using the standard `softIocPVX` executable included in the environment. You will create a minimal database file defining a single Process Variable (PV) and then use essential EPICS command-line tools like caget, caput, and pvxget to interact with this PV over the network, demonstrating both Channel Access (CA) and the newer PV Access (PVA) protocols.

## Lesson Overview
In this lesson, you will learn how to do the following:
* Create a minimal EPICS database (`.db`) file.
* Run a simple IOC using `softIocPVX`.
* List PVs within the running IOC using `dbl`.
* Interact with PVs using `caget` and `pvxget` (Read).
* Interact with PVs using `caput` and `pvxput` (Write).
* Perform basic connection troubleshooting for CA and PVA.

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

1. Open a new, separate terminal window, which we call it `Terminal 2`
2. Source the ALS-U EPICS environment with `disable` option, which suppress output messages in this new terminal as well: 
```shell
# In Terminal 2 (CA/PVA Clients)
$ source ~/epics/1.1.1/debian-12/7.0.7/setEpicsEnv.bash "disable"
```

3. Try reading the initial value and description using Channel Access (`caget`) and PV Access (`pvxget`), then write new values using caput (`CA`) and pvxput (`PVA`), and read them back:

```shell
# In Terminal 2 (CA/PVA Clients)
$ caget temperature:water      # Reads the VAL field (e.g., 0) using CA
$ caget temperature:water.DESC # Reads the DESC field using CA (e.g., "Water temperature Setpoint")
$ pvxget temperature:water     # Reads the value using PVA (shows structure, e.g., { "value": 0.0 })

# Write value 24 using CA
$ caput temperature:water 24
# Read back using CA and PVA
$ caget temperature:water      # Should show 24
$ pvxget temperature:water     # Should show { "value": 24.0 }

# Write value 44 using PVA
$ pvxput temperature:water 44
# Read back using CA
$ caget temperature:water      # Should show 44
```
The commands `caget`, `pvxget`, and `caput` are simple EPICS command-line clients. `caget` and `caput` use the CA (Channel Access) network protocol, while `pvxget` (and `pvxput`) use the newer PVA (PV Access) protocol available in EPICS 7+. These network protocols are fundamental to how different EPICS components (IOCs, clients, services) communicate.

## Troubleshooting
If your `caput` or `caget` commands fail with a message like `Channel connect timed out: PVNAME not found.`, it means the CA client tools cannot find your running IOC over the network.

```shell
# Example Error:
$ caget caget temperature:water 
Channel connect timed out: 'caget temperature:water' not found.
```

When running the IOC and CA clients on the same machine (like localhost), this often happens because the default CA broadcast mechanism isn't sufficient or is blocked. You need to explicitly tell the CA clients where to find the IOC server using an environment variable:

1. Set `EPICS_CA_ADDR_LIST`: In the terminal where you run `caput`/`caget` (Terminal 2), set this variable to point to the machine running the IOC (in this case, localhost).
```shell
# In Terminal 2 (CA Clients)
$ export EPICS_CA_ADDR_LIST=localhost
```

2. Retry the command:
```shell
# In Terminal 2 (CA Clients)
$ caget temperature:water
# Expected output (should now work, showing the current value):
temperature:water    44
```
3. If you would like to evaluate the PVA protocol, you also have to define the following EPICS environment variable `EPICS_PVA_ADDR_LIST` for PVA (Process Variable Access) protocol. We will cover PVA protocol for more advanced lesson later.

```shell
# In Terminal 2 (PVA Clients)
$ export EPICS_PVA_ADDR_LIST=localhost
$ pvxget temperature:water
# Expected output (should now work, showing structure):
# ...
# { "value": 44.0 }
# ...
```

### Rocky or Redhat Variant Firewall
Rocky Linux (Redhat Variant) has its own `firewalld` service running by default. It blocks the CA and PVA communication needed for EPICS. Thus, one should `stop` and `disable` the service for this training overall.
```shell
# Run these commands with administrator privileges (e.g., using sudo)
sudo systemctl stop firewalld      # stop the firewalld service
sudo systemctl disable firewalld   # unmarks the firewalld for autostart
sudo systemctl mask firewalld      # prevent the firewalld from being started
``` 
Note that you can edit the `firewalld` configuration to allow specific ports, but this is out-of-scope of this introductory training.


## Summary
In this lesson, you successfully performed the essential first steps in working with an EPICS IOC:

* Created a minimal database file (`water.db`) containing an Analog Output (`ao`) record named `temperature:water`.
* Ran a basic IOC using the `softIocPVX` executable, loading your database file.
* Verified the loaded PV using the dbl command in the IOC shell.
* Used command-line tools (`caget`, `caput`, `pvxget`, `pvxput`) to interact with the PV over the network using both Channel Access (CA) and PV Access (PVA) protocols.
* Learned basic troubleshooting steps for network connectivity issues involving `EPICS_CA_ADDR_LIST`, `EPICS_PVA_ADDR_LIST`, and system firewalls (`firewalld`).