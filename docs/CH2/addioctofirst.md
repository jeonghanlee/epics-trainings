# Expanding Your First IOC: Adding Another IOC
Building upon your first IOC, this chapter demonstrates how the ALS-U environment facilitates managing multiple, related IOC instances from a single, centralized codebase repository (identified by a unique `APPNAME`). You will learn to use different options (`-l LOCATION`, `-d device name`, `-f FOLDER`) of the `generate_ioc_structure.bash` tool to add new IOC configurations (creating new subdirectories within `iocBoot`) while reusing the core application code found in `<APPNAME>App`. The script's validation logic (like enforcing case-sensitivity for `APPNAME`) and the benefits of this shared codebase approach for maintenance and collaboration are highlighted through practical examples.

## Lesson Overview
In this lesson, you will learn how to:
* Add a new IOC application with a different `LOCATION` to the existing `APPNAME` (device name).
* Add a new IOC application with a different "application name" (a unique identifier for this new IOC instance) and `LOCATION`, while still using the same `APPNAME` (device name).
* Add a new IOC application with a different "application name", `LOCATION`, and a git clone folder name (repository name), while still using the same `APPNAME` (device name)

## Case 1: Your IOC Application name does match with the IOC repository `APPNAME`
This is the most common and preferred case. We start with the step of cloning `tools` and `mouse` from scratch.

```shell
$ git clone ssh://git@git-local.als.lbl.gov:8022/alsu/tools.git
$ git clone ssh://git@git-local.als.lbl.gov:8022/alsu/sandbox/jeonglee-mouse.git mouse   # note that we have to use `mouse` folder name here, as this will be the `APPNAME` used in the subsequent `generate_ioc_structure.bash` command.
```
Now we would like to create an IOC with `mouse` as the **APPNAME** and `park` as **LOCATION** with the same git folder or repository name `mouse`.

```shell
$ bash tools/generate_ioc_structure.bash -l park -p mouse
Your Location ---park--- was NOT defined in the predefined ALS/ALS-U locations
----> gtl ln ltb inj br bts lnrf brrf srrf arrf bl acc als cr ar01 ar02 ar03 ar04 ar05 ar06 ar07 ar08 ar09 ar10 ar11 ar12 sr01 sr02 sr03 sr04 sr05 sr06 sr07 sr08 sr09 sr10 sr11 sr12 bl01 bl02 bl03 bl04 bl05 bl06 bl07 bl08 bl09 bl10 bl11 bl12 fe01 fe02 fe03 fe04 fe05 fe06 fe07 fe08 fe09 fe10 fe11 fe12 alsu bta ats sta lab testlab
>>
>> 
>> Do you want to continue (Y/n)? Y
>> We are moving forward .

>> We are now creating a folder with >>> mouse <<<
>> If the folder is exist, we can go into mouse 
>> in the >>> /home/jeonglee/AAATemps/sandbox <<<
>> Entering into /home/jeonglee/AAATemps/sandbox/mouse
>> makeBaseApp.pl -t ioc
mouse exists, not modified.
>>> Making IOC application with IOCNAME park-mouse and IOC iocpark-mouse
>>> 
>> makeBaseApp.pl -i -t ioc -p mouse 
>> makeBaseApp.pl -i -t ioc -p park-mouse 
Using target architecture linux-x86_64 (only one available)
>>> 

>>> IOCNAME : park-mouse
>>> IOC     : iocpark-mouse
>>> iocBoot IOC path /home/jeonglee/AAATemps/sandbox/mouse/iocBoot/iocpark-mouse

Exist : .gitlab-ci.yml
Exist : .gitignore
Exist : .gitattributes
>> leaving from /home/jeonglee/AAATemps/sandbox/mouse
>> We are in /home/jeonglee/AAATemps/sandbox
```

Please enter `mouse` folder, and execute `tree` command
```shell
$ cd mouse/
mouse $  tree --charset=ascii -L 2
.
|-- configure
|   |-- CONFIG
|   |-- CONFIG_IOCSH
|   |-- CONFIG_SITE
|   |-- Makefile
|   |-- RELEASE
|   |-- RULES
|   |-- RULES_ALSU
|   |-- RULES_DIRS
|   |-- RULES.ioc
|   `-- RULES_TOP
|-- docs
|   |-- README_autosave.md
|   `-- SoftwareRequirementsSpecification.md
|-- iocBoot
|   |-- iochome-mouse
|   |-- iocpark-mouse
|   `-- Makefile
|-- Makefile
|-- mouseApp
|   |-- Db
|   |-- iocsh
|   |-- Makefile
|   `-- src
`-- README.md

