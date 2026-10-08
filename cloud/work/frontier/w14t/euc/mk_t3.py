src=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w14t/euc/cand/w6d.c').read()
head_end = src.index("void entity_update_callback(")
pre, fn = src[:head_end], src[head_end:]
def R(a,b):
    global fn
    assert fn.count(a)==1, (fn.count(a), a[:70])
    fn=fn.replace(a,b)
R("    char buf[16];\n    u8 color[4];\n    f32 dx, dz;\n    char buf2[80];\n    s32 i;\n    Piece *p;\n",
  "    /*@{decl*/char buf[16];\n    u8 color[4];\n    f32 dx, dz;\n    char buf2[80];\n    s32 i;\n    Piece *p;/*@| char buf[16];\n    u8 color[4];\n    f32 dx, dz;\n    char buf2[80];\n    Piece *p;\n    s32 i; @}*/\n")
R("    if (obj == 0) {}\n", "    /*@{e1*/if (obj == 0) {}/*@| @| if (mode == 0) {} @}*/\n")
R("    if (D_80154660[obj->car].timer <= 0.0f) {\n",
  "    if (/*@{cm*/D_80154660[obj->car].timer <= 0.0f/*@| 0.0f >= D_80154660[obj->car].timer @}*/) {\n")
R("    if (active_player_count < 4 && (D_80156994 != 0 || D_8014978C >= 6)) {\n",
  "    if (/*@{op*/active_player_count < 4 && (D_80156994 != 0 || D_8014978C >= 6)/*@| (D_80156994 != 0 || D_8014978C >= 6) && active_player_count < 4 @}*/) {\n")
R("        D_8012E700[(s16)D_80154660[obj->car].glow].color = *(u32 *)color;\n",
  "        D_8012E700[/*@{gc*/(s16)D_80154660[obj->car].glow/*@| D_80154660[obj->car].glow @}*/].color = *(u32 *)color;\n")
R("            dz = dx = p->height * 0.5f;\n",
  "            /*@{dz*/dz = dx = p->height * 0.5f;/*@| dx = dz = p->height * 0.5f; @}*/\n")
R("        p->scale += p->dscale;\n        p->dscale -= 0.002f;\n",
  "        /*@{sc*/p->scale += p->dscale;/*@| p->scale = p->scale + p->dscale; @}*/\n        p->dscale -= 0.002f;\n")
R("    D_80154660[obj->car].alpha -= 31;\n", "    /*@{al*/D_80154660[obj->car].alpha -= 31;/*@| D_80154660[obj->car].alpha = D_80154660[obj->car].alpha - 31; @}*/\n")
R("    D_80154660[obj->car].phase++;\n", "    /*@{ph*/D_80154660[obj->car].phase++;/*@| D_80154660[obj->car].phase = D_80154660[obj->car].phase + 1; @}*/\n")
R("        p->height += 0.2f;\n", "        /*@{hg*/p->height += 0.2f;/*@| p->height = p->height + 0.2f; @}*/\n")
R("        if (D_80154660[obj->car].life < 0.2f) {\n", "        if (/*@{lt*/D_80154660[obj->car].life < 0.2f/*@| 0.2f > D_80154660[obj->car].life @}*/) {\n")
R("        if (D_8012E700[D_80154660[obj->car].glow].glow < 1.0f)\n", "        if (/*@{gl*/D_8012E700[D_80154660[obj->car].glow].glow < 1.0f/*@| 1.0f > D_8012E700[D_80154660[obj->car].glow].glow @}*/)\n")
# car local (linked 'cr'): option 1 declares s16 car and loads it at entry
R("void entity_update_callback(Obj *obj, s16 mode) {\n", "void entity_update_callback(Obj *obj, s16 mode) {\n")
fn = fn.replace("    /*@{decl*/", "    /*@{cr*/ /*@| s16 car; @}*/\n    /*@{decl*/", 1)
R("    if (D_801170FC != 0)\n", "    /*@{cr*/ /*@| car = @@CARINIT@@; @}*/\n    if (D_801170FC != 0)\n")
parts = fn.split("obj->car")
out = parts[0]
for part in parts[1:]:
    out += "/*@{cr*/obj->car/*@| car @}*/" + part
fn = out.replace("@@CARINIT@@", "obj->car")
open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w14t/euc/t3.c','w').write(pre+fn)
print("ok")
