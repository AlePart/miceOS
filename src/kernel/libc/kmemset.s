.data

.section .text
.global kmemset


.type kmemset, @function

kmemset:

    pushl	%ebp		# Salva il puntatore al fram
    movl	%esp,%ebp	# punta al nuovo frame
    mov         %ebp,%eax
    add         4,%eax
    mov         %eax,%ebx
    add         4,%eax
    mov         %eax,%edx
    add         4,%eax
    mov         %eax,%ecx


    cld
    mov (%ebx), %esi
    mov (%eax), %edi
    rep movsb
    popl %ebp
    ret
