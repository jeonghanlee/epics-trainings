# Your First Running EPICS IOC

## Lesson Overview

In this lesson, you will learn how to do the following:
* Run a simple IOC
* Play with `caget`, `caput`, and `camonitor` commands
* Play with `pvxget`, `pvxput`, and `pvxmonitor` commands 

## Make an EPICS Database file

Please create the following EPICS database (db) file with the name as `water.db`[^note] with the following content:

```c
record(ai, "temperature:water")
{
    field(DESC, "Water temperature in the fish tank")
    field(VAL, "0")
}
```

This file defines a record instance called `temperature:water`, which is an analog input `ai` record.
As you can imagine, `DESC` stays for `Description`, and `VAL` is for the initial value.


## Run your first `softioc`

Now we start `softIocPVX` using this record database.

```bash
$ source ~/epics/1.1.1/debian-12/7.0.7/setEpicsEnv.bash
$ softIocPVX -d water.db
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

Type `dbl`, which shows the current running IOC EPICS database record (PV) list

```bash
7.0.7 > dbl
temperature:water
7.0.7 >
```

## Play with EPICS command line tools

Open a new terminal (let us call it `TL2`), and source the ALS-U EPICS environment, and then try the command line tools

```bash
training (master)$ source ~/epics/1.1.1/debian-12/7.0.7/setEpicsEnv.bash "disable"

EPICS_BASE is defined as /home/jeonglee/epics/1.1.1/debian-12/7.0.7/base

$ caget temperature:water
$ caget temperature:water.DESC
$ pvxget temperature:water
$ caput temperature:water 24
$ caget temperature:water
$ pvxget temperature:water
$ pvxput temperature:water 44
$ caget te=mperature:water
```

The commands `caget`, `pvxget`, and `caput` are the simplest EPICS CA/PVA Clients. As you can imagine, `caget` and `caput` use the CA protocol, while `pvxget` uses the PVA protocol. `camonitor` and `pvxmonitor` allow you to monitor changes to the PV. These network communication protocols are fundamental to how EPICS works. We often refer to EPICS communication as the `CA/PVA` network to distinguish it from other types of network communication.

## Reference 

[^note]:  [DB file reference can be found at EPICS site](https://epics-controls.org/resources-and-support/documents/getting-started)
