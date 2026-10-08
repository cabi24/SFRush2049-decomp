s=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w14j/func_8010D3C0/rs/w7b_best.c').read()
def rep(old,new):
    global s
    assert s.count(old)==1, (s.count(old), old)
    s=s.replace(old,new)
VOL='*(volatile s32 *)&D_80121D64'
rep("        case 350:\n            kind=0;",
    "        /*@{dp*/ /*@| @| default: kind=1; amount=D_80121D64; break; @| default: kind=1; amount="+VOL+"; break; @}*/\n        case 350:\n            kind=0;")
rep("        default:\n            kind=1;\n            amount=D_80121D64;\n            break;\n        }",
    "        /*@{dp*/default: kind=1; amount=D_80121D64; break;/*@| @| @| @}*/\n        }")
rep("            kind=1;\n            amount=D_80121D64;\n            break;\n        case 352:",
    "            /*@{k351*/kind=1;/*@| kind=1U; @}*/\n            amount=D_80121D64;\n            break;\n        case 352:")
rep("        if(amount) {}\n        if(amount) {}\n","        /*@{shp*/if(amount) {}\n        if(amount) {}\n/*@| @}*/")
rep("    Car952 *car;\n    Vehicle2056 *vehicle;\n    s32 kind,amount;\n    Limits16 *limits;\n    f32 maximum;",
    "    /*@{dd*/Car952 *car;\n    Vehicle2056 *vehicle;/*@| Vehicle2056 *vehicle;\n    Car952 *car; @}*/\n    /*@{kdecl*/s32 kind,amount;/*@| s32 amount,kind; @}*/\n    Limits16 *limits;\n    f32 maximum;")
rep("        effect->flags&=~6;","        /*@{f6*/effect->flags&=~6;/*@| effect->flags=effect->flags&~6; @}*/")
rep("        if(car->flags&16) {","        /*@{c16*/if(car->flags&16) {/*@| if((car->flags&16)!=0) { @}*/")
rep("        car=&D_80152818[effect->owner];\n        vehicle=&D_8014A250[effect->owner];",
    "        /*@{cv*/car=&D_80152818[effect->owner];\n        vehicle=&D_8014A250[effect->owner];/*@| car=D_80152818+effect->owner;\n        vehicle=D_8014A250+effect->owner; @}*/")
rep("        effect->flags&=~2;\n        effect->flags|=32;",
    "        /*@{ff*/effect->flags&=~2;\n        effect->flags|=32;/*@| effect->flags|=32;\n        effect->flags&=~2; @}*/")
rep("            car->extra_flags|=1;\n            car->state=1;",
    "            /*@{e354*/car->extra_flags|=1;\n            car->state=1;/*@| car->state=1;\n            car->extra_flags|=1; @}*/")
open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w14j/func_8010D3C0/t/t3.c','w').write(s)
