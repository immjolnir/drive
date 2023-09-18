
Zeroconf协议的实现
zeroconf是协议，因此对应的是各种产品的实现，现在最主要的实现是apple的Bonjour和开源的Avahi。
这两套实现在程序接口上是可以兼容的，因此可以对zeroconf技术依赖的产品，可以使用兼容的开发接口，而不需要考虑到底是使用了Bonjour还是avahi。

### Apple的zeroconf协议技术实现 – Bonjour
- https://developer.apple.com/bonjour/
  - https://github.com/apple-oss-distributions/mDNSResponder/

Bonjour是由apple实现的zeroconf协议的技术产品，Bonjour来源于法语，意思为你好。
使用了zeroconf技术的产品和服务，在网络中自动传播它们自己的服务信息并聆听其它设备的服务信息，设备之间就象在打招呼，这也是命名为Bonjour(法语：你好)的原因。
Bonjour是一套跨平台的产品，可以使用在Mac，Linux和Winows，以及是兼容POSIX的操作系统上，并且apple已经將其开源，所以也可以方便应用于各个产品。
Bonjour是由纯C编写的，同时也提供了Java的接口，目前如python或者ruby语言也可以很方便地调用使用它。

### 开源的zeroconf协议技术实现 – Avahi
- https://github.com/lathiat/avahi
- https://github.com/flungo-docker/avahi
- https://github.com/python-zeroconf/python-zeroconf


Avahi 是Zeroconf规范的开源实现，基本是使用在Linux和FreeBSD上。包含了一整套多播DNS(multicastDNS)/DNS-SD网络服务的实现，使用的发布授权是LGPL。
在程序接口上，它使用DNSD与apple的Bonjour进行兼容，方便原来使用Bonjour的产品进行迁移。
目前在绝大多数的Linux发行版本中，都讲avahi作为系统启动进程，并且大量的服务也依赖于avahi。
Avahi也是用纯C编写的，同样也可以使用诸如Java的语言去调用使用开发产品或者服务。

zeroconf最基础原理
zerconf协议主要是定义了三个层次：
1.不需利用dhcp server取得 设备地址如IP的相关资料
RFC 3927 , Dynamic Configuration of IPv4 Link-Local Addresses

http://www.ietf.org/rfc/rfc3927.txt

2.不需要通过DNS server就转换domain name和IP的关系
Multicast DNS

http://files.multicastdns.org/draft-cheshire-dnsext-multicastdns.txt

3.利用 DNS-SD来取得设备服务,而不需通过directory server
DNS-based Service Discovery, or DNS-SD.

http://files.dns-sd.org/draft-cheshire-dnsext-dns-sd.txt


