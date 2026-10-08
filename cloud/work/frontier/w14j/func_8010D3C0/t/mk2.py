s=open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w14j/func_8010D3C0/rs/w7b_best.c').read()
def rep(old,new):
    global s
    assert s.count(old)==1, (s.count(old), old)
    s=s.replace(old,new)
VOL='*(volatile s32 *)&D_80121D64'
PL='default: kind=1; amount=D_80121D64; break;'
# start block (opt 2 = plain start, opt 3 = volatile start); end block (opt 0 plain, 1 volatile)
rep("        case 350:\n            kind=0;",
    "        /*@{dp*/ /*@| @| default: kind=1; amount=D_80121D64; break; @| default: kind=1; amount="+VOL+"; break; @}*/\n        case 350:\n            kind=0;")
rep("        default:\n            kind=1;\n            amount=D_80121D64;\n            break;\n        }",
    "        /*@{dp*/default: kind=1; amount=D_80121D64; break;/*@| @| @| @}*/\n        }")
rep("    s32 kind,amount;","    /*@{kdecl*/s32 kind,amount;/*@| s32 amount,kind; @}*/")
rep("            kind=1;\n            amount=D_80121D64;\n            break;\n        case 352:",
    "            /*@{k351*/kind=1;/*@| kind=1U; @}*/\n            amount=D_80121D64;\n            break;\n        case 352:")
rep("        if(amount) {}\n        if(amount) {}\n","        if(amount) {}\n        if(amount) {}\n")
open('/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w14j/func_8010D3C0/t/t2.c','w').write(s)
