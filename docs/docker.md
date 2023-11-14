# Docker

## Essential Commands
- docker system df -v: to see the detailed space usage information.
- docker image ls: list images
- docker container ls -a:  list containers
- docker system prune: removes lots of redundant objects: all stopped containers, unused networks, dangling images, and build cache.
- docker run --privileged --pid=host docker/desktop-reclaim-space
- docker container prune: This will remove all stopped containers.  

- Show docker disk usage
```
$ docker system  df
TYPE            TOTAL     ACTIVE    SIZE      RECLAIMABLE
Images          6         2         62.23GB   30.98GB (49%)
Containers      3         2         279.2kB   0B (0%)
Local Volumes   0         0         0B        0B
Build Cache     0         0         0B        0B
```

- show docker dist usage(-v)
```
$ docker system  df -v
Images space usage:

REPOSITORY                              TAG              IMAGE ID       CREATED        SIZE      SHARED SIZE   UNIQUE SIZE   CONTAINERS
tutorials/workdir                       latest           0945a16d62b9   21 hours ago   78.21MB   77.81MB       400.5kB       0
mjolnir/opencv                          latest           7f5f4020cff2   5 days ago     3.65GB    1.716GB       1.934GB       2
mjolnir/base                            latest           73802e1686fd   5 days ago     1.716GB   1.716GB       0B            0
ubuntu                                  jammy-20230308   08d22c0ceb15   5 months ago   77.81MB   77.81MB       0B            0
```

SHARED SIZE is the amount of space that an image shares with another one (i.e. their common data)

UNIQUE SIZE is the amount of space that is only used by a given image

SIZE is the virtual size of the image, it is the sum of SHARED SIZE and UNIQUE SIZE


# Issues

- docker info
```
$ docker info
Client:
 Context:    default
 Debug Mode: false
 Plugins:
  app: Docker App (Docker Inc., v0.9.1-beta3)
  buildx: Docker Buildx (Docker Inc., v0.9.1-docker)
  scan: Docker Scan (Docker Inc., v0.21.0)

Server:
 Containers: 1
  Running: 1
  Paused: 0
  Stopped: 0
 Images: 15
 Server Version: 20.10.21
 Storage Driver: overlay2 # 这里是 overlay2
  Backing Filesystem: extfs
  Supports d_type: true
  Native Overlay Diff: true
  userxattr: false
 Logging Driver: json-file
 Cgroup Driver: systemd
 Cgroup Version: 2
```

- https://blog.csdn.net/w_y_x_y/article/details/108986916
- https://docs.docker.com/storage/storagedriver/


## [Docker ERROR: Error processing tar file(exit status 1): unexpected EOF](https://stackoverflow.com/questions/42784396/docker-error-error-processing-tar-fileexit-status-1-unexpected-eof)

> sudo chown -R zhishan:zhishan /var/lib/docker

There is an built in command to remove unused images (Version 1.13+):
```
docker image prune
```

Now to handle the situation:

- Stop Docker Service
```
systemctl stop docker
```

- Backup `/var/lib/docker` then remove `/var/lib/docker`

> Caution: This will remove images, containers, volumes, ... make sure you back it up first.

```
rm -rf /var/lib/docker
```

- Start Docker service
```
systemctl start docker  
```
After modifying the file: `docker.service `, it will tell you:
> The unit file, source configuration file or drop-ins of docker.service changed on disk. Run 'systemctl daemon-reload' to reload units.


In somecases it might be file permissions issue. Please review permissions.


In my case, the owner of the folder: `overlay2` is changed to __root__ after building a image from local.

- Before the building, I changed the owner of the `/var/lib/docker`.
```
$ sudo chown anonymous:anonymous -R /var/lib/docker
```

- Building the docker image with the command:
```
docker build --network=host --build-arg USE_LATEST_BOOST=true -t mjolnir/base:latest --pull -f $< .
```

