== func_800AB750 @ 0x800AB750 (32 words)
  800ab750: lui	t7,0x8015
  800ab754: lw	t7,3896(t7)
  800ab758: addiu	sp,sp,-32
  800ab75c: sll	t6,a0,0x6
  800ab760: sw	ra,20(sp)
  800ab764: move	t0,a1
  800ab768: addu	v0,t6,t7
  800ab76c: addiu	a1,v0,4
  800ab770: sw	v0,28(sp)
  800ab774: sw	t0,36(sp)
  800ab778: move	a0,a3
  800ab77c: jal	0x8008d6b0   <math_utility>
  800ab780: sw	a2,40(sp)
  800ab784: lw	a2,40(sp)
  800ab788: lw	v0,28(sp)
  800ab78c: lw	t0,36(sp)
  800ab790: lwc1	$f4,0(a2)
  800ab794: swc1	$f4,40(v0)
  800ab798: lwc1	$f6,4(a2)
  800ab79c: swc1	$f6,44(v0)
  800ab7a0: lwc1	$f8,8(a2)
  800ab7a4: swc1	$f8,48(v0)
  800ab7a8: lwc1	$f10,0(t0)
  800ab7ac: swc1	$f10,52(v0)
  800ab7b0: lwc1	$f16,4(t0)
  800ab7b4: swc1	$f16,56(v0)
  800ab7b8: lwc1	$f18,8(t0)
  800ab7bc: swc1	$f18,60(v0)
  800ab7c0: lw	ra,20(sp)
  800ab7c4: addiu	sp,sp,32
  800ab7c8: jr	ra
  800ab7cc: nop
