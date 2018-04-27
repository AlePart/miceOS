.data

.section .text
.global kmemset


.type kmemset, @function

kmemset:

    pushl	%ebp		# Salva il puntatore al fram
    movl	%esp,%ebp	# punta al nuovo frame
    mov         0x10(%ebp),%ecx
    #cld
    lea 0x0c(%ebp), %esi
    lea (%eax), %edi
    #rep movsb
loop:
    mov %esi,%edi
    add 0x04, %edi
    cmp 0x00, %ecx
    dec %ecx
    jnz loop
    popl %ebp
    ret
