ARCH=X86_64
BITS=64
DEBUG=FALSE
OPTIMIZE=FALSE
MULTICORE=FALSE
LOAD_SYSTEM=MULTIBOOT2
FILE_SYSTEM=FAT32

# Modules
BUILD_KERNEL=TRUE
BUILD_LIBC=FALSE
BUILD_DRIVERS=FALSE
LINK_INITIMG=TRUE
CREATE_ROOTDIR=FALSE

# Paths
SOURCE_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))
ROOT_DIR=$(SOURCE_DIR)/rootdir
IMG_LINK=

# Binaries
CC=gcc
CXX=g++
ASM=nasm
LINK=ld

# Flags
INCLUDE_DIRS=-I$(SOURCE_DIR)/arch/include -I$(SOURCE_DIR)/drivers/include -I$(SOURCE_DIR)/newlibc/include
CC_FLAGS=-nostdinc -ffreestanding -Wall -Wextra -c -masm=intel -zmuldefs -fno-stack-protector $(INCLUDE_DIRS)
CXX_FLAGS=$(CC_FLAGS)
ASM_FLAGS=-f elf64 -Wall
LINK_FLAGS=-m elf_x86_64 -nostdlib -z muldefs

ifeq ($(DEBUG), TRUE)
CC_FLAGS += -g
CXX_FLAGS += -g
ASM_FLAGS += -g
LINK_FLAGS += -g
endif
ifeq ($(OPTIMIZE), TRUE)
CC_FLAGS += -O2
CXX_FLAGS += -O2
ASM_FLAGS += -O2
LINK_FLAGS += -O
endif
ifeq ($(BITS), 64)
CC_FLAGS += -m64
CXX_FLAGS += -m64
else
CC_FLAGS += -m32
CXX_FLAGS += -m32
endif

# Color codes (for print)
RED=\033[1;31m
GREEN=\033[1;32m
WHITE=\033[1;37m
GRAY=\033[0;37m
NC=\033[0m

COMPILEC=$(GREEN)
LINKC=$(NC)
ERRORC=$(RED)
SECTIONC=$(WHITE)

# Documentation for variables:

# ARCH:			Main architecture
# BITS:			Number of bits in architecture
# DEBUG:		Compile with debug symbols
# OPIMIZE:		Optimize codde
# MULTICORE:	Use multicore (if available)
# LOAD_SYSTEM:	Boot method
# FILE_SYSTEM:	Main file system on root drive

# BUILD_KERNEL:  	Build the object files for the kernel
# BUILD_LIBC:    	Build newlix C library
# BUILD_DRIVERS: 	Build the userspace drivers
# LINK_INITIMG:  	Create init boot image (loaded by the bootloader)
# CREATE_ROOTDIR:	Create root directory