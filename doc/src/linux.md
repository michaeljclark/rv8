Linux
==========

## Dependencies

[riscv-tools](https://github.com/riscv/riscv-tools/)
[riscv-linux](https://github.com/riscv/riscv-linux/)
[riscv-pk](https://github.com/riscv/riscv-pk/)
[riscv-meta](https://github.com/michaeljclark/riscv-meta/)


## Build riscv-meta

```
cd $HOME/src/
$ git clone https://github.com/michaeljclark/riscv-meta.git
cd riscv-meta
make -j4 && sudo make install
```


## Build RISC-V Linux kernel priv-1.9 branch

```
cd $HOME/src/riscv-meta/
curl -L https://cdn.kernel.org/pub/linux/kernel/v4.x/linux-4.6.2.tar.xz | tar -xJ
cd linux-4.6.2
git init
git remote add -t priv-1.9 origin https://github.com/riscv/riscv-linux.git
git fetch
git checkout -f -t origin/priv-1.9
make ARCH=riscv menuconfig
make -j4 ARCH=riscv vmlinux
```


## Compile Busybox and customize kernel config

Compile [Busybox](https://www.busybox.net/) with ash support.

Create a directory named `initramfs` in the kernel directory and copy in the compiled `busybox`

Note specific kernel config parameters

```
CONFIG_BLK_DEV_INITRD=y
CONFIG_CMDLINE="earlyprintk=sbi-console rdinit=/bin/ash"
CONFIG_INITRAMFS_SOURCE="initramfs.txt"
```


## Busybox initramfs.txt specification file

Read [ramfs-rootfs-initramfs.txt](https://www.kernel.org/doc/Documentation/filesystems/ramfs-rootfs-initramfs.txt)

```
dir /dev 755 0 0
nod /dev/console 644 0 0 c 5 1
nod /dev/loop0 644 0 0 b 7 0
dir /bin 755 1000 1000
slink /bin/ash busybox 777 0 0
file /bin/busybox initramfs/busybox 755 0 0
dir /proc 755 0 0
dir /sys 755 0 0
dir /mnt 755 0 0
```

## Build BBL with Linux Payload

```
cd $HOME/src/riscv-meta/
make -j4 && sudo make install
git clone https://github.com/riscv/riscv-pk.git
mkdir riscv-pk/build
( cd riscv-pk/build && ../configure --host=riscv64-unknown-elf --with-payload=$HOME/src/riscv-meta/linux-4.6.2/vmlinux )
( cd riscv-pk/build && make )
```

_Note: alter directories to suit._

## Start rv-sys
```
rv-sys riscv-pk/build/bbl
```