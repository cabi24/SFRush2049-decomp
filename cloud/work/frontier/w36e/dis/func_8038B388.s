== func_8038B388 @ 0x8038B388 (25 words, 100 B)
  8038b388: 27bdffe8 addiu	sp,sp,-24
  8038b38c: afbf0014 sw	ra,20(sp)
  8038b390: 0c0e2b94 jal	0x8038ae50   <func_8038AE50>
  8038b394: 00000000 nop
  8038b398: 3c11803c lui	s1,0x803c
  8038b39c: 26318284 addiu	s1,s1,-32124   ; 803B8284 [FFFFFFFF nan]
  8038b3a0: 8e220000 lw	v0,0(s1)   ; 803B8284 [FFFFFFFF nan]
  8038b3a4: 2410ffff li	s0,-1
  8038b3a8: 12020005 beq	s0,v0,0x8038b3c0
  8038b3ac: 00022400 sll	a0,v0,0x10
  8038b3b0: 00047403 sra	t6,a0,0x10
  8038b3b4: 0c024095 jal	0x80090254   <sound_call_minimal>
  8038b3b8: 01c02025 move	a0,t6
  8038b3bc: ae300000 sw	s0,0(s1)   ; 803B8284 [FFFFFFFF nan]
  8038b3c0: 3c028003 lui	v0,0x8003
  8038b3c4: 2442eb70 addiu	v0,v0,-5264   ; 8002EB70 D_8002EB70
  8038b3c8: 844f0000 lh	t7,0(v0)   ; 8002EB70 D_8002EB70
  8038b3cc: 8fbf0014 lw	ra,20(sp)
  8038b3d0: 11e00004 beqz	t7,0x8038b3e4
  8038b3d4: 00000000 nop
  8038b3d8: 84580000 lh	t8,0(v0)   ; 8002EB70 D_8002EB70
  8038b3dc: 5700ffff bnezl	t8,0x8038b3dc
  8038b3e0: 84580000 lh	t8,0(v0)   ; 8002EB70 D_8002EB70
  8038b3e4: 03e00008 jr	ra
  8038b3e8: 27bd0018 addiu	sp,sp,24
