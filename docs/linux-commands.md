Linux Commands

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