# Chapter 4: Advanced IOC Configuration and Startup

This chapter significantly expands on IOC development by delving into advanced configuration techniques and the details of the IOC runtime environment. You will learn how to develop effectively an `iocsh` file and master database templating for scalable configurations. You will then apply these techniques to efficiently manage multiple similar devices within the IOC startup script (`st.cmd`), and finally explore the details of the different phases within that script.

This chapter covers the following topics:

* [Working with `iocsh`: Script Files and Commands](04.01.iocsh_basics.md): Developing startup script snippets (`*.iocsh` files) and understanding interactive shell usage.
* [Update the TCP Simulator](04.02.iocsimulator2.md): Developing the TCP simulator to handle multiple simulated device instances.
* [Managing Multiple Devices in `st.cmd`](04.03.multiple_devices.md): Applying templates and `iocsh` commands in the startup script for efficient configuration of multiple device connections.
* [A TC-32 Temperature Monitoring Device Simulator](04.04.iocsimulator3.md): Develop and understand the TC-32 temperature monitoring device simulator, which continuously streams data for 32 channels, illustrating the need for more efficient configuration methods for the EPICS record database.

* [Database Templates and Substitution](04.05.db_templates.md): Using `.template` and `.substitution` files with `Db/Makefile` for reusable database definitions.
* [IOC Startup Sequence (`st.cmd` Phases)](04.06.stcmd_phases.md): Learning about commands executed before and after `iocInit()`.
    * [Advanced `iocInit()`](04.06.01.adviocInit.md): Seperated Advanced `iocInit` description.

## Exercise

Do these on your own, without re-reading the sections above.

1. **Break it, then fix it.** Copy `st.cmd` to `st_break.cmd`, move `iocInit` above the `dbLoadRecords` line, and start the IOC. Note what behaves differently — there may be no hard error. Restore the order and say which phase each command belongs to.
2. **Combine it.** Write a `tc32_device.iocsh` snippet that configures one TC-32 device (Asyn port plus `dbLoadRecords` of the generated `TC-32.db`), then `iocshLoad` it twice in `st.cmd` for two emulator instances on different ports. Verify both PV sets with `caget`.
3. **Explain it.** In one sentence each: when does macro substitution happen for a `.template` loaded via `dbLoadRecords` versus the build-time generated `TC-32.db` — and why does the build-time method start faster?