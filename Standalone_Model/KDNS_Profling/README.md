
## KDNS - DNS resolver with DPDK

Using the standalone deployment model, we profiled the DNS resolver application kdns, which utilizes DPDK as its packet I/O library. DPDK (Data Plane Development Kit) is a kernel-bypass technology that allows packets to be forwarded directly from the NIC to userspace, bypassing the traditional kernel network stack. Running a DPDK-based application inside a container can be complex, primarily due to the need to bind the NIC directly to the DPDK process. For this reason, we opted for a standalone deployment model for profiling kdns. As part of our experiment, we profiled the kdns application in two scenarios: 1.With DPDK (kernel-bypass), 2. Without DPDK (using the kernel network stack).


## KDNS Installation 

## Introduction

KDNS is a high-performance DNS Server based on DPDK. Do not rely on the database, the user updates the data through the RESTful API.

## How to use

### 1. Compilation

Required  OS release: Ubuntu 22.04
Kernel version >= 4.14
glibc >= 2.7  (check with ldd --version)
DPDK version:- 22.11.6
GCC version:- 4.9+

1) Upgrade pkg-config while version < 0.28

```bash

wget https://pkg-config.freedesktop.org/releases/pkg-config-0.29.2.tar.gz
tar xzvf pkg-config-0.29.2.tar.gz
cd pkg-config-0.29.2
./configure --with-internal-glib
make
make install
mv /usr/bin/pkg-config /usr/bin/pkg-config.bak
ln -s /usr/local/bin/pkg-config /usr/bin/pkg-config

```

2) Install Required packages


```bash

apt install update
apt install libtool libtool-bin libpcap-dev texinfo libnuma-dev
apt install kernel-headers-$(uname -r) kernel-devel-$(uname -r)

```

3) Clone the repo and set PKG_CONFIG_PATH environment variable and allocate hugepages.

```bash

git clone https://github.com/modi2207/kdns.git
cd kdns
export PKG_CONFIG_PATH=/usr/lib/x86_64-linux-gnu/pkgconfig:/usr/local/lib/x86_64-linux-gnu/pkgconfig:/usr/lib/pkgconfig
mkdir -p /mnt/huge
mount -t hugetlbfs nodev /mnt/huge

```

4) Make all for the first time, after that make kdns if you just change the DNS code.

```bash
make all
```

### 2. Startup

The default configuration path for KDNS is /etc/kdns/kdns.cfg. An example for kdns.cfg as follows :

EAL configuration reference [DPDK document](http://dpdk.org/doc/guides/testpmd_app_ug/run_app.html#eal-command-line-options).

```bash
[EAL]
cores = 0,2
memory = 1024
mem-channels = 4
 
[NETDEV]
; 默认KNI网口名称
name-prefix = kdns
mode = rss
mbuf-num = 65535
kni-mbuf-num = 8191
rxqueue-len = 1024
txqueue-len = 2048
    
rxqueue-num = 1
txqueue-num = 1

; KNI网口IP地址
kni-ipv4 = 2.2.2.240
; BGP 发布的VIP
kni-vip = 127.0.0.1

[COMMON]
log-file = /export/log/kdns/kdns.log

fwd-def-addrs = 114.114.114.114:53,8.8.8.8:53
; 转发线程数
fwd-thread-num = 4
; 转发模式
fwd-mode = cache
; 转发请求超时时间
fwd-timeout = 2
; 转发请求mbuf数
fwd-mbuf-num = 65535

; 每IP全部报文限速
all-per-second = 1000
; 每IP DNS转发请求限速
fwd-per-second = 10
; 限速客户端数, 设置为0, 则关闭限速功能
client-num = 10240

web-port = 5500
ssl-enable = no
cert-pem-file = /etc/kdns/server1.pem
key-pem-file = /etc/kdns/server1-key.pem
zones = tst.local,example.com,168.192.in-addr.arpa
```

Start kdns:

```bash
sh start.sh
```

## Profile KDNS with standalone binary of perforator 

1. Build perforator CLI binary 

    ```bash
    ./ya make -r perforator/cmd/cli
    ```

2. Record and publish the profile using following command

    ```bash
        cd perforator/cmd/cli
        ./perforator record -p <process_id of kdns> --duration 1m --serve ":9001"
    ```

    It record the profile for 1 minute and then publish a flamegraph on 9001 port. You will get http URL of flamegraph. 


3. To make DNS reqeust to the kdns, we used dnspyre opensource tool (https://tantalor93.github.io/dnspyre/)

    3.1 DNS requests to DPDK path

       On the second VM, we added a static ARP entry mapping the IP address 10.10.1.9 to the MAC address of the DPDK NIC.

        ```bash
        arp -s 10.10.1.9 <mac_address of DPDK NIC>
        ```
        
       Then make DNS request using following command

        ```bash 
        ./dnspyre -s 10.10.1.9 -d 1m google.com
        ```

    3.2 DNS requests to Kernel path

        On the same VM (on which KDNS is runnning), we are using following command 

         ```bash 
        ./dnspyre -s 127.0.0.1 -d 1m google.com
        ```
