# Filesystem

- On my ubuntu server:
```
$ df -T
Filesystem                Type   1K-blocks       Used Available Use% Mounted on
tmpfs                     tmpfs    6362920       2688   6360232   1% /run
/dev/mapper/vgubuntu-root ext4  1920221472 1478959064 343646744  82% /
tmpfs                     tmpfs   31814588     343780  31470808   2% /dev/shm
tmpfs                     tmpfs       5120          4      5116   1% /run/lock
/dev/nvme0n1p1            vfat      523248       5364    517884   2% /boot/efi
tmpfs                     tmpfs    6362916        200   6362716   1% /run/user/1000
/dev/sda1                 ext4  1922658380 1766281028  58685368  97% /media/xx
```

## exFAT vs EXT4: Which Is Better for External Drives?
- Overview exFAT and EXT4
If you are using a Linux system, you may be interested in exFAT vs EXT4. In this part, I will introduce them to you, respectively.

- What Is EXT4?
EXT4 (fourth extended filesystem) is a journaling file system for Linux, developed as the successor to EXT3.
It is the default file system for many Linux distributions including Debian and Ubuntu.

This file system can support volumes with sizes in theory up to 64 zebibyte (ZiB) and single files with sizes up to 16 tebibytes (TiB) with the standard 4 KiB block size, and volumes with sizes up to 1 yobibyte (YiB) with 64 KiB clusters, though a limitation in the extent format makes 1 exbibyte (EiB) the practical limit.

- EXT4 Maximum File System Size
Block size	1KB	2KB	4KB	64KB
32-bit mode	4TB	8TB	16TB	256PB
64-bit mode	16ZB	32ZB	64ZB	1YB

- What Is exFAT?
exFAT (Extensible File Allocation Table) is a file system introduced by Microsoft in 2006 and optimized for flash memory such as USB flash drives and SD cards. It breaks through the partition size limit (32GB) and the file size limit (4GB) of FAT32. Therefore, it has been adopted by the SD Association as the default file system for SDXC cards larger than 32 GB.

The maximum volume size of the exFAT file system is 128 PB and the maximum file size is also 128 PB (the theoretical file size limit is 16 EB, but it is limited by the maximum volume size).

### exFAT vs EXT4
In terms of EXT4 vs exFAT, which is better for a USB drive? Both EXT4 and exFAT support big volume sizes (1EB for EXT4 and 128PB for exFAT) and big file sizes (16TB for EXT4 and 128PB for exFAT), which are enough for use nowadays. Therefore, I will compare the 2 file systems from the following aspects.

- Compatibility: EXT4 can only be used on Linux systems. However, exFAT is supported by Windows & macOS & Linux systems, and other devices like TVs, cameras, etc.

- Performance: exFAT is not as efficient in handling data as other file systems, such as EXT4. This can lead to slowdowns and lags when accessing or writing files.

- Security: EXT4 does not honor the “secure deletion” file attribute. Therefore, deleted sensitive data will appear in the file-system journal. However, exFAT drives are more vulnerable to unauthorized access and modification due to the lack of encryption or authentication.

- Storage: For a drive of large capacity, the default cluster size of EXT4 is 4KB while that of exFAT is 128KB. Each cluster can only be occupied by one file. Therefore, you will find that when a large number of files are stored on an EXT4 drive and an exFAT drive, the remaining space of the EXT4 drive will be more than that of the exFAT drive.

To understand the storage difference between exFAT and EXT4 better, you can refer to this post on the Stack Exchange forum.

> I have 2 hard drives, one is EXT4 and one is EXFAT. Both are 8TB, and both contain the same files. However, if I look at the amount of space being used in Gparted on Ubuntu, there is a big difference. EXT4: there is 154 GB free. EXFAT: there is 13 GB free.

From: https://superuser.com/questions/1669896/ext4-vs-exfat-storage-size-difference

Then, would you like to format the USB drive as EXT4 or exFAT? 
- If you want to use the USB drive across systems, exFAT is recommended. 

- If you use the USB drive only on a Linux system, EXT4 is recommended.


# What Is File System
File System, also known as filesystem or fs, is a method and data structure that the operating system uses to control how data is stored and retrieved. By separating the data into pieces and giving each a name, the data is easily isolated and identified. Taking its name from the way a paper-based data management system is named, each group of data is called a 'file'. The structure and logic rules used to name the groups of data and their names is called a 'file system'. - by Wikipedia
Definition of File System:

