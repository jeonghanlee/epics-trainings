# Chapter 6: Run IOCs with iocsh.bash

Run an input/output controller (IOC) using installed module support.
The `iocsh.bash` loader selects the required modules and starts
`softIocPVX`; your startup file configures the device and loads its records.

Complete Chapter 1 first. The device lessons reuse the files from
Chapters 3 and 4.

* [Run an IOC with installed modules](06.01.first_loader.md):
  create one PV, select `busy`, and verify CA and PVA access.
* [Connect the TCP simulator](06.02.tcp_loader.md):
  reuse the echo protocol and database without an application executable.
* [Read 32 temperature channels](06.03.tc32_loader.md):
  load the generated TC-32 database and check every channel.
* [Select modules and diagnose startup](06.04.loader_reference.md):
  choose exact versions, inspect a startup, and recognize loader errors.

The loader supports installed record and device support. Application-specific
C/C++ code must be built separately before it can be loaded.
