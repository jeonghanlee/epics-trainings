# Chapter 3: Connect an IOC to a TCP simulator

Follow the lessons in order; keep the application source for later exercises.

* [Configure StreamDevice support and two command records.](03.01.yoursecondioc.md)
* [Run a newline-delimited echo server.](03.02.iocsimulator.md)
* [Read replies and observe connection loss and recovery.](03.03.secondiocwithsim.md)

## Chapter exercises

1. Perform the simulator stop and restart exercise in lesson 3.3.
   Distinguish the last reply value from the command record's alarm.
2. In the `drvAsynIPPortConfigure` line of `st.cmd`, change `9399` to an
   unused port. Restart the IOC and inspect the connection and write
   errors. Restore `9399` and confirm a successful round trip.
3. Explain which protocol writes `Cmd-RB` and why reading an old reply
   alone does not prove that the device is connected.
