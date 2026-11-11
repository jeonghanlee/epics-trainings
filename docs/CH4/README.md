# Chapter 4: Reuse device and database configurations

Follow the lessons in order; keep the application source for later exercises.

* [Move device configuration into an iocshLoad fragment.](04.01.iocsh_basics.md)
* [Run independently configured request/reply simulators.](04.02.iocsimulator2.md)
* [Configure three device connections in one IOC.](04.03.multiple_devices.md)
* [Stream 32 temperature channels from one simulator.](04.04.iocsimulator3.md)
* [Load records directly, at build time, or with substitutions.](04.05.db_templates.md)
* [Explain the startup sequence and linked libraries.](04.06.stcmd_phases.md)
* [Read the Base initialization sequence.](04.06.01.adviocInit.md)
* [Locate each complete runnable example.](verified_examples.md)

## Chapter exercises

1. Copy the Chapter 3 `st.cmd` to `st_break.cmd`. Move `iocInit()`
   immediately before `dbLoadRecords`, start it, and inspect the startup
   error. Restore the original order and confirm normal communication.
2. In lesson 4.5, configure two TC32 simulators and verify all 64 PVs.
3. Explain which macros are resolved at build time and which remain for
   startup. Generating a flat database changes where expansion happens;
   it does not establish a measured startup-time advantage.

## Before moving on

- [ ] `jeonglee_Demo/` retains `iocsh/training_device.iocsh`, `simulator/advanced_connection_handler.sh`, and `simulator/tc32_emulator.bash` for Chapters 5 and 6.
- [ ] `jeonglee_Demo/db/TC-32.db` is rebuilt from restored sources so `head -n 7 db/TC-32.db` shows `$(P)$(R)CH01` and `$(PORT)`.
- [ ] Every `jeonglee_Demo` IOC is stopped with `exit` and every simulator is stopped with Ctrl+C.
- [ ] `ss -ltn 'sport = :9399 or sport = :9400 or sport = :9401'` reports no active TCP listeners.