10 directories, 16 files
```

Now, you can see there are two folders, `iochome-mouse` and `iocpark-mouse`, in the `iocBoot` folder.  These two folders represent your different IOC applications based on the same `mouse` EPICS IOC code repository.

## Case 2: Your IOC Application name does not match with the IOC `APPNAME`
This happens frequently when you work in the existing IOC application. We start with the step of cloning `tools` and `mouse` from scratch.

```shell
$ git clone ssh://git@git-local.als.lbl.gov:8022/alsu/tools.git
$ git clone ssh://git@git-local.als.lbl.gov:8022/alsu/sandbox/jeonglee-mouse.git mouse  # note that we have to use `mouse` folder name here, as this corresponds to the `APPNAME` we will use in the next step, even though the IOC application name will be different.
```
Now we would like to create an IOC with `woodmouse` as the IOC application name (using the `-d` option), `park` as **LOCATION**, within the same Git repository named `mouse` based on the **APPNAME** as `mouse`.

```shell
$ bash tools/generate_ioc_structure.bash -l park -p mouse -d woodmouse
Your Location ---park--- was NOT defined in the predefined ALS/ALS-U locations
----> gtl ln ltb inj br bts lnrf brrf srrf arrf bl acc als cr ar01 ar02 ar03 ar04 ar05 ar06 ar07 ar08 ar09 ar10 ar11 ar12 sr01 sr02 sr03 sr04 sr05 sr06 sr07 sr08 sr09 sr10 sr11 sr12 bl01 bl02 bl03 bl04 bl05 bl06 bl07 bl08 bl09 bl10 bl11 bl12 fe01 fe02 fe03 fe04 fe05 fe06 fe07 fe08 fe09 fe10 fe11 fe12 alsu bta ats sta lab testlab
>>
>> 
>> Do you want to continue (Y/n)? 
>> We are moving forward .

>> We are now creating a folder with >>> mouse <<<
>> If the folder is exist, we can go into mouse 
>> in the >>> /home/jeonglee/AAATemps/sandbox <<<
>> Entering into /home/jeonglee/AAATemps/sandbox/mouse
>> makeBaseApp.pl -t ioc
mouse exists, not modified.
>>> Making IOC application with IOCNAME park-woodmouse and IOC iocpark-woodmouse
>>> 
>> makeBaseApp.pl -i -t ioc -p mouse 
>> makeBaseApp.pl -i -t ioc -p park-woodmouse 
Using target architecture linux-x86_64 (only one available)
>>> 

>>> IOCNAME : park-woodmouse
>>> IOC     : iocpark-woodmouse
>>> iocBoot IOC path /home/jeonglee/AAATemps/sandbox/mouse/iocBoot/iocpark-woodmouse

Exist : .gitlab-ci.yml
Exist : .gitignore
Exist : .gitattributes
>> leaving from /home/jeonglee/AAATemps/sandbox/mouse
>> We are in /home/jeonglee/AAATemps/sandbox
```

You can see the `iocBoot/iocpark-woodmouse` folder, and we also have the same `mouseApp` folder.
```shell
$ tree --charset=ascii -L 2 mouse/
mouse/
|-- configure
|   |-- CONFIG
|   |-- CONFIG_IOCSH
|   |-- CONFIG_SITE
|   |-- Makefile
|   |-- RELEASE
|   |-- RULES
|   |-- RULES_ALSU
|   |-- RULES_DIRS
|   |-- RULES.ioc
|   `-- RULES_TOP
|-- docs
|   |-- README_autosave.md
|   `-- SoftwareRequirementsSpecification.md
|-- iocBoot
|   |-- iochome-mouse
|   |-- iocpark-mouse
|   |-- iocpark-woodmouse
|   `-- Makefile
|-- Makefile
|-- mouseApp
|   |-- Db
|   |-- iocsh
|   |-- Makefile
|   `-- src
`-- README.md

11 directories, 16 files
```
Now, we can revisit the folders `iochome-mouse`, `iocpark-mouse`, and `iocpark-woodmouse` shortly. Please check the difference among `iochome-mouse`, `iocpark-mouse`, and `iocpark-woodmouse`. You can do this with a generic Linux command-line tool, such as `diff`.

