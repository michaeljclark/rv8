Linux
==========

This document outlines how to build busybox, bbl-lite and riscv-linux
to run on the riscv-meta `rv-sys` system emulator. Note: The build
scripts require that the `riscv64-unknown-elf` and `riscv64-linux-gnu`
toolchains are installed and located in directory specified by the
environment variable `RISCV`.

## Dependencies

- [riscv-gnu-toolchain](https://github.com/riscv/riscv-gnu-toolchain/)
- [riscv-linux](https://github.com/michaeljclark/riscv-linux/)
- [bbl-lite](https://github.com/michaeljclark/bbl-lite/)
- [riscv-meta](https://github.com/michaeljclark/riscv-meta/)


## Build riscv-meta

```
cd $HOME/src/
$ git clone https://github.com/michaeljclark/riscv-meta.git
cd riscv-meta
make -j4 && sudo make install
```


## Build RISC-V Linux

Builds busybox, bbl-lite and riscv-linux:

- [busybox](https://busybox.net/)
- [riscv-linux](https://github.com/michaeljclark/riscv-linux/)
- [bbl-lite](https://github.com/michaeljclark/bbl-lite/)

```
cd riscv-meta
make linux
```


## Start RISC-V Linux

The following command starts the simulation:

```
rv-sys build/riscv64-unknown-elf/bin/bbl
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
[    0.000000] Linux version 4.6.2-00043-g23bf08e (mclark@minty) (gcc version 6.1.0 (GCC) ) #6 Tue Jan 31 18:39:31 NZDT 2017
[    0.000000] bootconsole [early0] enabled
[    0.000000] Available physical memory: 1018MB
[    0.000000] Initial ramdisk at: 0xffffffff80015f30 (724872 bytes)
[    0.000000] Zone ranges:
[    0.000000]   Normal   [mem 0x0000000080600000-0x00000000bfffffff]
[    0.000000] Movable zone start for each node
[    0.000000] Early memory node ranges
[    0.000000]   node   0: [mem 0x0000000080600000-0x00000000bfffffff]
[    0.000000] Initmem setup node 0 [mem 0x0000000080600000-0x00000000bfffffff]
[    0.000000] Built 1 zonelists in Zone order, mobility grouping on.  Total pages: 257045
[    0.000000] Kernel command line: earlyprintk=sbi-console rdinit=/sbin/init 
[    0.000000] PID hash table entries: 4096 (order: 3, 32768 bytes)
[    0.000000] Dentry cache hash table entries: 131072 (order: 8, 1048576 bytes)
[    0.000000] Inode-cache hash table entries: 65536 (order: 7, 524288 bytes)
[    0.000000] Sorting __ex_table...
[    0.000000] Memory: 1022420K/1042432K available (2442K kernel code, 123K rwdata, 488K rodata, 800K init, 230K bss, 20012K reserved, 0K cma-reserved)
[    0.000000] SLUB: HWalign=64, Order=0-3, MinObjects=0, CPUs=1, Nodes=1
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
[    0.000000] 9p: Installing v9fs 9p2000 file system support
[    0.000000] io scheduler noop registered
[    0.000000] io scheduler cfq registered (default)
[    0.000000] 9pnet: Installing 9P2000 support
[    0.000000] Freeing unused kernel memory: 800K (ffffffff80000000 - ffffffff800c8000)
[    0.000000] This architecture does not have kernel memory protection.


BusyBox v1.26.1 (2017-01-20 22:18:31 NZDT) built-in shell (ash)
Enter 'help' for a list of built-in commands.

/ # 
```
