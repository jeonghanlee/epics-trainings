# EPICS client discovery variables

The exercises use explicit loopback searches so clients find an IOC
on the same host. Activate the selected EPICS tree in every terminal.

| Variable | Role in these lessons |
| --- | --- |
| `EPICS_CA_ADDR_LIST` | UDP destinations for Channel Access searches |
| `EPICS_CA_AUTO_ADDR_LIST` | Adds automatic interface broadcast addresses when `YES` |
| `EPICS_PVA_ADDR_LIST` | UDP destinations for pvAccess searches |
| `EPICS_PVA_AUTO_ADDR_LIST` | Adds automatic interface broadcast addresses when `YES` |
| `EPICS_PVA_NAME_SERVERS` | Optional TCP name servers for PVA discovery |

For an IOC on this host, use these client settings:

```bash
export EPICS_CA_AUTO_ADDR_LIST=NO EPICS_CA_ADDR_LIST=127.0.0.1
export EPICS_PVA_AUTO_ADDR_LIST=NO EPICS_PVA_ADDR_LIST=127.0.0.1
```

An explicit address list and automatic addresses are separate settings.
Setting a list does not by itself disable automatic broadcast searches.
Use addresses reachable from the client for remote IOCs; `127.0.0.1`
always refers to the client's own host.

To inspect this host's IPv4 addresses, use:

```bash
ip -4 addr show
```

Discovery locates the server; subsequent data operations use a TCP
connection. Successful discovery alone does not establish that a device
is connected or a record has valid data. Read the value and its alarms.

The [PVXS network configuration reference](https://epics-base.github.io/pvxs/netconfig.html)
describes PVA client and server settings beyond these local exercises.
