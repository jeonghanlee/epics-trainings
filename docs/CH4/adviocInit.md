# `iocInit()` for For advanced or challenge users 

The `iocInit()` command is the critical point in the startup script. While you typically just call `iocInit`, it's actually implemented as two distinct phases internally: `iocBuild` and `iocRun`. The `iocInit` command executes both of these phases sequentially to bring the IOC fully online. Understanding these phases provides a deeper insight into the startup sequence.

The initialization process, as performed by `iocInit` (which encompasses `iocBuild` followed by `iocRun`), consists of the following detailed steps:

## Phase 1: `iocBuild` (Building the IOC's Structure - Still Quiescent)

This phase sets up the core environment and loads/initializes the static configuration but does *not* yet start the main processing or I/O threads.

1.  **Configure Main Thread:**
    * The main thread's execution environment is prepared.
    * `initHookAtIocBuild` is announced (allowing registered functions to run at this specific point).
    * The message "Starting iocInit" is logged.
    * On Unix, signals like `SIGHUP` are configured to be ignored, preventing unexpected shutdown.
    * `initHookAtBeginning` is announced.

2.  **General Purpose Modules:**
    * `coreRelease` is called, typically printing the EPICS Base version.
    * `taskwdInit` is called to start the task watchdog system, which monitors other tasks.
    * `callbackInit` is called to start the general-purpose callback tasks used for scheduling work at different priorities.
    * `initHookAfterCallbackInit` is announced.

3.  **Channel Access Links:**
    * `dbCaLinkInit` initializes the module for handling Channel Access links between records. Its processing task is *not* started yet.
    * `initHookAfterCaLinkInit` is announced.

4.  **Driver Support:**
    * `initDrvSup` is called. This routine finds all registered driver support entry tables and calls each driver's `init` routine.
    * `initHookAfterInitDrvSup` is announced.

5.  **Record Support:**
    * `initRecSup` is called. This routine finds all registered record support entry tables and calls the module-level `init` routine for each record type.
    * `initHookAfterInitRecSup` is announced.

6.  **Device Support (Initial Call):**
    * `initDevSup` is called for the first time. This routine finds all registered device support entry tables and calls their `init` routine, indicating this is the initial call.
    * `initHookAfterInitDevSup` is announced.

7.  **Database Records:**
    * `initDatabase` is called, performing a multi-pass initialization of the loaded record instances:
        * **Pass 1:** Initializes basic record fields (like `RSET`, `PACT`) and calls record support's `init_record` function for each record.
        * **Pass 2:** Converts `PV_LINK` fields into either `DB_LINK` (if the target PV is in the same IOC) or `CA_LINK` (if the target is remote). Extended device support's `add_record` routine may be called.
        * **Pass 3:** Calls record support's `init_record` function again for each record (second pass).
    * An exit routine is registered to handle database shutdown when the IOC exits.
    * `dbLockInitRecords` is called to set up the database lock sets.
    * `dbBkptInit` initializes the database debugging module.
    * `initHookAfterInitDatabase` is announced.

8.  **Device Support (Final Call):**
    * `initDevSup` is called for a second and final time. This allows device support to perform any final setup that requires the database records to be fully initialized and linked.
    * `initHookAfterFinishDevSup` is announced.

9.  **Scanning and Access Security:**
    * `scanInit` initializes the periodic, event, and I/O event scanners, but the scan threads are created in a state where they cannot process records yet.
    * `asInit` initializes the Channel Access access security system. If this fails, IOC initialization is aborted.
    * `dbProcessNotifyInit` initializes support for process notification.
    * After a short delay, `initHookAfterScanInit` is announced.

10. **Initial Processing:**
    * `initialProcess` is called. This function finds all records that have their `PINI` (Process at Initialization) field set to `YES` and triggers a processing cycle for them.
    * `initHookAfterInitialProcess` is announced.

11. **Channel Access Server (Initial Setup):**
    * `rsrv_init` is called to initialize the Channel Access server structures, but its network tasks are not yet allowed to run, so it doesn't announce its presence on the network.
    * `initHookAfterCaServerInit` is announced.

At this point, the `iocBuild` phase is complete. The IOC's structure is built, configurations loaded, and support initialized, but it is still in a quiescent state. `initHookAfterIocBuilt` is announced. If you had started with `iocBuild`, the command would finish here.

## Phase 2: `iocRun` (Bringing the IOC Online)

This phase activates the threads and processes that allow the IOC to actively monitor hardware, process records, and communicate via Channel Access.

12. **Enable Record Processing:**
    * `initHookAtIocRun` is announced.
    * `scanRun` is called, which starts the scan threads and sets the global variable `interruptAccept` to `TRUE`. This variable acts as a flag indicating that the IOC is ready to handle I/O interrupts.
    * `dbCaRun` is called, which enables the Channel Access link processing task.
    * `initHookAfterDatabaseRunning` is announced.
    * If this is the first time `iocRun` (or `iocInit`) is executed, `initHookAfterInterruptAccept` is announced.

13. **Enable CA Server:**
    * `rsrv_run` is called. This allows the Channel Access server tasks to begin running and announce the IOC's presence to the network.
    * `initHookAfterCaServerRunning` is announced.
    * If this is the first time, `initHookAtEnd` is announced.
    * A command completion message is logged, and `initHookAfterIocRunning` is announced.

Once `iocInit()` (completing both `iocBuild` and `iocRun`) finishes, the IOC is **"live."** Records configured for scanning will begin processing, Channel Access clients can connect and interact with the PVs, and the IOC is actively monitoring and controlling the connected hardware.
