# Linux Elements

* netstat

* iostat

## fsck(file system check)
用于检查和维护不一致的文件系统。若系统掉电或磁盘发生问题，可利用fsck命令对文件系统进行检查。

> UNEXPECTED INCOSISTENCY: RUN fsck MANUALLY

```
fsck -f -y /dev/sda3
```
- `-f`: 表示对没有错误的文件强制检查

- `-y`: 自动执行修复

几分钟后，按 `ctr+d`, 系统重启

## [What do 'real', 'user' and 'sys' mean in the output of time(1)?](https://stackoverflow.com/questions/556405/what-do-real-user-and-sys-mean-in-the-output-of-time1)

`Real`, `User` and `Sys` process time statistics

One of these things is not like the other. `Real` refers to actual elapsed time; `User` and `Sys` refer to CPU time used only by the process.

- `Real` is wall clock time - time from start to finish of the call. This is all elapsed time including time slices used by other processes and time the process spends blocked (for example if it is waiting for I/O to complete).

- `User` is the amount of CPU time spent in user-mode code (outside the kernel) within the process. This is only actual CPU time used in executing the process. Other processes and time the process spends blocked do not count towards this figure.

- `Sys` is the amount of CPU time spent in the kernel within the process. This means executing CPU time spent in system calls within the kernel, as opposed to library code, which is still running in user-space. Like 'user', this is only CPU time used by the process. See below for a brief description of kernel mode (also known as 'supervisor' mode) and the system call mechanism.

`User+Sys` will tell you how much actual CPU time your process used. Note that this is across all CPUs, so if the process has multiple threads (and this process is running on a computer with more than one processor) it could potentially exceed the wall clock time reported by Real (which usually occurs). Note that in the output these figures include the User and Sys time of all child processes (and their descendants) as well when they could have been collected, e.g. by wait(2) or waitpid(2), although the underlying system calls return the statistics for the process and its children separately.

> buildtime: real=1181.66 user=8304.23 sys=909.27 [ make --jobs 8 --no-print-directory -C build/relwithdebinfo ]

It match the formula: `user + sys ~= real * 8`.

```bash
TIME_COMMAND=/usr/bin/time --format "buildtime: real=%e user=%U sys=%S [ %C ]"
target:
    @ $(TIME_COMMAND) cmake ...
```

## parallel

```
$ apt show parallel

Description: build and execute command lines from standard input in parallel
 GNU Parallel is a shell tool for executing jobs in parallel using one
 or more machines. A job is typically a single command or a small
 script that has to be run for each of the lines in the input. The
 typical input is a list of files, a list of hosts, a list of users, or
 a list of tables.
 .
 If you use xargs today you will find GNU Parallel very easy to use. If
 you write loops in shell, you will find GNU Parallel may be able to
 replace most of the loops and make them run faster by running jobs in
 parallel. If you use ppss or pexec you will find GNU Parallel will
 often make the command easier to read.
 .
 GNU Parallel also makes sure output from the commands is the same
 output as you would get had you run the commands sequentially. This
 makes it possible to use output from GNU Parallel as input for other
 programs.
```
For example: 
```

ls | grep -E '\.c$' | parallel --halt now,fail=1 --will-cite "gcc $cflags -c -o '{.}.o' '{}'"


$ ls | grep -E '\.c$' |parallel "echo {.}, {}"
1299, 1299.c
129, 129.c
12, 12.c
1300, 1300.c
1301, 1301.c
```