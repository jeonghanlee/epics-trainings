# Chapter 5: Inspect IOC application configuration

Follow the lessons in order; keep the application source for later exercises.

* [Compare function-style and space-separated commands.](05.01.how-to-ioc-log1.md)
* [Select dependency paths with RELEASE and local overrides.](05.02.how-to-ioc-log2.md)
* [Change installation and IOC-visible paths.](05.03.how-to-ioc-log3.md)
* [Run an operating-system command from the IOC shell.](05.04.how-to-ioc-log4.md)

## Chapter exercises

1. Save any existing `configure/RELEASE.local`. Set `EPICS_BASE` to
   `/nonexistent/epics/base` in that file and run `make`. Restore the
   saved content, or set it to the active Base path, and rebuild.
2. Complete the command-style comparison in lesson 5.1 and the
   installation-path exercise in lesson 5.3.
3. Explain the difference between selecting dependencies in `RELEASE`
   and selecting build options in `CONFIG_SITE`.

## Before moving on

- [ ] `configure/RELEASE.local` and `configure/CONFIG_SITE.local` in `jeonglee_Demo/` are restored so `make` succeeds.
- [ ] `iocBoot/ioctraining-jeonglee_Demo/envPaths` sets `TOP` to your `jeonglee_Demo` directory, with `db/` and `iocsh/` intact for Chapter 6.
- [ ] Every `jeonglee_Demo` IOC, `softIoc`, and `tc32_emulator.bash` process is stopped.
- [ ] `ss -ltn 'sport = :9399'` reports no active TCP listener.
