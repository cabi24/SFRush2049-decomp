
/home/cburnes/.conveyor/blobs/1c220b7382b3db545dcfabba77fd3e4569248a6bdc5f2cdb2ffe4b84e89de9d3:     file format elf32-tradbigmips


Disassembly of section .text:

00000000 <dll_get_priority>:
   0:	14800003 	bnez	a0,10 <dll_get_priority+0x10>
   4:	00000000 	nop
   8:	3c040000 	lui	a0,0x0
			8: R_MIPS_HI16	__osRunningThread
   c:	8c840000 	lw	a0,0(a0)
			c: R_MIPS_LO16	__osRunningThread
  10:	03e00008 	jr	ra
  14:	8c820004 	lw	v0,4(a0)
	...
