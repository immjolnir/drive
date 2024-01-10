# Ubuntu

## Fix the Read only disk issu
- 使用 “磁盘” 程序
  - 点击左下方齿轮状的按钮，并单击运行“check filesystem”和“Repair filesystem”命令
  - 重新挂载硬盘后你会发现，锁的图标不见了，修复成功！


## mv - move (rename) files

- `$ mv -i melodic-desktop-full.rosinstall melodic-desktop-full.rosinstall.old`
```
       -i, --interactive
              prompt before overwrite
```

## Ubuntu disk图形化界面上 Create Patition Image是什么意思？
在Ubuntu的磁盘管理图形化界面上，“Create Partition Image”是一个功能选项，它允许用户创建选定分区的一个完整镜像备份。选择这个选项后，系统会将所选分区的所有数据按照当时的状态打包成一个映像文件（通常为.img格式），该映像文件包含分区的结构、文件系统以及所有数据内容。日后如果需要恢复分区至当前状态或者复制到另一个硬盘上时，可以使用这个映像文件进行操作。通过创建分区镜像，用户可以方便地备份和迁移他们的系统分区或数据分区而不必逐个文件手动处理。

### 它生成的映像文件会更小吗？
生成的映像文件不一定比源分区的实际大小要小，通常情况下，“Create Partition Image”功能会创建一个与源分区同样大小或接近大小的映像文件。如果分区中存在大量未使用的空间，那么生成的映像文件在使用特定类型的压缩算法时可能会比实际分区大小小一些。

有些磁盘备份工具支持在创建映像时进行压缩，以减少映像文件占用的空间，但这通常会增加备份过程所需的时间。如果不启用压缩功能，映像文件将反映分区上所有已分配空间的内容，即使这些空间内没有实际数据也会占用相应大小。


## find the Linux kernel version?

### 1st Way
```
$ uname -r
5.15.0-52-generic
```
But what do the other digits mean here? Let me explain:

- 5 – Kernel version
- 15 – Major revision
- 0 – Minor revision
- 52 – Bug fix
- generic – Distribution-specific string. For Ubuntu, it means I’m using the desktop version. For Ubuntu server edition, it would be ‘server’.

### 2nd Way
```
$ uname -a
Linux 006IT00075 5.15.0-52-generic #58-Ubuntu SMP Thu Oct 13 08:03:55 UTC 2022 x86_64 x86_64 x86_64 GNU/Linux
```
Let me explain the output and what it means:

- `Linux` – Kernel name. If you run the same command on BSD or macOS, the result will be different.

- `006IT00075` – Hostname.

- `5.15.0-52-generic` – Kernel release (as we saw above).

- `#58-Ubuntu SMP Thu Oct 13 08:03:55 UTC 2022` – This means that Ubuntu has compiled 5.15.0-52-generic 58 times. A timestamp for the last compilation is also there.

- x86_64 – Machine architecture.

- x86_64 – Processor architecture.

- x86_64 – Operating system architecture (you can run a 32-bit OS on a 64-bit processor).

- GNU/Linux – Operating system (and no, it won’t show the distribution name).

### 3rd Way
```
$ cat /proc/version

Linux version 5.15.0-52-generic (buildd@lcy02-amd64-032) (gcc (Ubuntu 11.2.0-19ubuntu1) 11.2.0, GNU ld (GNU Binutils for Ubuntu) 2.38) #58-Ubuntu SMP Thu Oct 13 08:03:55 UTC 2022
```

### 4th Way
```
$ sudo dmesg | grep Linux
[    0.000000] Linux version 5.15.0-52-generic (buildd@lcy02-amd64-032) (gcc (Ubuntu 11.2.0-19ubuntu1) 11.2.0, GNU ld (GNU Binutils for Ubuntu) 2.38) #58-Ubuntu SMP Thu Oct 13 08:03:55 UTC 2022 (Ubuntu 5.15.0-52.58-generic 5.15.60)
[    0.336129] ACPI: Added _OSI(Linux-Dell-Video)
[    0.336129] ACPI: Added _OSI(Linux-Lenovo-NV-HDMI-Audio)
[    0.336129] ACPI: Added _OSI(Linux-HPI-Hybrid-Graphics)
[    0.526653] pps_core: LinuxPPS API ver. 1 registered
[    0.641238] Linux agpgart interface v0.103
```