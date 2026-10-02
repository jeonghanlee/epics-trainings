# Chapter 2: First EPICS IOC

Now that your environment is set up, this chapter walks you through creating your first EPICS Input/Output Controller (IOC) within the ALS-U EPICS Environment.

This chapter covers the following topics:

* [First EPICS IOC](02.01.yourfirstioc.md): Building a basic EPICS IOC from scratch.
* [Expand the First IOC](02.02.addioctofirst.md): Adding more functionality to your initial IOC.

## Exercise

Do these on your own, without re-reading the sections above.

1. **Break it, then fix it.** In `configure/RELEASE`, point `ASYN` at a directory that does not exist (for example `ASYN = $(MODULES)/asyn_missing`), then run `make` and read the error. Restore the original path, rebuild, and explain in one sentence how the `ifneq ($(ASYN),)` blocks in `mouseApp/src/Makefile` relate to what you just saw.
2. **Do it alone.** Add a third instance with a location of your choice (for example `-l lab -p mouse`), build it with `make`, and run its `./st.cmd` until you see the IOC shell prompt.
3. **Diff it.** Run `diff iocBoot/iochome-mouse/st.cmd iocBoot/iocpark-mouse/st.cmd`, then `diff iocBoot/iochome-mouse/st.cmd iocBoot/iocpark-woodmouse/st.cmd`. For each differing line, say which generator option (`-l`, `-p`, `-d`, `-f`) caused it.
