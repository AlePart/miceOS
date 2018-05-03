.data

.section .text
.global kmemcmp


.type kmemcmp, @function


kmemcmp:
pushl	%ebp                # Save old frame ptr
movl	%esp,%ebp           # framestack is saved on base pointer
pushl   %esi
pushl   %edi
movl    0x10(%ebp), %ecx    # getting counter
movl    0x0c(%ebp), %esi    # getting source
movl    0x08(%ebp), %edi    # getting dest
xor     %eax, %eax          # setting eax to 0
cld                         # clear direction
cmp     %ecx, %ecx          # verify size > 0
repe    cmpsb               # comparison
jnz     error               # if error
exit:
popl    %edi                # restore edi
popl    %esi                # restore esi
popl    %ebp                # restore old frame
ret


error:
movl    %esi, %eax          # move esi in eax
movl    0x0c(%ebp), %ebx    # move starting esi in ebx
sub     %ebx, %eax          # eax - ebx
jmp exit
