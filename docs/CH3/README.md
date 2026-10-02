# Chapter 3: Second EPICS IOC and Device Simulation

This chapter builds on the previous examples by guiding you through configuring a second EPICS IOC. A key focus is simulating device communication – specifically using a TCP-based simulator to mimic interactions often performed over serial interfaces. You will learn to set up this simulation and test the communication between the IOC and the simulator.

This chapter covers the following topics:

* [Configure the Second IOC](03.01.yoursecondioc.md): Setting up and configuring a second IOC application, potentially introducing device support relevant for external communication.
* [Create the TCP Simulator](03.02.iocsimulator.md): Developing a simple TCP server application to simulate responses from a hardware device (like one communicating over serial).
* [Test IOC-Simulator Communication](03.03.secondiocwithsim.md): Testing the interaction between your second IOC and the device simulator.

## Exercise

Do these on your own, without re-reading the sections above.

1. **Break it, then fix it.** With the IOC running, stop the simulator (`Ctrl+C` in Terminal 1) and `caput` a new string to `jeonglee:myoffice:Cmd`. Check `caget jeonglee:myoffice:Cmd-RB` — what do you see, and why? Restart the simulator and confirm the next `caput` round-trips again. If it does not recover on its own, restart the IOC and note what changed.
2. **Change one thing.** Edit `st.cmd` to point `TARGET_PORT` at a port with no listener, restart the IOC, and read the Asyn connection messages during startup. Restore `9399` and confirm the simulator's terminal shows the new connection.
3. **Explain it.** In one sentence each: what does the `OUT` field's `@training.proto sendRawQuery(...) $(PORT)` do, and where does the echoed reply end up?
