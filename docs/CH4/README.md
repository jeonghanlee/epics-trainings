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