According to Wikipedia, it could be defined that the file system is a structure that the operating system applies to manage data in groups via logic rules. 

- Why Is File System Important
The file system helps the operating system to manage data and files in a logical way. With it, users can easily access, secure, read, write, and modify data on their storage devices. 

Instead, without the file system, data on a drive will be stored disorderly, taking huge storage devices and making it difficult for users to access and find desired files. The file system also makes lost files easy to be retrieved.

- What's The Relationship Between File System and Operating System
If you want to understand the relationship between the file system and operating system, it's essential to learn:

  - How the file system works
A file system indexes all the data information on a storage device, including the size of the File, attributes, location, and hierarchy in the directory. The file system also specifies the path to a file via the structure of directories with a format.


- Structure of file system metadata

All the data information of files on a storage drive is stored in the file system metadata:
- Date created
- Date modified
- Last date of access
- Last backup
- User ID of the file creator
- Access permissions
- File size

- How the operating system access file system


## 文件系统的实现原理是什么?
文件系统是操作系统中用于组织、管理和存储磁盘上数据的重要组件。其基本实现原理主要包括以下几个核心部分：

- 目录结构：文件系统采用分层的目录（或称为文件夹）结构来组织文件，允许用户通过路径名（如“/home/user/documents”）定位文件。

- inode（索引节点）：在许多文件系统中，每个文件都有一个唯一的inode，它包含了文件的元数据，如大小、创建时间、权限以及指向实际数据块的指针等信息。

- 数据存储：文件数据被分割成固定大小的数据块（或扇区），这些数据块按照inode中的指针顺序链接起来存储在磁盘上。

- 分配策略：当新文件创建或现有文件扩展时，文件系统采用一定的分配策略（如连续分配、链接分配、索引分配）来分配和管理磁盘空间。

- 一致性与缓存：为了提高性能，文件系统通常会使用缓存机制（如页缓存、缓冲区缓存）来减少对磁盘的访问。同时，它还需要维护事务日志或使用其他方法来保证在系统崩溃等情况下的数据一致性。

- 文件系统元数据：除了inode外，还有诸如超级块这样的元数据区域，它们记录了整个文件系统的状态，例如总的inode数量、未使用的inode和数据块数量、文件系统的类型等。

- 文件操作接口：操作系统通过提供API或系统调用来支持各种文件操作，如打开、读写、创建、删除、重命名等，文件系统则负责将这些高级操作转化为对底层磁盘数据的实际处理。

总的来说，文件系统是通过抽象逻辑视图（即用户看到的目录结构和文件）和物理磁盘存储之间的映射关系，配合高效的资源管理和数据访问机制，为用户提供了一种方便、灵活、可靠的数据存储服务。


## ext4, ntfs, exfat等哪种文件格式更安全？
在不同的应用场景和操作系统下，文件系统的安全性表现各有优势：

- ext4：它是Linux系统中最常用的文件系统格式，提供了日志记录功能，能够有效减少因意外中断导致的数据丢失风险。从数据安全角度来看，ext4具备相对稳定的数据校验机制，在Linux环境下较少出现数据丢失或损坏的情况。

- ntfs：是Windows NT系列操作系统默认的文件系统，支持权限管理、事务处理及磁盘配额等功能，增强了文件安全性。然而，其复杂的结构可能导致在非Windows系统中使用时存在一定的兼容性和稳定性问题。

- exFAT：由微软公司设计，主要针对闪存设备（如USB驱动器和SD卡），它跨越了不同操作系统平台（Windows、Mac OS X、Linux等）之间的文件共享障碍。exFAT在设计上没有日志文件系统那样的错误恢复能力，但比起早期的FAT32格式，它的数据完整性更高，不过它在数据安全性方面并不像NTFS那样提供完整的文件权限和加密功能。

总的来说，如果只考虑单一操作系统内部且强调数据安全和稳定性，ext4和ntfs都有各自的安全特性；而在跨平台兼容性与通用场景下，exFAT虽不如前两者在原生系统中的安全防护全面，但能满足基本的安全需求。每种文件格式的安全性还取决于用户的使用习惯和实际操作环境，比如定期备份、避免突然断电等也会直接影响到数据安全。

## 如何给 exFAT添加上 ext4的日志记录功能？

exFAT文件系统并未内建日志记录功能，它是微软设计的跨平台文件系统，主要用于闪存存储设备（如SD卡、USB驱动器），以支持大容量文件和在不同操作系统之间轻松交换数据。
而ext4是Linux系统下的文件系统，其日志记录机制（journaling）是ext系列文件系统为了提高文件系统的稳定性和可靠性而设计的核心功能之一。

