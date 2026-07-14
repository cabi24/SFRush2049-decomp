
/home/cburnes/.conveyor/blobs/be02b8d59a43d9cd3245aad95193905fafbdcfce90575aad59de20188f905538:     file format elf32-tradbigmips


Disassembly of section .text:

00000000 <osDpIsBusy>:
   0:	3c0ea410 	lui	t6,0xa410
   4:	8dc2000c 	lw	v0,12(t6)
   8:	304f0100 	andi	t7,v0,0x100
   c:	11e00003 	beqz	t7,1c <osDpIsBusy+0x1c>
  10:	00001025 	move	v0,zero
  14:	03e00008 	jr	ra
  18:	24020001 	li	v0,1
  1c:	03e00008 	jr	ra
  20:	00000000 	nop
	...