- Review the `/var/lib/docker`:
```
root@006IT00075:/var/lib/docker# ll
total 64
drwx--x---  13 anonymous anonymous  4096  8月  1 11:20 ./
drwxr-xr-x  73 root    root     4096  8月  1 13:32 ../
drwx--x--x   4 anonymous anonymous  4096  8月  1 11:20 buildkit/
drwx--x---   3 anonymous anonymous  4096  8月  1 13:31 containers/
drwx------   3 anonymous anonymous  4096  8月  1 11:20 image/
drwxr-x---   3 anonymous anonymous  4096  8月  1 11:20 network/
drwx--x--- 126 root    root    16384  8月  1 13:31 overlay2/
drwx------   4 anonymous anonymous  4096  8月  1 11:20 plugins/
drwx------   2 anonymous anonymous  4096  8月  1 11:20 runtimes/
drwx------   2 anonymous anonymous  4096  8月  1 11:20 swarm/
drwx------   2 anonymous anonymous  4096  8月  1 13:31 tmp/
drwx------   2 anonymous anonymous  4096  8月  1 11:20 trust/
drwx-----x   2 anonymous anonymous  4096  8月  1 11:20 volumes/
```
`overlay2`'s owner is changed to __root__. It leads into the above error.

But what do the change during the building process?

## [What's the __overlay2__ folder in docker?](https://stackoverflow.com/questions/46672001/is-it-safe-to-clean-docker-overlay2)

`/var/lib/docker/overlay2` - is a folder where docker store writable layers for your container.
`overlay2` is system docker folder, and they may change it structure anytime.



- docker builder prune --all
- docker system prune -a && docker volume prune

- [Why is Docker filling up /var/lib/docker/overlay2?](https://stackoverflow.com/questions/61592088/why-is-docker-filling-up-var-lib-docker-overlay2)
  - du -h --max-depth=1 /var/lib/docker/overlay2 | sort -rh | head -25
  
  - find /var/lib/docker/containers/ -type f -name "*.log" -delete
  - sudo systemctl restart docker


## [Docker overlay2 eating Disk Space](https://stackoverflow.com/questions/61884165/docker-overlay2-eating-disk-space)
Add the lines into `/etc/docker/daemon.json` to setup a log rotation.
```
{
  "log-driver": "json-file",
  "log-opts": {
    "max-size": "10m",
    "max-file": "3"
    }
}
```


## Can't initialize GTK backend in function 'cvInitSystem'

- Solution : `ssh -Y user@wherever`
  - `-Y`: Enables trusted X11 forwarding. Trusted X11 forwardings are not subjected to the X11 SECURITY extension controls.

## Remove images

```
$ docker images
REPOSITORY                              TAG                               IMAGE ID       CREATED        SIZE
stammerer                               1.2.0                             1eb764078442   5 months ago   9.3GB
stammerer                               1.2.0                             1eb764078442   5 months ago   9.3GB

$ docker rmi stammerer
Untagged: stammerer:latest

$ docker rmi stammerer
Error: No such image: stammerer

$ docker rmi 1eb764078442
Untagged: stammerer:1.2.0
Deleted: sha256:1eb76407844223656b4fb09f66d383b331ad427ed99f275603fa11a839e9d6e7
```

## Alias a docker image
```
docker tag stammerer:latest stammerer:myversion
```
So the image stammerer will not be removed if either `stammerer:latest` or `stammerer:myversion` exists.

## Save Container
```
$ docker ps

container id ...
b8a238692945

$ docker commit b8a238692945 mjolnir/opencv:latest
```


## Why /var/run/docker.sock permissions are changed every time I log out? How can I forbid it?
- https://askubuntu.com/questions/1194205/why-var-run-docker-sock-permissions-are-changed-every-time-i-log-out-how-can-i

- https://segmentfault.com/a/1190000039426040

- https://www.escapelife.site/posts/43a2bb9b.html

- https://cloud.tencent.com/developer/article/2146377?areaSource=&traceId=

- https://slions.github.io/2021/07/12/%E7%90%86%E8%A7%A3%E5%AD%98%E5%82%A8%E9%A9%B1%E5%8A%A8overlay2/


sudo groupadd docker
sudo usermod -aG docker $USER
newgrp docker

You can change the group ownership permanently by editing /etc/systemd/system/sockets.target.wants/docker.socket.

Set the right group at line

SocketGroup=docker
Then run systemctl daemon-reload and reboot

