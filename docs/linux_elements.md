# Linux Elements

## fsck(file system check)
用于检查和维护不一致的文件系统。若系统掉电或磁盘发生问题，可利用fsck命令对文件系统进行检查。

> UNEXPECTED INCOSISTENCY: RUN fsck MANUALLY

```
fsck -f -y /dev/sda3
```
- `-f`: 表示对没有错误的文件强制检查

- `-y`: 自动执行修复

几分钟后，按 `ctr+d`, 系统重启
