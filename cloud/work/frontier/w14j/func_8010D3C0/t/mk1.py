import sys
s=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w14j/func_8010D3C0/rs/w7b_best.c').read()
def rep(old,new):
    global s
    assert s.count(old)==1, (s.count(old), old)
    s=s.replace(old,new)
PL='default: kind=1; amount=D_80121D64; break;'
VO='default: kind=1; amount=*(volatile s32 *)&D_80121D64; break;'
# dp (4 options): end block gets plain / volatile / none / none; start block gets none / none / plain / volatile
rep("        case 350:\n            kind=0;",
    "        /*@{dp*/ /*@| @| "+PL+" @| "+VO+" @}*/\n        case 350:\n            kind=0;")
rep("        default:\n            kind=1;\n            amount=D_80121D64;\n            break;\n        }",
    "        /*@{dp*/"+PL+"/*@| "+VO+" @| @| @}*/\n        }")
rep("        if(amount) {}\n        if(amount) {}\n",
    "        /*@{shp*/if(amount) {}\n        if(amount) {}\n/*@| @}*/")
rep("    s32 kind,amount;","    /*@{kdecl*/s32 kind,amount;/*@| s32 amount,kind; @}*/")
rep("if(car->kind==kind) car->amount+=amount;","/*@{cmp*/if(car->kind==kind) car->amount+=amount;/*@| if(kind==car->kind) car->amount+=amount; @}*/")
rep("            kind=1;\n            amount=D_80121D64;\n            break;\n        case 352:",
    "            kind=1;\n            amount=/*@{v351*/D_80121D64/*@| *(volatile s32 *)&D_80121D64 @}*/;\n            break;\n        case 352:")
open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w14j/func_8010D3C0/t/t1.c','w').write(s)