```shell
iocBoot $ diff iochome-mouse/st.cmd iocpark-mouse/st.cmd
35,36c35,36
< epicsEnvSet("IOCNAME", "home-mouse")
< epicsEnvSet("IOC", "iochome-mouse")
---
> epicsEnvSet("IOCNAME", "park-mouse")
> epicsEnvSet("IOC", "iocpark-mouse")
64c64
< #--asSetFilename("$(DB_TOP)/access_securityhome-mouse.acf")
---
> #--asSetFilename("$(DB_TOP)/access_securitypark-mouse.acf")
```

```shell
iocBoot $ diff iochome-mouse/st.cmd iocpark-woodmouse/st.cmd 
35,36c35,36
< epicsEnvSet("IOCNAME", "home-mouse")
< epicsEnvSet("IOC", "iochome-mouse")
---
> epicsEnvSet("IOCNAME", "park-woodmouse")
> epicsEnvSet("IOC", "iocpark-woodmouse")
64c64
< #--asSetFilename("$(DB_TOP)/access_securityhome-mouse.acf")
---
> #--asSetFilename("$(DB_TOP)/access_securitypark-woodmouse.acf")
```

Historically, the variables `IOC` and `IOCNAME` have been a source of confusion. Therefore, we want to define them clearly from the outset, as these variables are used extensively to identify your IOC in the production environment.

## Case 3: Your clone folder name does not match with the IOC `APPNAME` (directory)
In practice, developers may encounter situations where the name of the cloned Git repository folder differs from the IOC's `APPNAME` (device name). The recommended practice within the ALS-U EPICS environment is to ensure that the Git repository name matches the primary `APPNAME` (device name) of the IOC it contains, especially at the beginning of the development workflow. However, we also need to accommodate existing IOC applications and provide developers with a more flexible solution for their Git workflow (clone, branch, or fork).

```shell
$ git clone ssh://git@git-local.als.lbl.gov:8022/alsu/tools.git
$ git clone ssh://git@git-local.als.lbl.gov:8022/alsu/sandbox/jeonglee-mouse.git
```
Here we use `bts`, which is case-sensitive and defined in our predefined location list, as **LOCATION**.

```shell
$ bash tools/generate_ioc_structure.bash -l bts -p mouse -f jeonglee-mouse
The following ALS / ALS-U locations are defined.
----> gtl ln ltb inj br bts lnrf brrf srrf arrf bl acc als cr ar01 ar02 ar03 ar04 ar05 ar06 ar07 ar08 ar09 ar10 ar11 ar12 sr01 sr02 sr03 sr04 sr05 sr06 sr07 sr08 sr09 sr10 sr11 sr12 bl01 bl02 bl03 bl04 bl05 bl06 bl07 bl08 bl09 bl10 bl11 bl12 fe01 fe02 fe03 fe04 fe05 fe06 fe07 fe08 fe09 fe10 fe11 fe12 alsu bta ats sta lab testlab
Your Location ---bts--- was defined within the predefined list.

>> We are now creating a folder with >>> jeonglee-mouse <<<
>> If the folder is exist, we can go into jeonglee-mouse 
>> in the >>> /home/jeonglee/AAATemps/sandbox <<<
>> Entering into /home/jeonglee/AAATemps/sandbox/jeonglee-mouse
>> makeBaseApp.pl -t ioc
mouse exists, not modified.
>>> Making IOC application with IOCNAME bts-mouse and IOC iocbts-mouse
>>> 
>> makeBaseApp.pl -i -t ioc -p mouse 
>> makeBaseApp.pl -i -t ioc -p bts-mouse 
Using target architecture linux-x86_64 (only one available)
>>> 

>>> IOCNAME : bts-mouse
>>> IOC     : iocbts-mouse
>>> iocBoot IOC path /home/jeonglee/AAATemps/sandbox/jeonglee-mouse/iocBoot/iocbts-mouse

Exist : .gitlab-ci.yml
Exist : .gitignore
Exist : .gitattributes
>> leaving from /home/jeonglee/AAATemps/sandbox/jeonglee-mouse
>> We are in /home/jeonglee/AAATemps/sandbox
```

