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