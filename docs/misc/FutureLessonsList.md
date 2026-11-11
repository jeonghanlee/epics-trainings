# Further training topics

Chapters 1 through 6 cover local Input/Output Controller (IOC) applications,
text device protocols, database templates, `iocsh.bash` module loading,
introductory `QSRV2` group mapping, and local `autosave` and `linStat` startup
fragments. These advanced production topics belong in separate courses:

* Record processing chains, hardware links, and alarm severity design.
* Production `autosave` policies, NFS storage layouts, and `asVerify` audits
  beyond the local `commonIocsh` setup in
  [lesson 6.4](../CH6/06.04.common_services.md).
* Fleet-wide IOC health monitoring, `caPutLog` auditing, and `recsync` service
  discovery beyond host and process metrics in
  [lesson 6.4](../CH6/06.04.common_services.md).
* Sequencer (`snc`/`seq`) state-machine programs.
* Full three-layer pvAccess (PVA) device object architecture, write ordering
  (`+putorder`), and legacy proxy IOCs beyond the compact `dbLoadGroup` summary
  in [lesson 6.3](../CH6/06.03.tc32_loader.md).
* Managed IOC daemons with `procServ` and `systemd`.
* Site deployment and operational acceptance testing.

These subjects extend the exercises in this book and are not prerequisites for
Chapters 1 through 6.
