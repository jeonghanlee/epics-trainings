# Chapter 6: Run IOCs with iocsh.bash

Run an Input/Output Controller (IOC) using installed module support without
compiling an application binary. The `iocsh.bash` loader resolves module
dependencies, loads shared libraries and database definition (DBD) files, and
starts `softIocPVX` with your startup configuration.

Complete Chapter 1 first. Lessons 6.2 and 6.3 reuse the simulator scripts,
databases, protocols, and fragments created in Chapters 3 and 4.

* [Run an IOC with installed modules](06.01.first_loader.md):
  inspect `iocsh.bash -n` output, load `busy`, and verify Channel Access (CA)
  and pvAccess (PVA) access.
* [Connect the TCP simulator](06.02.tcp_loader.md):
  run single-device and three-device `StreamDevice` startups by reusing
  `training_device.iocsh` unchanged.
* [Read 32 temperature channels](06.03.tc32_loader.md):
  load `TC-32.db` and expose a composite `QSRV2` group process variable (PV)
  with `dbLoadGroup`.
* [Add host statistics, autosave, and regex search](06.04.common_services.md):
  combine `linStat`, `autosave`, and `retools` with `commonIocsh` to persist a
  setpoint across restarts.
* [Module selection and loader errors](06.05.loader_reference.md):
  pin module versions, check exported macros in `cfg/iocsh.conf`, and reproduce
  four startup error checks.

`iocsh.bash` loads installed record and device support. Application-specific C
or C++ code requires a compiled IOC application as shown in Chapters 2 through
5.

## Chapter exercises

1. In [lesson 6.3](06.03.tc32_loader.md), add a `ch08` `scalar` mapping for
   `$(P)$(R)CH08.VAL` to `tc32_group.json`. Stop `tc32_emulator.bash` in
   terminal 1 with Ctrl+C, enter `exit` in terminal 2, start
   `./tc32_emulator.bash --port 9399` again in terminal 1, and restart
   `iocsh.bash st.cmd` in terminal 2. Confirm that `pvxgl(1, "*")` reports
   `Atomic Members:5` and `pvxget LOADER:TC32:Summary` includes `ch08.value`.
2. In [lesson 6.4](06.04.common_services.md), add a second `ao` record named
   `$(P)Limit` with `field(PINI, "YES")` and `info(autosaveFields, "VAL")`.
   Write a value with `caput`, wait 6 seconds, restart the IOC, and verify that
   `settings.sav` restores both records.
3. Run `iocsh.bash -n` on `st.cmd` from [lesson 6.4](06.04.common_services.md)
   and explain why `commonIocsh` fragments (`linStat.iocsh` and
   `autosave.iocsh`) use `iocshLoad` rather than `module` directives.

## Before moving on

- [ ] Every `iocsh.bash` (`softIocPVX`) session is stopped with `exit`.
- [ ] Every `tcpserver.bash` and `tc32_emulator.bash` process is stopped with Ctrl+C.
- [ ] `ss -ltn 'sport = :9399 or sport = :9400 or sport = :9401'` reports no active TCP listeners.
- [ ] Temporary scratch directories from lessons 6.1 and 6.5 are removed or noted outside the EPICS distribution tree.
