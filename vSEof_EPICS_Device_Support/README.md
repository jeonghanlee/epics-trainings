# Very Simple Example of EPICS Device Support

This is the copy and modified version of the KEK EPICS training material. The original site is http://www-linac.kek.jp/cont/epics/second/

## Description

This is a very simple device support example.

This EPICS device support provides this "second" part (00 - 59) of the realtime clock. The EPICS record database provides two records, one-tenth and ten-times of the "second" value.

## More Description

* **`devAiSecond.c`** provides a device support using ai (analog input) record support. It reads the system clock and extract the "second" part. As it takes only a short time to get the information, the device support is relatively simple. If it takes relatively long time to process input/output, the later part of the process should be postponed in order not to prevent the real-time processing of other part of your ioc application. Such a device support is called an asynchronous device support. The part of the process may be performed in another thread or process, and that is called a driver support. You may write your own asynchronous device support and the corresponding driver support, or you may utilize AsynDriver.

* **`aiSecond.db`** database defines two process variables. One of them is processed every second (SCAN: 1 second) and reads the realtime clock through the devAiSecond device support. It specifies a linear conversion (LINR: LINEAR, ASLO: 0.1, etc) to have one tenth of the "second" value. It also specifies several alarm conditions (HIHI: 5.5, HHSV: MAJOR, etc), and a MAJOR alram is generated if the value is equal or above 5.5.

* **`aiSecond.dbd`** binds the record support "ai", the device support name "Second" (used in the database, aiSecond.db), and the dset structure "devAiSecond" (used in the device support, devAiSecond.c).