```shell
$ tree --charset=ascii -L 2 jeonglee-mouse/
jeonglee-mouse/
|-- configure
|   |-- CONFIG
|   |-- CONFIG_IOCSH
|   |-- CONFIG_SITE
|   |-- Makefile
|   |-- RELEASE
|   |-- RULES
|   |-- RULES_ALSU
|   |-- RULES_DIRS
|   |-- RULES.ioc
|   `-- RULES_TOP
|-- docs
|   |-- README_autosave.md
|   `-- SoftwareRequirementsSpecification.md
|-- iocBoot
|   |-- iocbts-mouse
|   |-- iochome-mouse
|   `-- Makefile
|-- Makefile
|-- mouseApp
|   |-- Db
|   |-- iocsh
|   |-- Makefile
|   `-- src
`-- README.md

10 directories, 16 files
```

The `-f` option allows us to specify the existing top-level folder (jeonglee-mouse in this case) where the IOC application structure should be created. This is useful when the cloned repository name does not match the desired **APPNAME** for the IOC.

## Case 4: Your clone folder name does not match with the IOC `APPNAME` (directory) and you use the wrong application name.
```shell
$ git clone ssh://git@git-local.als.lbl.gov:8022/alsu/tools.git
$ git clone ssh://git@git-local.als.lbl.gov:8022/alsu/sandbox/jeonglee-mouse.git
```

Here we use `ar05`, which is case-sensitive and defined in our predefined location list, as **LOCATION**. However, we use the wrong application name `mOuse` with incorrect casing.

```shell
$ tools/generate_ioc_structure.bash -l ar05 -p mOuse -f jeonglee-mouse
The following ALS / ALS-U locations are defined.
----> gtl ln ltb inj br bts lnrf brrf srrf arrf bl acc als cr ar01 ar02 ar03 ar04 ar05 ar06 ar07 ar08 ar09 ar10 ar11 ar12 sr01 sr02 sr03 sr04 sr05 sr06 sr07 sr08 sr09 sr10 sr11 sr12 bl01 bl02 bl03 bl04 bl05 bl06 bl07 bl08 bl09 bl10 bl11 bl12 fe01 fe02 fe03 fe04 fe05 fe06 fe07 fe08 fe09 fe10 fe11 fe12 alsu bta ats sta lab testlab
Your Location ---ar05--- was defined within the predefined list.

>> We are now creating a folder with >>> jeonglee-mouse <<<
>> If the folder is exist, we can go into jeonglee-mouse 
>> in the >>> /home/jeonglee/AAATemps/sandbox <<<
>> Entering into /home/jeonglee/AAATemps/sandbox/jeonglee-mouse

>> We detected the APPNAME is the different lower-and uppercases APPNAME.
>> APPNAME : mOuse should use the same as the existing one : mouse.
>> Please use the CASE-SENSITIVITY APPNAME to match the existing APPNAME 

Usage    : tools/generate_ioc_structure.bash [-l LOCATION] [-d DEVICE] [-p APPNAME] [-f FOLDER] <-a>

              -l : LOCATION - Standard ALS IOC location name with a strict list. Beware if you ignore the standard list!
              -p : APPNAME - Case-Sensitivity 
              -d : DEVICE - Optional device name for the IOC. If specified, IOCNAME=LOCATION-DEVICE. Otherwise, IOCNAME=LOCATION-APPNAME
              -f : FOLDER - repository, If not defined, APPNAME will be used

 bash tools/generate_ioc_structure.bash -p APPNAME -l Location -d Device
 bash tools/generate_ioc_structure.bash -p APPNAME -l Location -d Device -f Folder

```
In this case, the template generator will provide an explanation and will not proceed with the creation of a new IOC application. This is to enforce consistency in the `APPNAME` casing within the repository, aligning with the principle of keeping similar IOC codes together for better maintenance and collaboration.

## Assignments
* Compile and Run Your IOC Applications: Navigate into the top-level directory of your IOC repository (e.g., mouse or jeonglee-mouse). For each of the IOC applications you created (e.g., `iocpark-mouse`, `iocpark-woodmouse`, `iocbts-mouse`), compile the code using the make command in the top-level directory. Then, navigate into the respective iocBoot subdirectory (e.g., `iocBoot/iocpark-mouse`) and run the IOC using the `./st.cmd` command.

* Push Your Local Changes: Ensure you have added all your changes using `git add .` and committed them with a descriptive message using `git commit -m "Your commit message"`. Then, push your local changes to your sandbox repository on **GitLab**.