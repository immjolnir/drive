Linux Commands

# What is the best way to know if a .gz file has been corrupted or not?
The best way to determine if a .gz file has been corrupted is to use a tool to check the file's integrity. One common tool for this is the gzip command-line utility, which can be used to check the integrity of a .gz file using the following command:

gzip -t [filename.gz] 

This command will perform a test on the compressed file and will return an error message if the file is corrupted. Another way to check if a .gz file is corrupted is to decompress it and compare the decompressed file with the original one. If the decompressed file is identical to the original one, the archive is not corrupted. You can use the command :

gunzip [filename.gz]

If the command execute successfully and the file is identical to the original one, then the archive is not corrupted.

Another way is to use an online file verification tool like MD5 or SHA1 to create a hash of the file before compressing it and then compare the hash of the compressed file to the original one, if they match the file is not corrupted.

It's worth noting that if the file is corrupted, it's possible that it can't be repaired, in that case, you should try to use another copy of the file.


# How ping Works in Linux
The Linux ping command is a simple utility used to check whether a network is available and if a host is reachable. With this command, you can test if a server is up and running. It also helps with troubleshooting various connectivity issues.

The ping command allows you to:
- Test your internet connection.
- Check if a remote machine is online.
- Analyze if there are network issues, such as dropped packages or high latency.

When you try to “ping” a remote host, your machine starts sending __ICMP__ echo requests and waits for a response. If the connection is established, you receive an echo reply for every request.

The output for the ping command contains the amount of time it takes for every packet to reach its destination and return.

The terminal keeps printing the responses until interrupted. After every session, there are a few lines with ping statistics.
```
ping -c 3 ip
```

- ping “localhost” to Check Local Network
If you encounter issues reaching a website or a remote machine, you can ping localhost to confirm you have a network connection. Use one of the three ways to check the local network interface:

  - ping 0 – This is the quickest way to ping localhost. Once you type this command, the terminal resolves the IP address and provides a response.
  - ping localhost – You can use the name to ping localhost. The name refers to your computer, and when we use this command, we say: “ping this computer.”
  - ping 127.0.0.1 – Some people prefer using the IP address 127.0.0.1 to ping localhost.

- Specify the Internet Protocol

To request IPv6 or IPv4 address, add -6 or -4 after the ping command and before a hostname/IP.

  - ping -6 hostname/IPv6
  - ping -4 hostname/IPv4

- Change Ping Packet Size
In some scenarios, you may want to use -s to increase the packet size from the default value of 56 (84) bytes. The number in parenthesis represents the ping bytes sent including 28 bytes of the header packet.

For example, to increase the packet size to 1000 bytes:

ping -s 1000 google.com

- Change Time Interval Between Ping Packets
The default interval between each ping request is set to one second. You can increase or decrease that time using the -i switch. To decrease the ping interval, use values lower than 1.

ping -i 0.5 google.com

To increase the ping interval, enter any value higher than 1.

- Flood a Network Using ping to Test Performance
You can use ping flood to test your network performance under heavy load.

Ping flood -f option requires root to execute. Otherwise, apply sudo to your ping command to flood a host. This command sends a large number of packets as soon as possible.

sudo ping -f hostname-IP

- Limit Number of Ping Packets
To make the ping command automatically stop after it sends a certain number of packets, use -c and a number. This sets the desired amount of ping requests, as shown in this example:

ping -c 2 google.com

- Suppress Ping Output to Print only Summary Statistics
If you do not want to clog your screen with information for every packet, use the -q switch to display the summary only.

The -q option prints one line with the regular ping information and then provides the statistics at the end. The letter “q” in this command stands for “quiet” output.

ping -c 10 -q google.com

- Add Timestamp Before Each Line in ping Output
If you want to note the time of day when you execute the ping command, add the -D option.

ping -D google.com

This prints a timestamp in UNIX format before each line.

- Get an Audible Ping When a Host is Reachable
When you use the -a switch, the system plays a sound when there is a response from a host.

An audible ping is useful when you are troubleshooting network issues and do not want to look at the screen until there is a response.

ping -a google.com

The output looks the same as a regular ping command output.

- Show Ping Version and Exit
At any time, you can check the version of the ping tool on your system.

Append -V to the ping command to display the version of this utility.

ping -V google.com
ping from iputils 20211215

# date
```bash
#!/bin/bash

dt=$(date '+%d/%m/%Y %H:%M:%S');
echo "$dt"
```

- if you need milliseconds, use `date '+%F %T.%3N'`.
- if you need microseconds, use `date '+%F %T.%6N'`.
- if you need nanoseconds, use `date '+%F %T.%N'`. 

`%F` is for date, it gives `YYYY-MM-DD`, you can use `%D` instead to get `MM-DD-YYYY`, `%T` is for time, `HH:MM:SS`.


# find

## Find empty file/directory

- Find all empty files: `find  /path/to/dest -type f -empty`

- Find all empty directories: `find  /path/to/dest -type d -empty`

- Find Empty File And Delete Them: `find  /path/to/dest -type f -empty -delete`

- Find Empty File Owned By A User Called vivek: 
  - `find  /path/to/dest -type f -empty -user vivek`
  - `find  /path/to/dest -type f -empty -user vivek -delete`

```
/usr/bin/ld.gold: error: CMakeFiles/drive_benchmark_messages.dir/src/tsv_data.cpp.o: file is empty
collect2: error: ld returned 1 exit status
```
Finally, 
```
find  . -name *.o -type f -empty  -exec rm {} \;
```

