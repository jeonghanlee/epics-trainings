# Chapter 5: Understanding IOC Application Configuration

An EPICS Input/Output Controller (IOC) application requires configuration to define its behavior, load databases, initialize hardware interfaces, and set various parameters. The primary file responsible for the initial setup of an IOC instance is the startup script, typically named `st.cmd`. Additionally, other files like `configure/RELEASE`, `configure/CONFIG_SITE`, and `system.dbd` play crucial roles in defining dependencies, site-specific settings, and the overall database definition.

This chapter covers the following topics:

- [Style of `st.cmd`](05.01.how-to-ioc-log1.md): Style of `st.cmd` Commands
- [`RELEASE` file](05.02.how-to-ioc-log2.md): Deep Insight on `configure/RELEASE`
- [`CONFIG_SITE` file](05.03.how-to-ioc-log3.md): `configure/CONFIG_SITE` - Controlling Application-Specific Build Options
- [`system.dbd` file](05.04.how-to-ioc-log4.md): What `system.dbd` file is

## Exercise

Do these on your own, without re-reading the sections above.

1. **Break it, then fix it.** Create `configure/RELEASE.local` with a bogus `EPICS_BASE` path (e.g., `/nonexistent/epics/base`) and run `make`. Note what fails. Delete the file and rebuild to recover.
2. **Convert it.** Take the TC-32 `st2.cmd` from Chapter 4 and rewrite it fully in the space-separated style (like `st4.cmd` above), start the IOC, and confirm identical behavior with `dbl` and `caget`.
3. **Explain it.** In one sentence each: what does `configure/RELEASE` decide versus `configure/CONFIG_SITE` — and why does ALS-U keep the `*.local` overrides out of git?
 