由于exFAT与ext4的设计原则和结构完全不同，因此无法直接给exFAT添加ext4的日志记录功能。
每种文件系统都有其独特的数据管理方式和错误恢复机制，exFAT不具备像ext4那样的日志式文件系统特性。
若需类似日志保护的数据安全功能，可以考虑在支持exFAT的平台上使用具有日志记录功能的文件系统，比如在Linux环境中使用ext4或其他有日志功能的文件系统。

## exFat如何能在Linux上使用的？实现原理是什么？
exFAT文件系统在Linux上使用，可以通过以下几种方式实现：
安装官方或第三方驱动程序：
• 自从2019年微软公开了exFAT的部分技术规格并授权给了Linux内核社区，许多现代Linux发行版已经将exFAT支持直接集成到了内核中。用户可以直接通过更新到支持exFAT的内核版本或者安装相关软件包来获得exFAT支持。例如，在Ubuntu等发行版中可以安装 exfat-utils• 和 fuse-exfat• 包。

使用FUSE（Filesystem in Userspace）实现：
• 在较早时期，Linux支持exFAT主要是通过FUSE实现的，这是一个让用户态程序提供文件系统服务的机制。通过安装如fuse-exfat• 这样的FUSE模块，可以让Linux系统挂载exFAT格式的分区。

编译自定义驱动：
• 对于一些特定环境下的嵌入式Linux系统，可能需要手动下载源代码编译exFAT驱动，比如基于relan/exfat或dorimanx/exfat-nofuse项目提供的开源实现进行交叉编译，并按照相应指南进行安装和配置。

实现原理大致如下：
• FUSE方式实现时，exFAT驱动是一个运行在用户空间的应用程序，它接收到内核对文件系统操作的请求后，将这些请求转换为对exFAT文件系统的具体操作，然后将结果返回给内核，这样就可以透明地在Linux下访问exFAT分区。
• 内核原生支持时，exFAT驱动是作为内核模块存在的，可以直接处理与exFAT相关的磁盘I/O操作，无需借助用户空间程序，性能通常更高且集成度更好。

无论是哪种方式，核心都是为了实现在Linux操作系统下对exFAT文件系统的读写、创建、删除等基本文件系统操作功能，从而解决不同操作系统间的数据交换问题。


## 还有哪些具有日志功能的文件系统？
具有日志功能的文件系统（Journaling File Systems）主要包括以下几种：

- ext3/4：ext3是Linux内核中第一个广泛应用的日志文件系统，其继任者ext4也保留了日志记录特性，并在性能和扩展性上进行了优化。

- XFS：高性能64位日志文件系统，由SGI开发并在Linux系统中广泛使用，能够支持大容量存储且具备很高的稳定性和数据完整性。

- JFS (IBM Journaled File System)：由IBM公司开发，旨在提供一种高度可靠且可扩展的日志文件系统，被多种操作系统支持，包括AIX、Linux等。

- ReiserFS：原名Reiser3，也是一种日志文件系统，适用于Linux系统，在处理大量小文件时有优秀表现。

- Btrfs (B-tree file system)：它是一个现代的复制写入日志文件系统，设计目标是在Linux上提供高级的错误检测和恢复能力，以及先进的特性如数据校验、快照和自动伸缩。

- ZFS：尽管ZFS最初为Solaris操作系统设计，但现在也在多个其他平台可用，包括Linux。ZFS不仅是一种日志文件系统，还提供了存储池管理、数据校验、冗余等多种高级存储功能。

这些日志文件系统通过维护事务日志来确保在系统崩溃或电源故障的情况下能快速并完整地恢复文件系统的状态，从而保护用户数据免受意外损害。

## 如何实现自己的一套可以跨平台的文件系统？
实现一套跨平台的文件系统是一个相当复杂且工程量巨大的任务，需要对操作系统的工作原理有深入理解，并考虑到不同平台上硬件架构、内核接口和用户需求的差异。以下是一些关键步骤和考虑点：

- 设计抽象层：
• 为了实现跨平台，首先需要设计一个能够与不同操作系统底层文件系统API进行交互的抽象层。该层将提供一组通用的接口定义，屏蔽各操作系统的差异性。

- 兼容多种文件系统结构：
• 跨平台文件系统可能不直接取代宿主机上的原生文件系统，而是构建在现有文件系统之上，所以要确保对各种已有文件系统（如NTFS、ext4、APFS等）的良好适应性。

