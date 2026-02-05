# Kernel module test task

The task is descrbed in **task.pdf**

## build

The program is build with **make**

```bash
git clone https://github.com/max7im-prog/kernlel-test-task
cd ./kernlel-test-task
make all
```

## Targets

The following make targets are supplied: **all**, **clean**, **load**, **unload**


## Usage 

Upon running the **load** command, the kernel module is loaded and starts to write hello messages into **/var/tmp/test_task/output.txt**

Writing period and output file can be configured by using **setParams** executable which is created upon executing **all** target inside the **/params/** directory. Time is configured in seconds, output file is set relative to **/var/tmp/test_task/**
