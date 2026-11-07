# Chapter 2: Generate and expand IOC applications

Follow the lessons in order; keep the application source for later exercises.

* [Generate, build, and start a compiled IOC application.](02.01.yourfirstioc.md)
* [Add boot instances and separate folder and application names.](02.02.addioctofirst.md)

## Chapter exercises

1. From `mouse/`, run `make clean uninstall`. Enable `ASYN` in
   `configure/RELEASE` with the path `$(MODULES)/asyn_missing`, then run
   `make`. Read the missing dependency error. Restore the original file
   and rebuild successfully.
2. Add and run the `lab` instance described in lesson 2.2.
3. Compare the generated startup files with `diff` as shown in lesson
   2.2. Explain which generator option accounts for each changed line.
