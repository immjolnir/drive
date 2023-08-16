# ps aux输出信息详解

`ps -aux` 是以BSD方式显示 

- a 显示所有用户的进程(show processes for all users) 

- u 显示用户(display the process's user/owner) 

- x 显示无控制终端的进程(also show processes not attached to a terminal)

```bash
$ ps aux
USER         PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root           1  0.0  0.0  22196  5984 pts/0    Ss+  Aug04   0:01 /bin/bash
root         877  0.0  0.0  22200  6068 pts/1    Ss   Aug04   0:01 bash
root        9041  0.0  0.0  22196  5892 pts/2    Ss+  10:34   0:00 bash
root       17574  0.0  0.0  36028  3220 pts/1    R+   11:06   0:00 ps aux
```

我们对每一列所代表的信息进一步的解释：

- USERE 进程的拥有者

- PID 进程的ID

- %CPU 占用的CPU使用率, cpu utilization of the process in "##.#" format.  Currently, it is the CPU time used divided by the time the process has been running (cputime/realtime ratio), expressed as a percentage.  It will not add up to 100% unless you are lucky.


- %MEM 占用的内存使用率, ratio of the process's resident set size to the physical memory on the machine, expressed as a percentage.

- %VSZ Virtual Memory Size。占用虚拟内存大小。该进程使用的虚拟内存量（KB）

- %RSS Resident Set Size。常驻内存集合大小，表示该进程分配的内存大小。该进程占用的固定内存量（KB）

- TYY 终端的次要装置号（minor device number of tty）,该进程在哪个终端上运行（登陆者的终端位置），若与终端无关，则显示（？）。若为pts/0等，则表示由网络连接主机进程；

- STAT 进程的状态，其有以下几种状态表示：
  - D: 无法中断的休眠状态 (通常 IO 的进程)
  - R: 正在执行中
  - S: 睡眠中，可中断
  - T: 暂停执行
  - Z: 已退出，成为僵尸进程
  - W: 没有足够的记忆体分页可分配
  - X 死掉的进程  （基本很少见）
  - <: 高优先序的行程
  - N: 低优先序的行程
  - L: 有记忆体分页分配并锁在记忆体内 (实时系统或I/O),有些页被锁进内存
  - s 进程的领导者（在它之下有子进程）
  - l 多进程的（使用 CLONE_THREAD, 类似 NPTL pthreads）
  - + 位于后台的进程组

- START: 行程开始时间

- TIME: 执行的时间, 该进程实际使用CPU运行的时间

- COMMAND: 所执行的指令


## ps -ef
`ps -ef` 是以System V方式显示，该种方式比BSD方式显示的多 

- e 显示所有用户的进程(all processes), 此参数的效果和指定"a"参数相同 

- f 用ASCII字符显示树状结构，表达程序间的相互关系(ASCII art forest)

