# Short Summary on EPICS Environment Variables

This is a short reminder which we forget frequently.

## Channel Access

### `EPICS_CA_ADDR_LIST` 

* `EPICS_CA_ADDR_LIST` determines where to search
* `EPICS_CA_ADDR_LIST` is a list (seperated by spaces): `"123.45.1.255 123.15.2.14 123.45.2.108"`
* The default of `EPICS_CA_ADDR_LIST` is broadcast addresses of all interfaces on the host: It works when IOC servers are on the same subnet as IOC clients.
* Broadcast address goes to all servers on a subnet, for example, `123.45.1.255`

```bash
ifconfig -a |grep broadcast
        inet 172.17.0.1  netmask 255.255.0.0  broadcast 172.17.255.255
        inet 192.168.1.180  netmask 255.255.255.0  broadcast 192.168.1.255

ip addr show |grep -E 'inet' |grep -E 'brd'
    inet 192.168.1.180/24 brd 192.168.1.255 scope global dynamic noprefixroute wlo1
    inet 172.17.0.1/16 brd 172.17.255.255 scope global docker0
```


### `EPICS_CA_AUTO_ADDR_LIST`

* `YES` : it will include default addresses above in searches
* `NO`  : it does not search on default addresses
* If you set `EPICS_CA_ADDR_LIST`, usually set this to `NO`


## PV Access

[copy from https://epics-base.github.io/pvxs/netconfig.html]

A PV Access network protocol operation proceeds in two phases: PV name resolution, and data transfer. Name resolution is the process is determining which PVA server claims to provide each PV name. Once this is known, a TCP connection is open to that server, and the operation(s) are executed.

The PVA Name resolution process is similar to Channel Access protocol.

## `EPICS_PVA_ADDR_LIST` and `EPICS_PVA_NAME_SERVERS`, `EPICS_PVA_AUTO_ADDR_LIST`



When a name needs to be resolved, a PVA client will begin sending UDP search messages to any addresses listed in `EPICS_PVA_ADDR_LIST` and also via TCP to any servers listed in `EPICS_PVA_NAME_SERVERS` which can be reached.

UDP searches are by default sent to port `5076`, subject to `EPICS_PVA_BROADCAST_PORT` and port numbers explicitly given in `EPICS_PVA_ADDR_LIST`.

The addresses in `EPICS_PVA_ADDR_LIST` may include IPv4/6 unicast, multicast, and/or broadcast addresses. By default (cf. `EPICS_PVA_AUTO_ADDR_LIST`) the address list is automatically populated with the IPv4 broadcast addresses of all local network interfaces.

Searches will be repeated periodically in perpetuity until a positive response is received, or the operation is cancelled.


## Reference

[1] https://controlssoftware.sns.ornl.gov/training/2019_USPAS/Presentations/06%20Channel%20Access.pdf
[2] https://epics-base.github.io/pvxs/netconfig.html


