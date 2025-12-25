export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

i686-elf-as src/boot.s -o boot.o
i686-elf-gcc -c src/kernel/kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -c src/kernel/terminal.c -o terminal.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -T src/linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o terminal.o -lgcc
cp myos.bin isodir/boot/
grub-mkrescue -o myos.iso isodir

qemu-system-i386 -cdrom myos.iso