## find查找指定时间内修改过的文件或目录

find命令的格式
```
find pathname -options [ -print -exec -ok ...]
```

* `-ok`: 它的作用和 `-exec` 一样，只是需要用户交互，更安全
  - 

Options:

- `-mtime`: 天单位， `-1`: 表示1天之内修改过的; `+1` or `1` 表示1天之前修改过的

- `-mmin`: 分钟单位, `-5`: 即表示五分钟内修改过的; `+5` or `5`就是五分钟之前修改过的


例子:

- 查找5分钟内的文件，并ls它
  - `find . -type f -mmin -5 -exec ls {} \;`

- 在目录中查找更改时间在n日以前的文件并删除它们
  - `find . -type f -mtime +14 -exec rm {} \;`

- 查找5分钟内的文件，并移动到 `/tmp` 目录下
  - `find . -type f -mmin -5 -exec mv {} /tmp \;`

- 在当前目录中查找以.log结尾、更改时间在5日以上的文件，并删除，但在删除前给出提示
  - `find . -name “*.log” -mtime +5 -ok rm {} \;`

- 查找系统当中所有长度为0的普通文件，并列出它们的完整路径
  - `find / -type f -size 0 -exec ls -l {} \;`


另外：find中搭配atime、ctime和mtime的使用方法介绍如下：

- `atime`：访问时间（accesstime），指的是文件最后被读取的时间，可以使用touch命令更改为当前时间；

- `ctime`：变更时间（changetime），指的是文件本身最后被变更的时间，变更动作可以使chmod、chgrp、mv等等；

- `mtime`：修改时间（modifytime），指的是文件内容最后被修改的时间，修改动作可以使echo重定向、vi等等；

## Permission

```
$ find . -perm 775 -a -type f -exec rm {} \;
```

# What is a Package Manager in Linux? How Does it Work?

```
  echo "  use latest boost"; \
  add-apt-repository ppa:mhier/libboost-latest; \
  apt-get update && apt-get install -y --no-install-recommends libboost1.81-all-dev; \
  add-apt-repository --remove ppa:mhier/libboost-latest; \
```

- https://askubuntu.com/questions/307/how-can-ppas-be-removed

```
Usage: apt-add-repository <sourceline>

apt-add-repository is a script for adding apt sources.list entries.
It can be used to add any repository and also provides a shorthand
syntax for adding a Launchpad PPA (Personal Package Archive)
repository.

<sourceline> - The apt repository source line to add. This is one of:
  a complete apt line in quotes,
  a repo url and areas in quotes (areas defaults to 'main')
  a PPA shortcut.
  a distro component

  Examples:
    apt-add-repository 'deb http://myserver/path/to/repo stable myrepo'
    apt-add-repository 'http://myserver/path/to/repo myrepo'
    apt-add-repository 'https://packages.medibuntu.org free non-free'
    apt-add-repository http://extras.ubuntu.com/ubuntu
    apt-add-repository ppa:user/repository
    apt-add-repository ppa:user/distro/repository
    apt-add-repository multiverse
```

# dpkg : how to ignore error in post/pre scripts
- https://stackoverflow.com/questions/9311627/dpkg-how-to-ignore-error-in-post-pre-scripts
```
$ sudo dpkg -i --force-all your.deb
(Reading database ... 225821 files and directories currently installed.)
Unpacking protobuf (1.0.999999) over (1.0.593) ...
dpkg: warning: overriding problem because --force enabled:
dpkg: warning: trying to overwrite '/opt/test/lib/python/common/__init__.py', which is also in package test-drive 1.0.12284
dpkg: warning: overriding problem because --force enabled:
dpkg: warning: trying to overwrite '/opt/test/lib/python/control/__init__.py', which is also in package test-drive 1.0.12284
dpkg: warning: overriding problem because --force enabled:
```

# Linux中的重定向，大于号＞，小于号＜

```
$ run.sh > log.txt 2>&1

$ cat < log.txt
```

- `>`: Write log.txt as stdout

- `<`: Read the file log.txt as stdin


For instance:
```
$ strace cat < a.txt
execve("/usr/bin/cat", ["cat"], 0x7ffddc900bf0 /* 82 vars */) = 0
brk(NULL)                               = 0x557a668b5000
arch_prctl(0x3001 /* ARCH_??? */, 0x7fffd1176520) = -1 EINVAL (Invalid argument)
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f3aac6e0000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=118513, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 118513, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f3aac6c3000

...

newfstatat(1, "", {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0x5), ...}, AT_EMPTY_PATH) = 0
newfstatat(0, "", {st_mode=S_IFREG|0664, st_size=6, ...}, AT_EMPTY_PATH) = 0
fadvise64(0, 0, 0, POSIX_FADV_SEQUENTIAL) = 0
mmap(NULL, 139264, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f3aabbfe000
read(0, "A\nB\nC\n", 131072)            = 6 # 这里就是从 stdin(0) 中读取
write(1, "A\nB\nC\n", 6A                    # 这里是往 stdout(1) 中写入
B
C
)                = 6
read(0, "", 131072)                     = 0
munmap(0x7f3aabbfe000, 139264)          = 0
close(0)                                = 0
close(1)                                = 0
close(2)                                = 0
```
- find out the system calls that are being made using `strace`
- source code to linux's `cat` command: https://github.com/coreutils/coreutils/blob/master/src/cat.c#L213


