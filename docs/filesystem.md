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



