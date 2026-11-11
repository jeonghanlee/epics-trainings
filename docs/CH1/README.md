# Chapter 1: Select and verify the environment

Follow the lessons in order; keep the application source for later exercises.

* [Install the binary distribution matching your OS.](01.01.installation.md)
* [Read and write a PV through CA and PVA.](01.02.testenv.md)
* [Distinguish host architecture from the OS directory.](01.03.epicshostarch.md)
* [Compare the three IOC startup methods.](01.04.module_loader.md)

## Chapter exercises

1. Add the air temperature record and repeat the failed-search exercise
   in [lesson 1.2](01.02.testenv.md). Restore the client address afterward.
2. Print `EPICS_HOST_ARCH` and explain why `linux-x86_64` does not identify
   a compatible Debian, Ubuntu, or Rocky binary tree.
3. Stop the IOC with `exit` before starting Chapter 2.

## Before moving on

- [ ] `EPICS_BASE` and `EPICS_HOST_ARCH` come from the `setEpicsEnv.bash` script matching your operating system.
- [ ] `EPICS_CA_ADDR_LIST` is restored to `127.0.0.1` after the failed-search check in lesson 1.2.
- [ ] Every `softIocPVX` session from lesson 1.2 is stopped with `exit`.
- [ ] `~/ioc-training/first/water.db` stays outside the EPICS distribution directory.
