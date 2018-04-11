export PATH="$HOME/opt/cross/i686-elf-tools-linux/bin:$PATH"
PWD=`pwd`
BUILD_DIR="$PWD/build"
SRC_DIR="$PWD/src"
BOOT_DIR="$SRC_DIR/boot"
GRUB_DIR="$SRC_DIR/grub"
GRUB_BUILD="$BUILD_DIR/iso/boot/grub"
mkdir $BUILD_DIR

i686-elf-as $BOOT_DIR/boot.s -o $BUILD_DIR/boot.o
i686-elf-gcc -c $SRC_DIR/kernel.c -o $BUILD_DIR/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -T $SRC_DIR/linker.ld -o $BUILD_DIR/miceOS.bin -ffreestanding -O2 -nostdlib $BUILD_DIR/boot.o $BUILD_DIR/kernel.o -lgcc
grub-file --is-x86-multiboot $BUILD_DIR/miceOS.bin
if grub-file --is-x86-multiboot $BUILD_DIR/miceOS.bin; then
  echo multiboot confirmed
else
  echo the file is not multiboot
fi


mkdir -p "$GRUB_BUILD"
cp $BUILD_DIR/miceOS.bin $GRUB_BUILD/../myos.bin
cp $GRUB_DIR/grub.cfg $GRUB_BUILD/grub.cfg
grub-mkrescue -o $BUILD_DIR/miceOS.iso $BUILD_DIR/iso
