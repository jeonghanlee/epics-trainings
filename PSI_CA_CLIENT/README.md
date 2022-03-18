# Lessons on Channel Access Client Programming

This is the copy and modified version of the PSI Training Lessons. The original site is http://epics.web.psi.ch/training/caClientLessons/

In order to integrate a latest EPICS building system, old source codes are only used. I created a generic caClient example as following command.

```bash
makeBaseApp.pl -t caClient caLessons
```

Then copy all `calessonX.c` file into `App`, and modify its Makefile to use as `PROD_HOST`.



The original short introduction of each caLesson as follows:

## caLesson1

This lesson shows how to write a very simple CA client program. It connects to some channels, reads them, prints them and exits.

## caLesson2

Here you will learn how to read more than a bare number from a channel. This is very useful to format a value correctly or to know the physical units of a value. In fact, it is this additional information that makes the difference between a value and a bare number.

You will also see one way to get your data a bit more structured. There are many ways to achive the same result, of course. You should know how macros work in C to understand this lesson.

## caLesson3

In this lesson, you will learn about different data types in Channel Access. You will also see the difference between "static" and "dynamic" data of a channel and how use this to reduce network traffic when reading data repeatedly.


## caLesson4

This lesson introduces "monitors". When you install a monitor, a user-defined callback function is called by the Channel Access library whenever a channel has new values available. This is much more "network friendly" than high rate polling.

You will also see the differences beween the Channel Access APIs of EPICS R3.13 and R3.14. In R3.14, you can still use the R3.13 API and Channel Access clients written for EPICS R3.13 should work without modifications with EPICS R3.14. Anyway, to be able to understand other people's code, you should know both flavours and only R3.14 is designed for multi-threading.

## caLesson5

 In this lesson we will write to a device for the first time. We will use a "put-wait" method, that does not return before the device has understood and executed the written value. This method is best suited for GUI-less programs which can block. (In later lessons, we will learn a different way to write values.)

After the value is written, we will wait unil the device has finished using a monitor on a done flag. This is much more efficient than polling a done flag in a loop.
