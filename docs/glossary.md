# Training glossary

These terms identify the components used throughout the exercises.

| Term | Meaning |
| --- | --- |
| Application | A source tree built into an IOC executable and installed configuration files |
| asyn port | A named communication interface used by device support |
| Boot instance | One startup directory that configures an IOC application for a location or device |
| CA | Channel Access, the protocol used by `caget`, `caput`, and `camonitor` |
| Database | A file defining record instances and their fields |
| DBD | Database definition: available record types, device support, drivers, and registrars |
| EOS | End-of-string delimiter used to separate device messages |
| IOC | Input/Output Controller, the EPICS process serving and processing records |
| IOC shell | The command interpreter inside an IOC, also called iocsh |
| iocsh.bash | The EPICS-env launcher that resolves installed modules and starts `softIocPVX` |
| iocshLoad | An IOC shell command that executes another file with macro arguments |
| Macro | A named value substituted in a database or startup file |
| PVA | pvAccess, the protocol used by `pvxget` and `pvxput` |
| PV | Process variable, a named value exposed by an IOC |
| PVXS | The library providing PVA clients and QSRV2 support in this distribution |
| QSRV2 | PVXS support exposing EPICS database records over PVA |
| Record | One database instance containing a value, fields, processing behavior, and alarms |
| Registrar | A function that registers support or commands with the IOC |
| StreamDevice | Device support that exchanges text messages using protocol files |
| Substitution file | A file listing macro values used to instantiate a database template |
| Template | A database definition parameterized with macros |
| TOP | The application's top-level path, used during builds and IOC startup |
