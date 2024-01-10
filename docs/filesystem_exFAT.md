# extFAT

## How to Format a USB Disk as exFAT on Linux?
For a long time, __FAT__ has been the default choice of filesystem for formatting disks. It is compatible with pretty much all the major operating systems.

The one major problem with the __FAT__ filesystem is that you cannot transfer a file larger than 4 GB. This means even if your USB disk has 32 GB of free space, if you try to transfer an ISO image or some other file greater than 4 GB in size, the transfer will fail.

To overcome the limitations of _FAT_ filesystem, Microsoft came up with __exFAT__ filesystem.
In this tutorial, I’ll show you how to format a USB disk in __exFAT__ filesystem.

### Prerequisites
Starting Linux kernel 5.4, exFAT filesystem support is enabled in the Linux kernel itself.

For Ubuntu 22.04 and higher, you should use this command instead:
```
sudo apt install exfat-fuse exfatprogs

$ dpkg -L exfat-fuse
/.
/sbin
/sbin/mount.exfat-fuse
/usr
/usr/share
/usr/share/doc
/usr/share/doc/exfat-fuse
/usr/share/doc/exfat-fuse/NEWS.Debian.gz
/usr/share/doc/exfat-fuse/changelog.Debian.gz
/usr/share/doc/exfat-fuse/copyright
/usr/share/man
/usr/share/man/man8
/usr/share/man/man8/mount.exfat-fuse.8.gz

$ dpkg -L exfatprogs
/.
/usr
/usr/sbin
/usr/sbin/dump.exfat
/usr/sbin/exfatlabel
/usr/sbin/fsck.exfat
/usr/sbin/mkfs.exfat
/usr/sbin/tune.exfat
/usr/share
/usr/share/doc
/usr/share/doc/exfatprogs
/usr/share/doc/exfatprogs/changelog.Debian.gz
/usr/share/doc/exfatprogs/copyright
/usr/share/man
/usr/share/man/man8
/usr/share/man/man8/dump.exfat.8.gz
/usr/share/man/man8/exfatlabel.8.gz
/usr/share/man/man8/fsck.exfat.8.gz
/usr/share/man/man8/mkfs.exfat.8.gz
/usr/share/man/man8/tune.exfat.8.gz
```

### 图像化界面
- GNOME Disks, Format Partitions, 
- To use exFat, Choose other
```
$ sudo fdisk -l

Disk /dev/sda: 1.82 TiB, 2000398934016 bytes, 3907029168 sectors
Disk model: Portable SSD T5 
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 33553920 bytes
Disklabel type: gpt
Disk identifier: 5C7C2069-F7CE-4FEA-A7F0-E67FD0658C69

Device     Start        End    Sectors  Size Type
/dev/sda1  65535 3907000094 3906934560  1.8T Microsoft basic data
```

### Command line
- Usefull command
```
$ /usr/sbin/mkfs.exfat

$ mount.exfat-fuse
FUSE exfat 1.3.0+git20220115
Usage: mount.exfat-fuse [-d] [-o options] [-V] <device> <dir>

$ sudo tune.exfat -v /dev/sda
exfatprogs version : 1.1.3
[exfat_get_blk_dev_info: 202] Block device name : /dev/sda
[exfat_get_blk_dev_info: 203] Block device offset : 0
[exfat_get_blk_dev_info: 204] Block device size : 2000398934016
[exfat_get_blk_dev_info: 205] Block sector size : 512
[exfat_get_blk_dev_info: 206] Number of the sectors : 3907029168
[exfat_get_blk_dev_info: 208] Number of the clusters : 15261832

$ mkfs.exfat
exfatprogs version : 1.1.3
Usage: mkfs.exfat
	-L | --volume-label=label                              Set volume label
	-c | --cluster-size=size(or suffixed by 'K' or 'M')    Specify cluster size
	-b | --boundary-align=size(or suffixed by 'K' or 'M')  Specify boundary alignment
	     --pack-bitmap                                     Move bitmap into FAT segment
	-f | --full-format                                     Full format
	-V | --version                                         Show version
	-v | --verbose                                         Print debug
	-h | --help                                            Show help
```
Once you have identified your USB disk, format it as exfat using the command below.

