import sys
HDR='''/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
extern s16 D_80151AD0;
extern s8 D_80143F54[];
extern s32 D_8014A110;
extern s16 D_8014A108;
extern u8 D_8014A250[];
extern u8 D_80152038[];
extern u8 D_80152818[];
extern s8 D_8012E77C[];
extern s8 D_8015256C[];
extern s32 D_80150B60;
extern s8 D_80150B68[];
#define IDX(a) (*(s16 *)(D_8014A250 + (a) * 2056 + 1990))
#define K4(i) (*(s32 *)(D_80152038 + (i) * 120 + 20))
#define K6(i) (*(s8 *)(D_80152818 + (i) * 952 + 931))
#define K6B(i) D_8015256C[D_8012E77C[i]]
#define K0(i) (*(s8 *)(D_80152818 + (i) * 952 + 238))
'''
def sort(K, opt):
    if opt['sty']=='idx':
        cmp="""        a = D_80143F54[j];
        b = D_80143F54[j + 1];
        ia = IDX(a);
        ib = IDX(b);
        if (%s(ia) < %s(ib)) {
          D_80143F54[j + 1] = a;
          D_80143F54[j] = b;
        }
"""%(K,K)
    else:
        cmp="""        a = D_80143F54[j];
        b = D_80143F54[j + 1];
        if (%s(IDX(a)) < %s(IDX(b))) {
          D_80143F54[j + 1] = a;
          D_80143F54[j] = b;
        }
"""%(K,K)
    return """    for (i = 0; i < D_8014A108 - 1; i++) {
      for (j = 0; j < D_8014A108 - 1; j++) {
%s      }
    }
"""%cmp
def tie(K):
    return """    for (i = 1; i < D_8014A108; i++) {
      if (%s(IDX(D_80143F54[0])) == %s(IDX(D_80143F54[i]))) {
        D_80150B68[i] = 1;
        D_80150B60++;
      }
    }
"""%(K,K)
def gen(opt):
    decl=opt.get('decl',['i','j','a','b','ia','ib'])
    T={'i':'s32','j':'s32','a':'s8','b':'s8','ia':'s16','ib':'s16'}
    d=''.join('  %s %s;\n'%(T[x],x) for x in decl)
    s=HDR+"void func_800F7F3C(void)\n{\n"+d+"""  for (i = 0; i < D_80151AD0; i++) {
    D_80143F54[i] = i;
  }
  if (D_8014A110 == 4) {
"""+sort('K4',opt)+tie('K4')+"  } else if (D_8014A110 == 6) {\n"+sort('K6',opt)+"    i = 0;\n"+sort('K6B',dict(opt,sty='plain')).replace('K6B(IDX(a))','K6B(IDX(a))')+tie('K6')+"  } else {\n"+sort('K0',opt)+tie('K0')+"  }\n}\n"
    return s
if __name__=='__main__':
    open(sys.argv[1],'w').write(gen(dict(sty=sys.argv[2])))
