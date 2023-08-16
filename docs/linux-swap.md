# [Use the file as Swap partition](https://zhuanlan.zhihu.com/p/607087087)

- Use 8G file as Swap 
```
# mkdir /swap

# dd if=/dev/zero of=/swap/swapfile bs=1M count=8192
8192+0 records in
8192+0 records out
8589934592 bytes (8.6 GB, 8.0 GiB) copied, 4.94766 s, 1.7 GB/s

# mkswap /swap/swapfile
mkswap: /swap/swapfile: insecure permissions 0644, fix with: chmod 0600 /swap/swapfile
Setting up swapspace version 1, size = 8 GiB (8589930496 bytes)
no label, UUID=20b3f1ee-05ee-42d5-89d5-70aac04edd27

# ll /swap/swapfile
-rw-r--r-- 1 root root 8589934592  8月 15 15:53 /swap/swapfile

# chmod 0600 /swap/swapfile

# ll /swap/swapfile
-rw------- 1 root root 8589934592  8月 15 15:53 /swap/swapfile

# swapon /swap/swapfile
swapon: /swap/swapfile: swapon failed: Device or resource busy
```

- Edit `/etc/fstab` with adding the line:

```
# <file system> <mount point>   <type>  <options>       <dump>  <pass>
/swap/swapfile swap swap defaults 0 0
```

- Check it after reboot
```
# cat /proc/swaps
Filename				Type		Size		Used		Priority
/swap/swapfile                          file		8388604		0		-2
/dev/dm-1                               partition	999420		0		-3

# swapoff /swap/swapfile

# cat /proc/swaps
Filename				Type		Size		Used		Priority
/dev/dm-1                               partition	999420		0		-2

# swapon /swap/swapfile

```

### Check Swap Usage
To view all devices marked as a swap in the `/etc/fstab` file you can use the `--all` option. Though devices that are already working as swap space are skipped.
```
# swapon --all
```
If you want to view a summary of swap space usage by device, use the `--summary` option as follows.

- Using /proc/swaps – Measures Swap Space
- free
- top
- atop
- htop
- vmstat

