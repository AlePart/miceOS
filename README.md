# miceOS
Scratch your own itch project for understandind how OS are made.

## Building (experimental)
A premade docker image with a complete development environment is available.
The image contains the following components:
* QtCreator 4.6
* i686-elf toolchain
* cmake 3.11.0
* qemu
* Build & Run already configured with qemu

For using just follow these steps:
```
cd /path/to/miceos/repository
./run-ide.sh
```

If everythings works ok a QtCreator window should pop up ready for developing the project 

![Alt Text](https://raw.githubusercontent.com/AlePart/miceOS/master/doc/run_ide_example.gif)