- 遵循标准或创建中间格式：
• 可以选择遵循已有的跨平台标准，如POSIX规范，或者开发一种中间数据格式，使得不同平台上的程序都能理解和操作这种格式的数据。

- 实现核心功能：
• 实现文件元数据管理（如inode、目录结构等）、文件内容存储、缓存机制、日志记录（可选，用于提高稳定性）等功能。

- 容错及恢复能力：
• 对于分布式或网络文件系统来说，还需要设计复制、冗余和故障切换机制以保证高可用性和数据一致性。

- 性能优化：
• 文件系统的性能对于用户体验至关重要，因此要在设计时就考虑到如何优化读写操作、目录搜索以及I/O调度等问题。

- 编译和移植：
• 代码需要以平台无关的方式编写，通常使用C/C++这样的低级语言，并针对目标平台进行编译和适配。

- 测试验证：
• 在多种不同的操作系统环境下进行全面的功能测试和性能测试，确保文件系统在不同环境下的表现符合预期。

- 遵循许可证要求：
• 开发过程中需要注意版权和许可证问题，确保新文件系统可以合法地在各个支持的平台上分发和使用。

- 维护和更新：
• 随着时间推移和技术发展，跨平台文件系统需要不断迭代升级以适应新的硬件特性和软件环境变化。

实际上，已有许多开源或商业的跨平台文件系统方案，例如FUSE（Filesystem in Userspace）允许开发者在用户态编写文件系统模块，从而实现跨平台；还有像Samba这样的服务，可以在非Windows系统上模拟出CIFS（Common Internet File System）共享，实现在不同操作系统之间共享文件资源。

## Default cluster size for NTFS, FAT, and exFAT
- https://support.microsoft.com/en-us/topic/default-cluster-size-for-ntfs-fat-and-exfat-9772e6f1-e31a-00d7-e18f-73169155af95

All file systems that are used by Windows organize your hard disk based on `cluster size` (also known as allocation unit size). `Cluster size` represents the __smallest__ amount of disk space that can be used to hold a file. When file sizes do not come out to an even multiple of the `cluster size`, additional space must be used to hold the file (up to the next multiple of the `cluster size`). On the typical hard disk partition, the average amount of space that is lost in this manner can be calculated by using the equation (`cluster size`)/2 * (number of files).

If no `cluster size` is specified when you format a partition, defaults are selected based on the size of the partition.

- Default cluster sizes for exFAT
The following table describes the default cluster sizes for exFAT.

| Volume size  | Default cluster size  |
|--------------|---------------|
| 7 MB–256 MB  | 4 KB  |
| 256 MB–32 GB | 32 KB  |
| 32 GB–256 TB | 128 KB  |
| > 256 TB     | NOT Support  |

就是说，当我使用的disk 分区大于 32GB时，文件的最小分配单元就是128KB， 比如空文件(至少有几个字符)，空目录. 但它不支持link.
```
$ ln -s empty empty.1
ln: failed to create symbolic link 'empty.1': Operation not permitted
```
- Default cluster sizes for FAT32
| Volume size  | Default cluster size  |
|--------------|---------------|
| 8GB–16GB     | 8 KB  |
| 16GB–32GB    | 16 KB  |
| 32GB–2TB     | 32 KB  |


# cmd
- list disk
```
sudo fdisk -l
Disk /dev/sda: 1.82 TiB, 2000398934016 bytes, 3907029168 sectors
Disk model: Portable SSD T5
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 33553920 bytes
Disklabel type: gpt
Disk identifier: 5C7C2069-F7CE-4FEA-A7F0-E67FD0658C69

Device          Start        End    Sectors   Size Type
/dev/sda1       65535 1953205139 1953139605 931.3G Linux filesystem
/dev/sda2  1953205140 2929807709  976602570 465.7G Microsoft basic data
/dev/sda3  2929807710 3907000094  977192385   466G Linux filesystem
GPT PMBR size mismatch (4294967294 != 7814037167) will be corrected by write.


Disk /dev/sdb: 3.64 TiB, 4000787030016 bytes, 7814037168 sectors
Disk model: PSSD T9
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 2097152 bytes
Disklabel type: gpt
Disk identifier: 4302C1F3-E8B6-4C00-AD1A-53F7673899ED

Device      Start        End    Sectors  Size Type
/dev/sdb1      40     409639     409600  200M EFI System
/dev/sdb2  411648 7814035455 7813623808  3.6T Microsoft basic data
```
