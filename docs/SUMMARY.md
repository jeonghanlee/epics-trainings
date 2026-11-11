# Summary

[ALS-U EPICS environment training](README.md)
[Install the training prerequisites](prerequisites.md)
---
# Chapter 1
- [Select and verify the environment](CH1/README.md)
    - [Install the EPICS environment](CH1/01.01.installation.md)
    - [Run your first IOC](CH1/01.02.testenv.md)
    - [Understand architecture and OS trees](CH1/01.03.epicshostarch.md)
    - [IOC startup choices](CH1/01.04.module_loader.md)

# Chapter 2
- [Generate and expand IOC applications](CH2/README.md)
    - [Generate and run an IOC application](CH2/02.01.yourfirstioc.md)
    - [Add IOC instances to one application](CH2/02.02.addioctofirst.md)

# Chapter 3
- [Connect an IOC to a TCP simulator](CH3/README.md)
    - [Configure an IOC for TCP communication](CH3/03.01.yoursecondioc.md)
    - [Run a TCP echo simulator](CH3/03.02.iocsimulator.md)
    - [Test IOC communication with the simulator](CH3/03.03.secondiocwithsim.md)

# Chapter 4
- [Reuse device and database configurations](CH4/README.md)
    - [Reuse a device startup fragment](CH4/04.01.iocsh_basics.md)
    - [Run a command-aware TCP simulator](CH4/04.02.iocsimulator2.md)
    - [Connect three devices to one IOC](CH4/04.03.multiple_devices.md)
    - [Run a 32-channel temperature simulator](CH4/04.04.iocsimulator3.md)
    - [Generate and load 32 channel records](CH4/04.05.db_templates.md)
    - [IOC startup sequence](CH4/04.06.stcmd_phases.md)
        - [EPICS Base initialization reference](CH4/04.06.01.adviocInit.md)
    - [Example files](CH4/verified_examples.md)

# Chapter 5
- [Inspect IOC application configuration](CH5/README.md)
    - [Compare IOC shell command styles](CH5/05.01.how-to-ioc-log1.md)
    - [Dependency paths in RELEASE](CH5/05.02.how-to-ioc-log2.md)
    - [Change an application's installation path](CH5/05.03.how-to-ioc-log3.md)
    - [Run an operating-system command](CH5/05.04.how-to-ioc-log4.md)
---
# Chapter 6
- [Run IOCs with iocsh.bash](CH6/README.md)
    - [Run an IOC with installed modules](CH6/06.01.first_loader.md)
    - [Connect the TCP simulator](CH6/06.02.tcp_loader.md)
    - [Read 32 temperature channels](CH6/06.03.tc32_loader.md)
    - [Module selection and loader errors](CH6/06.04.loader_reference.md)

[EPICS client discovery variables](misc/EPICSEnv.md)
[Further training topics](misc/FutureLessonsList.md)
[Contributors](misc/contributors.md)

[Training glossary](glossary.md)
