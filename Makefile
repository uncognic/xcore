PREFIX ?= $(HOME)/opt/cross
TARGET ?= i686-elf
PATH := $(PREFIX)/bin:$(PATH)

CC := $(TARGET)-gcc
AS := $(TARGET)-as
GRUB_MKRESCUE ?= grub-mkrescue
QEMU ?= qemu-system-i386

CFLAGS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS := -T src/linker.ld -ffreestanding -O2 -nostdlib

KERNEL_SRCS := $(wildcard src/kernel/*.c)
ASM_SRCS := $(wildcard src/*.s)
SRC := $(ASM_SRCS) $(KERNEL_SRCS)

OBJDIR := build
OBJS := $(patsubst src/%.c,$(OBJDIR)/%.o,$(filter %.c,$(SRC))) \
        $(patsubst src/%.s,$(OBJDIR)/%.o,$(filter %.s,$(SRC)))

BIN := os.bin
ISO := os.iso
ISODIR := isodir
BOOTDIR := $(ISODIR)/boot

.PHONY: all build iso run clean

all: iso

build: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ -lgcc

$(BOOTDIR)/$(BIN): $(BIN) | $(BOOTDIR)
	cp $< $@

$(BOOTDIR):
	mkdir -p $@

$(OBJDIR)/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: src/%.s
	mkdir -p $(dir $@)
	$(AS) $< -o $@

iso: $(BOOTDIR)/$(BIN)
	$(GRUB_MKRESCUE) -o $(ISO) $(ISODIR)

run: iso
	$(QEMU) -cdrom $(ISO)

clean:
	rm -rf $(OBJDIR) $(BIN) $(ISO) 
	rm -f $(ISODIR)/$(BIN)