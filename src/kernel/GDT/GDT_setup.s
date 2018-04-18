
.section .data
GDT_contents:
.byte 0, 0, 0, 0, 0, 0, 0, 0  		#null selector
.byte 255, 255, 0, 0, 0, 0x9A, 0xCF, 0	#KM Code Selector
.byte 255, 255, 0, 0, 0, 0x92, 0xCF, 0	#KM Data Selector
.byte 255, 255, 0, 0, 0, 0xFA, 0xCF, 0	#UM Code Selector
.byte 255, 255, 0, 0, 0, 0xF2, 0xCF, 0	#UM Data Selector
.byte 0x67,  0, 0, 0, 0, 0xE9, 0x00, 0  #TSS Selector

.section .text

