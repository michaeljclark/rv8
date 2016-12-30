Linux
==========

## Dependencies

- [riscv-tools](https://github.com/riscv/riscv-tools/)
- [riscv-linux](https://github.com/riscv/riscv-linux/)
- [riscv-pk](https://github.com/riscv/riscv-pk/)
- [riscv-meta](https://github.com/michaeljclark/riscv-meta/)


## Build riscv-meta

```
cd $HOME/src/
$ git clone https://github.com/michaeljclark/riscv-meta.git
cd riscv-meta
make -j4 && sudo make install
```


## Build RISC-V Linux kernel priv-1.9 branch

Commands to pull the priv-1.9 branch from here [riscv-linux](https://github.com/riscv/riscv-linux/).

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

Configure specific kernel config parameters for initramfs support:

```
CONFIG_BLK_DEV_INITRD=y
CONFIG_CMDLINE="earlyprintk=sbi-console rdinit=/bin/ash"
CONFIG_INITRAMFS_SOURCE="initramfs.txt"
```


## Busybox initramfs

Read [ramfs-rootfs-initramfs.txt](https://www.kernel.org/doc/Documentation/filesystems/ramfs-rootfs-initramfs.txt)

Create a directory named `initramfs` in the kernel directory and copy in the compiled `busybox`

This script can be used to create an initramfs specification file and directory structure:

```
mkdir initramfs
cp ../busybox-1.24.1/busybox initramfs/
cd linux-4.6.2
cat >initramfs.txt <<EOF
dir /dev 755 0 0
nod /dev/console 644 0 0 c 5 1
nod /dev/loop0 644 0 0 b 7 0
dir /bin 755 1000 1000
slink /bin/ash busybox 777 0 0
file /bin/busybox initramfs/busybox 755 0 0
dir /proc 755 0 0
dir /sys 755 0 0
dir /mnt 755 0 0
EOF
```


## Build BBL with Linux Busybox Payload

The following command will build BBL (Berkeley Boot Loader) with a Linux payload:

```
cd $HOME/src/riscv-meta/
make -j4 && sudo make install
git clone https://github.com/riscv/riscv-pk.git
mkdir riscv-pk/build
( cd riscv-pk/build && ../configure --host=riscv64-unknown-elf --with-payload=$HOME/src/riscv-meta/linux-4.6.2/vmlinux )
( cd riscv-pk/build && make )
```


## Start rv-sys

The following command starts the simulation:

```
rv-sys riscv-pk/build/bbl
```

Example output:

```
              vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
                  vvvvvvvvvvvvvvvvvvvvvvvvvvvv
rrrrrrrrrrrrr       vvvvvvvvvvvvvvvvvvvvvvvvvv
rrrrrrrrrrrrrrrr      vvvvvvvvvvvvvvvvvvvvvvvv
rrrrrrrrrrrrrrrrrr    vvvvvvvvvvvvvvvvvvvvvvvv
rrrrrrrrrrrrrrrrrr    vvvvvvvvvvvvvvvvvvvvvvvv
rrrrrrrrrrrrrrrrrr    vvvvvvvvvvvvvvvvvvvvvvvv
rrrrrrrrrrrrrrrr      vvvvvvvvvvvvvvvvvvvvvv  
rrrrrrrrrrrrr       vvvvvvvvvvvvvvvvvvvvvv    
rr                vvvvvvvvvvvvvvvvvvvvvv      
rr            vvvvvvvvvvvvvvvvvvvvvvvv      rr
rrrr      vvvvvvvvvvvvvvvvvvvvvvvvvv      rrrr
rrrrrr      vvvvvvvvvvvvvvvvvvvvvv      rrrrrr
rrrrrrrr      vvvvvvvvvvvvvvvvvv      rrrrrrrr
rrrrrrrrrr      vvvvvvvvvvvvvv      rrrrrrrrrr
rrrrrrrrrrrr      vvvvvvvvvv      rrrrrrrrrrrr
rrrrrrrrrrrrrr      vvvvvv      rrrrrrrrrrrrrr
rrrrrrrrrrrrrrrr      vv      rrrrrrrrrrrrrrrr
rrrrrrrrrrrrrrrrrr          rrrrrrrrrrrrrrrrrr
rrrrrrrrrrrrrrrrrrrr      rrrrrrrrrrrrrrrrrrrr
rrrrrrrrrrrrrrrrrrrrrr  rrrrrrrrrrrrrrrrrrrrrr

       INSTRUCTION SETS WANT TO BE FREE
[    0.000000] Linux version 4.6.2-00033-gb596193-dirty (mclark@monty.local) (gcc version 6.1.0 (GCC) ) #50 Fri Dec 30 20:34:59 NZDT 2016
[    0.000000] bootconsole [early0] enabled
[    0.000000] Available physical memory: 1020MB
[    0.000000] Initial ramdisk at: 0xffffffff80013c10 (489035 bytes)
[    0.000000] Zone ranges:
[    0.000000]   Normal   [mem 0x0000000080400000-0x00000000bfffffff]
[    0.000000] Movable zone start for each node
[    0.000000] Early memory node ranges
[    0.000000]   node   0: [mem 0x0000000080400000-0x00000000bfffffff]
[    0.000000] Initmem setup node 0 [mem 0x0000000080400000-0x00000000bfffffff]
[    0.000000] Built 1 zonelists in Zone order, mobility grouping on.  Total pages: 257550
[    0.000000] Kernel command line: earlyprintk=sbi-console rdinit=/bin/ash 
[    0.000000] PID hash table entries: 4096 (order: 3, 32768 bytes)
[    0.000000] Dentry cache hash table entries: 131072 (order: 8, 1048576 bytes)
[    0.000000] Inode-cache hash table entries: 65536 (order: 7, 524288 bytes)
[    0.000000] Sorting __ex_table...
[    0.000000] Memory: 1025264K/1044480K available (2005K kernel code, 107K rwdata, 396K rodata, 560K init, 220K bss, 19216K reserved, 0K cma-reserved)
[    0.000000] SLUB: HWalign=32, Order=0-3, MinObjects=0, CPUs=1, Nodes=1
[    0.000000] NR_IRQS:0 nr_irqs:0 0
[    0.000000] clocksource: riscv_clocksource: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 191126044627 ns
[    0.000000] Calibrating delay loop (skipped), value calculated using timer frequency.. 20.00 BogoMIPS (lpj=100000)
[    0.000000] pid_max: default: 32768 minimum: 301
[    0.000000] Mount-cache hash table entries: 2048 (order: 2, 16384 bytes)
[    0.000000] Mountpoint-cache hash table entries: 2048 (order: 2, 16384 bytes)
[    0.000000] devtmpfs: initialized
[    0.000000] clocksource: jiffies: mask: 0xffffffff max_cycles: 0xffffffff, max_idle_ns: 19112604462750000 ns
[    0.000000] NET: Registered protocol family 16
[    0.000000] clocksource: Switched to clocksource riscv_clocksource
[    0.000000] NET: Registered protocol family 2
[    0.000000] TCP established hash table entries: 8192 (order: 4, 65536 bytes)
[    0.000000] TCP bind hash table entries: 8192 (order: 4, 65536 bytes)
[    0.000000] TCP: Hash tables configured (established 8192 bind 8192)
[    0.000000] UDP hash table entries: 512 (order: 2, 16384 bytes)
[    0.000000] UDP-Lite hash table entries: 512 (order: 2, 16384 bytes)
[    0.000000] NET: Registered protocol family 1
[    0.000000] Unpacking initramfs...
[    0.000000] console [sbi_console0] enabled
[    0.000000] console [sbi_console0] enabled
[    0.000000] bootconsole [early0] disabled
[    0.000000] bootconsole [early0] disabled
[    0.000000] futex hash table entries: 256 (order: 0, 6144 bytes)
[    0.000000] workingset: timestamp_bits=61 max_order=18 bucket_order=0
[    0.000000] io scheduler noop registered
[    0.000000] io scheduler cfq registered (default)
[    0.000000] Freeing unused kernel memory: 560K (ffffffff80000000 - ffffffff8008c000)
[    0.000000] This architecture does not have kernel memory protection.
#
```