```
$ sudo mkfs.exfat -n bj04 /dev/sda
exfatprogs version : 1.1.3
Creating exFAT filesystem(/dev/sda, cluster size=131072)

Writing volume boot record: done
Writing backup volume boot record: done
Fat table creation: done
Allocation bitmap creation: done
Upcase table creation: done
Writing root directory entry: done
Synchronizing...

exFAT format complete!

$ sudo tune.exfat -v /dev/sda
exfatprogs version : 1.1.3
[exfat_get_blk_dev_info: 202] Block device name : /dev/sda
[exfat_get_blk_dev_info: 203] Block device offset : 0
[exfat_get_blk_dev_info: 204] Block device size : 2000398934016
[exfat_get_blk_dev_info: 205] Block sector size : 512
[exfat_get_blk_dev_info: 206] Number of the sectors : 3907029168
[exfat_get_blk_dev_info: 208] Number of the clusters : 15261832

$ mkdir empty-dir
$ du -sh empty-dir/
128K	empty-dir/

$ echo "abc" > empty
$ du -sh empty
128K	empty
```
- Optionally, run fsck check to make sure the formatting has been done properly.
```
$ sudo fsck.exfat /dev/sda 
exfatprogs version : 1.1.3
/dev/sda: clean. directories 2, files 1
```

- Tune the exFAT
```
$ sudo mkfs.exfat -L bj04  -c 4096 -v -b 16M /dev/sda
exfatprogs version : 1.1.3
[exfat_get_blk_dev_info: 202] Block device name : /dev/sda
[exfat_get_blk_dev_info: 203] Block device offset : 0
[exfat_get_blk_dev_info: 204] Block device size : 2000398934016
[exfat_get_blk_dev_info: 205] Block sector size : 512
[exfat_get_blk_dev_info: 206] Number of the sectors : 3907029168
[exfat_get_blk_dev_info: 208] Number of the clusters : 488378646
[exfat_zero_out_disk: 523] zero out written size : 2040713216, disk size : 2000398934016
Creating exFAT filesystem(/dev/sda, cluster size=4096)

Writing volume boot record: [exfat_setup_boot_sector:  81] Volume Offset(sectors) : 0
[exfat_setup_boot_sector:  83] Volume Length(sectors) : 3907029168
[exfat_setup_boot_sector:  85] FAT Offset(sector offset) : 32768
[exfat_setup_boot_sector:  87] FAT Length(sectors) : 3815464
[exfat_setup_boot_sector:  89] Cluster Heap Offset (sector offset) : 3866624
[exfat_setup_boot_sector:  91] Cluster Count : 487895318
[exfat_setup_boot_sector:  93] Root Cluster (cluster offset) : 14894
[exfat_setup_boot_sector:  95] Volume Serial : 0x779fe381
[exfat_setup_boot_sector:  96] Sector Size Bits : 9
[exfat_setup_boot_sector:  98] Sector per Cluster bits : 3
done
Writing backup volume boot record: [exfat_setup_boot_sector:  81] Volume Offset(sectors) : 0
[exfat_setup_boot_sector:  83] Volume Length(sectors) : 3907029168
[exfat_setup_boot_sector:  85] FAT Offset(sector offset) : 32768
[exfat_setup_boot_sector:  87] FAT Length(sectors) : 3815464
[exfat_setup_boot_sector:  89] Cluster Heap Offset (sector offset) : 3866624
[exfat_setup_boot_sector:  91] Cluster Count : 487895318
[exfat_setup_boot_sector:  93] Root Cluster (cluster offset) : 14894
[exfat_setup_boot_sector:  95] Volume Serial : 0x779fe381
[exfat_setup_boot_sector:  96] Sector Size Bits : 9
[exfat_setup_boot_sector:  98] Sector per Cluster bits : 3
done
Fat table creation: [exfat_create_fat_table: 307] Total used cluster count : 14895
done
Allocation bitmap creation: done
Upcase table creation: done
Writing root directory entry: done
Synchronizing...

exFAT format complete!
```
The option: `-f` will do full format. It's too slow.

- Verify it
```
$ du -sh empty-dir/
4.0K	empty-dir/

$ du -sh empty
4.0K	empty
```

## Source code
- https://github.com/relan/exfat

```
$ apt show exfat-fuse
Package: exfat-fuse
Version: 1.3.0+git20220115-2
Priority: optional
Section: universe/otherosfs
Source: fuse-exfat
Origin: Ubuntu
Maintainer: Ubuntu Developers <ubuntu-devel-discuss@lists.ubuntu.com>
Original-Maintainer: Sven Hoexter <hoexter@debian.org>
Bugs: https://bugs.launchpad.net/ubuntu/+filebug
Installed-Size: 68.6 kB
Depends: libc6 (>= 2.34), libfuse2 (>= 2.6), fuse
Recommends: exfatprogs
Homepage: https://github.com/relan/exfat
Download-Size: 25.7 kB
APT-Sources: http://cn.archive.ubuntu.com/ubuntu jammy/universe amd64 Packages
Description: read and write exFAT driver for FUSE
 fuse-exfat is a read and write driver implementing
 the extended file allocation table as a filesystem in userspace.
 A mounthelper is provided unter the name mount.exfat-fuse.
```

- https://github.com/exfatprogs/exfatprogs [recommended]
