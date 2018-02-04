#!/bin/bash

#
# Script to build bbl with riscv-linux plus busybox payload
#

set -e

TOPDIR=$(pwd)
ARCHIVE_DIR=${TOPDIR}/build/archive
BUILD_DIR=${TOPDIR}/build/linux

RISCV_RISCV_PK_REPO=https://github.com/michaeljclark/riscv-pk.git

BUSYBOX_VERSION=1.26.1
BUSYBOX_ARCHIVE=busybox-${BUSYBOX_VERSION}.tar.bz2
BUSYBOX_URL=https://busybox.net/downloads/${BUSYBOX_ARCHIVE}
BUSYBOX_BUILD_DIR=${TOPDIR}/build/busybox-${BUSYBOX_VERSION}

LINUX_VERSION=4.6.2-riscv-a1
LINUX_ARCHIVE=riscv-linux-${LINUX_VERSION}.tar.gz
LINUX_URL=https://github.com/michaeljclark/riscv-linux/archive/v${LINUX_VERSION}.tar.gz
LINUX_BUILD_DIR=${TOPDIR}/build/riscv-linux-${LINUX_VERSION}

RISCV_PK_BUILD_DIR=${TOPDIR}/build/riscv-pk

XCODE_DIR=/Applications/Xcode.app/
XCODE_INCLUDE=${XCODE_DIR}/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr/include/

#
# Find out number of processors
#

NPROCS=1
OS=$(uname -s)
if [ "${OS}" = "Linux" ]; then
	NPROCS=$(grep -c ^processor /proc/cpuinfo)
fi
if [ "${OS}" = "Darwin" ]; then
	NPROCS=$(sysctl -n hw.ncpu)
fi

if [ ! -d "${ARCHIVE_DIR}" ]; then
	mkdir -p ${ARCHIVE_DIR}
fi

#
# Display elf.h note if we are building on macos
#
if [ "${OS}" = "Darwin" ]; then
	if [ ! -d "${XCODE_DIR}" ]; then
		echo "You need to install Xcode"
	fi
fi
if [ -d "${XCODE_DIR}" -a ! -f "${XCODE_INCLUDE}/elf.h" ]; then
	echo "===[ Manually patch Xcode with Linux /usr/include/elf.h ]==="
	echo "Locate Linux /usr/include/elf.h header and copy to ${XCODE_INCLUDE}"
	echo "You may also need to install ncurses. e.g. brew install ncurses"
	exit
fi

#
# Download and build busybox
#

if [ ! -f "${ARCHIVE_DIR}/${BUSYBOX_ARCHIVE}" ]; then
	echo "===[ Downloading Busybox ]==="
	curl -o ${ARCHIVE_DIR}/${BUSYBOX_ARCHIVE} ${BUSYBOX_URL}
fi

if [ ! -d "${BUSYBOX_BUILD_DIR}" ]; then
	echo "===[ Extracting Busybox ]==="
	(
		cd build
		tar xjf ${ARCHIVE_DIR}/${BUSYBOX_ARCHIVE}
	)
fi

echo "===[ Building Busybox ]==="
(
	cp scripts/busybox.config ${BUSYBOX_BUILD_DIR}/.config
	cd ${BUSYBOX_BUILD_DIR}
	make ARCH=riscv CROSS_COMPILE=riscv64-unknown-linux-gnu- oldconfig
	make -j${NPROCS} ARCH=riscv CROSS_COMPILE=riscv64-unknown-linux-gnu-
)

#
# Download and build riscv-linux
#

if [ ! -f "${ARCHIVE_DIR}/${LINUX_ARCHIVE}" ]; then
	echo "===[ Downloading Linux Kernel ]==="
	curl -L -o ${ARCHIVE_DIR}/${LINUX_ARCHIVE} ${LINUX_URL}
fi

if [ ! -d "${LINUX_BUILD_DIR}" ]; then
	echo "===[ Extracting Linux Kernel ]==="
	(
		cd build
		tar xzf ${ARCHIVE_DIR}/${LINUX_ARCHIVE}
	)
fi

if [ ! -d "${LINUX_BUILD_DIR}/initramfs" ]; then
	echo "===[ Creating RISC-V Linux kernel initramfs directory ]==="
	mkdir ${LINUX_BUILD_DIR}/initramfs
fi

echo "===[ Creating RISC-V Linux kernel initramfs configuration ]==="
cp scripts/initramfs.config ${LINUX_BUILD_DIR}/initramfs.config
cp scripts/inittab ${LINUX_BUILD_DIR}/initramfs/inittab

echo "===[ Copying Busybox into RISC-V Linux kernel initramfs directory ]==="
cp ${BUSYBOX_BUILD_DIR}/busybox ${LINUX_BUILD_DIR}/initramfs/busybox

echo "===[ Configuring RISC-V Linux kernel ]==="
cp scripts/linux.config ${LINUX_BUILD_DIR}/.config
(
	cd ${LINUX_BUILD_DIR}
	make ARCH=riscv oldconfig
)

echo "===[ Building RISC-V Linux kernel ]==="
(
	cd ${LINUX_BUILD_DIR}
	make -j${NPROCS} ARCH=riscv vmlinux
)

#
# Download and build riscv-pk
#

if [ ! -d "${RISCV_PK_BUILD_DIR}" ]; then
	echo "===[ Cloning riscv-pk repository ]==="
	(
		cd build
		git clone --branch bbl-lite ${RISCV_RISCV_PK_REPO}
	)
fi

if [ ! -d "${RISCV_PK_BUILD_DIR}/build" ]; then
	echo "===[ Configuring riscv-pk repository ]==="
	(
		mkdir ${RISCV_PK_BUILD_DIR}/build
		cd ${RISCV_PK_BUILD_DIR}/build
		../configure --host=riscv64-unknown-elf --with-payload=${LINUX_BUILD_DIR}/vmlinux
	)
fi

echo "===[ Building riscv-pk Repository ]==="
(
	cd ${RISCV_PK_BUILD_DIR}/build
	make -j$NPROCS
)

echo "===[ Copying riscv-pk to target directory ]==="
if [ ! -d "build/riscv64-unknown-elf/bin" ]; then
	mkdir -p build/riscv64-unknown-elf/bin
fi
cp ${RISCV_PK_BUILD_DIR}/build/bbl build/riscv64-unknown-elf/bin/bbl

#
# Ready to run
#
echo "===[ Build complete ]==="
echo
echo "usage: rv-sys build/riscv64-unknown-elf/bin/bbl"
