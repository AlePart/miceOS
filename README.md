# miceOS
scratch your own itch project.
Understandind how OS are made.

Use script in utils in order to download toolchain

toolchain path must be $HOME/opt/cross/i686-elf-tools-linux/bin
in order to use compile script

In order to clean build dir compile and execute you could youse thi cmd line
rm -rf build && ./compile.sh &&  qemu-system-i386 -cdrom build/miceOS.iso 
