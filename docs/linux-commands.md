Linux Commands

# find

## find查找指定时间内修改过的文件或目录

find命令的格式
```
find pathname -options [ -print -exec -ok ...]
```

* `-ok`: 它的作用和 `-exec` 一样，只是需要用户交互，更安全
  - 

Options:

- `-mtime`: 天单位

- `-mmin`: 分钟单位, `-5`: 即表示五分钟内修改过的, `+5` or `5`就是五分钟之前修改过的


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

