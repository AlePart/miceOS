.data

.section .text
.global kmemcpy


.type kmemcpy, @function


kmemcpy:
pushl	%ebp		# Save old frame ptr
movl	%esp,%ebp	# punta al nuovo framestack is saved on base pointer
cld                         # clear direction esi and edi increment
mov 0x10(%ebp), %ecx        # getting counter
mov 0x0c(%ebp), %esi        # getting source
mov 0x08(%ebp), %edi        # getting dest
rep movsb
popl %ebp
ret
