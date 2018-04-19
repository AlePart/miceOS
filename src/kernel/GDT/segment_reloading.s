.data
.text
.globl reload_data
.globl reloadCS

.reload_data:
push %ax
mov $0x10, %ax
mov %ax, %ds
mov %ax, %es
mov %ax, %fs
mov %ax, %gs
mov %ax, %ss
pop %ax
ret



reloadCS:
jmp reload_data
