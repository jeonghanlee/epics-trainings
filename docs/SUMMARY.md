# Summary

[Training contents](README.md)

[Introduction](introduction.md)
[Prerequisites: Required System Packages](prerequisites.md)
-----------
# Chapter 1
- [Getting Started](CH1/README.md)
    - [Installation](CH1/01.01.installation.md)
    - [Test Environment](CH1/01.02.testenv.md)
    - [Host Architecture and OS-Specific folder](CH1/01.03.epicshostarch.md)
    - [IOC startup choices](CH1/01.04.module_loader.md)

# Chapter 2
- [First EPICS IOC](CH2/README.md)
    - [First EPICS IOC](CH2/02.01.yourfirstioc.md)
    - [Expand the First IOC](CH2/02.02.addioctofirst.md)

# Chapter 3
- [Second IOC and TCP simulator](CH3/README.md)
    - [Configure the Second IOC](CH3/03.01.yoursecondioc.md)
    - [Create the TCP Simulator](CH3/03.02.iocsimulator.md)
    - [Test IOC-Simulator Communication](CH3/03.03.secondiocwithsim.md)

# Chapter 4
- [Advanced IOC Configuration and Startup](CH4/README.md)
    - [Working with `iocsh`: Script Files and Commands](CH4/04.01.iocsh_basics.md)
    - [Extending the TCP Simulator](CH4/04.02.iocsimulator2.md)
    - [Managing Multiple Devices in `st.cmd`](CH4/04.03.multiple_devices.md)
    - [A TC-32 Temperature Monitoring Device Simulator](CH4/04.04.iocsimulator3.md)
    - [Database Templates and Substitution](CH4/04.05.db_templates.md)
    - [IOC Startup Sequence (`st.cmd` Phases)](CH4/04.06.stcmd_phases.md)
        - [Advanced `iocInit()` Note](CH4/04.06.01.adviocInit.md)
    - [Verified Working Examples](CH4/verified_examples.md)

# Chapter 5
- [How To's Collection](CH5/README.md)
    - [Style of `st.cmd`](CH5/05.01.how-to-ioc-log1.md): Style of `st.cmd` Commands
    - [`RELEASE` file](CH5/05.02.how-to-ioc-log2.md): Deep Insight on `configure/RELEASE`
    - [`CONFIG_SITE` file](CH5/05.03.how-to-ioc-log3.md): `configure/CONFIG_SITE` - Controlling Application-Specific Build Options
    - [`system.dbd`](CH5/05.04.how-to-ioc-log4.md): What `system.dbd` file is
----------
# Chapter 6
- [Run IOCs with iocsh.bash](CH6/README.md)
    - [First loader IOC](CH6/06.01.first_loader.md)
    - [Connect the TCP simulator](CH6/06.02.tcp_loader.md)
    - [Read 32 temperature channels](CH6/06.03.tc32_loader.md)
    - [Module selection and loader errors](CH6/06.04.loader_reference.md)

[EPICS environment](misc/EPICSEnv.md)
[Future Lessons](misc/FutureLessonsList.md)
[Contributors](misc/contributors.md)

[Glossary](glossary.md)
