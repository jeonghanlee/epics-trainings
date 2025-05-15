 Summary

[Introduction](introduction.md)
-----------
# Chapter 1
- [Getting Started](CH1/README.md)
    - [Installation](CH1/01.01.installation.md)
    - [Test Environment](CH1/01.02.testenv.md)
    - [Host Architecture and OS-Specific folder](CH1/01.03.epicshostarch.md)

# Chapter 2
- [First EPICS IOC and Gitlab CI](CH2/README.md)
    - [First EPICS IOC](CH2/02.01.yourfirstioc.md)
    - [Expand the First IOC](CH2/02.02.addioctofirst.md)
    - [Continous Integration](CH2/02.03.ciwithgitlab.md)

# Chapter 3
- [Second EPICS IOC and Serial Simulator](CH3/README.md)
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

# Chapter 5
- [How To's Collection](CH5/README.md)
    - [Style of `st.cmd`](CH5/05.01.how-to-ioc-log1.md): Style of `st.cmd` Commands
    - [`RELEASE` file](CH5/05.02.how-to-ioc-log2.md): Deep Insight on `configure/RELEASE`
    - [`CONFIG_SITE` file](CH5/05.03.how-to-ioc-log3.md): `configure/CONFIG_SITE` - Controlling Application-Specific Build Options
    - [`system.dbd`](CH5/05.04.how-to-ioc-log4.md): What `system.dbd` file is
----------
[EPICS environment](misc/EPICSEnv.md)
[Future Lessons](misc/FutureLessonsList.md)
[Contributors](misc/contributors.md)
