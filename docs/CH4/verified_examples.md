# Chapter 4 example files

Use the complete file definitions in the lesson where each file is introduced.
Run one IOC startup at a time.

| Goal | Files and lesson |
| --- | --- |
| Configure one TCP connection | [training_device.iocsh and st2.cmd](04.01.iocsh_basics.md) |
| Connect three TCP devices | [st3.cmd](04.03.multiple_devices.md) |
| Generate a 32-record database | [temperature.template and TC-32.substitutions](04.05.db_templates.md#define-the-protocol-and-template) |
| Compare database loading methods | [st.cmd, st2.cmd, and st3.cmd](04.05.db_templates.md#verify-each-loading-method) |
| Connect several temperature devices | [TC-32.iocsh and st_multi.cmd](04.05.db_templates.md#configure-several-temperature-devices) |

The application is `jeonglee_Demo`. The TCP lessons use
`iocBoot/iocB46_182-jeonglee_Demo/`; the temperature lessons use
`iocBoot/ioctraining-jeonglee_Demo/`.

Use the lesson's verification commands to confirm record names, metadata,
live values, and shutdown. A successful build or visible IOC prompt alone
does not establish device communication.
