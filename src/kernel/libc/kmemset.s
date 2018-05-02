.data

.section .text
.global kmemset


.type kmemset, @function

kmemset:

    pushl	%ebp		# Save old frame ptr
    movl	%esp,%ebp	# punta al nuovo framestack is saved on base pointer
    cld                         # clear direction esi and edi increment
    mov 0x10(%ebp), %ecx        # getting counter
    lea 0x0c(%ebp), %esi        # getting source
    lea (%eax), %edi            # getting dest
    cmp %ecx,%ecx               # reset ZF


copy:
    movsb                       # copy 1 byte
    dec %esi                    # return to the symbol
    cmp 0x01, %ecx              # if last copy
    dec %ecx                    # counter --
    jnz copy                    # copy another byte
    popl %ebp
    ret



