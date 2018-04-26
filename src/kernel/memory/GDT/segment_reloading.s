.data

.section .text
.global reload_data
.global .reloadCS

.type reload_data, @function

reload_data:
push %ax
mov $0x10, %ax
mov %ax, %ds
mov %ax, %es
mov %ax, %fs
mov %ax, %gs
mov %ax, %ss
pop %ax
jmp .reloadCS
rejump:
ret



.reloadCS:
jmp rejump
