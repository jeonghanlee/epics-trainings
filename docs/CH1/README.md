# Chapter 1: Environment Setup and Verification

Welcome to the ALS-U EPICS Environment documentation. This first chapter focuses on getting the environment operational. It covers the installation procedure and the essential steps to test your setup.

This chapter covers the following topics:

* [Installation](01.01.installation.md): Provides detailed steps to set up the ALS-U EPICS Environment.
* [Test Environment](01.02.testenv.md): Outlines how to launch and run tests to ensure the environment is functioning correctly after installation.
* [Host Architecture and OS-Specific folder](01.03.epicshostarch.md): Explains the environment's approach to host architecture support, focusing on `EPICS_HOST_ARCH`, the `linux-x86_64` standard, and the role of OS-specific directories.

## Exercise

Do these on your own, without re-reading the sections above.

1. **Break it, then fix it.** Open a fresh terminal and run `caget` without sourcing `setEpicsEnv.bash`. Note the failure. Then source the script, start `softIocPVX -d water.db`, and in the client terminal check that `echo $EPICS_CA_ADDR_LIST` shows a value (if not, `export EPICS_CA_ADDR_LIST=localhost` and confirm `caget temperature:water` works). Then `unset EPICS_CA_ADDR_LIST`, run `caget temperature:water` again, observe the timeout, restore the value, and confirm the read works.
2. **Extend it.** Copy `water.db` to `air.db`, add a second `ao` record named `temperature:air` with an initial `VAL` of `20`, and bring up the IOC yourself. Verify both PVs with `dbl` and `caget`.
3. **Explain it.** Print `EPICS_HOST_ARCH` in your terminal and, in one sentence, explain why the environment keeps the architecture name (`linux-x86_64`) separate from the OS folder name (e.g., `ubuntu-24.04`).
