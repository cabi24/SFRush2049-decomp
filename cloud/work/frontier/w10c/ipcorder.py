import sys, re
# usage: ipcorder.py ORDER(comma list of names) BODY OUTBODY OUTPART OUTHDR
order=sys.argv[1].split(',')
decl={'p1':'f32 *p1','p2':'f32 *p2','out':'f32 *out','poly':'Poly *poly','outIdx':'s16 *outIdx','flag':'s32 flag','vcOut':'f32 *vcOut','mat':'f32 *mat','rad2':'f32 rad2'}
arg={'p1':'cur','p2':'end','out':'np','poly':'poly','outIdx':'(s16 *)&idx','flag':'0','vcOut':'vc','mat':'(f32 *)m','rad2':'radius * radius'}
old='f32 *p1, f32 *p2, f32 *out, Poly *poly, s16 *outIdx, s32 flag, f32 *vcOut, f32 *mat, f32 rad2'
new=', '.join(decl[o] for o in order)
oldcall='input_process_controller(cur, end, np, poly, (s16 *)&idx, 0, vc, (f32 *)m, radius * radius)'
newcall='input_process_controller('+', '.join(arg[o] for o in order)+')'
b=open(sys.argv[2]).read(); assert oldcall in b
open(sys.argv[3],'w').write(b.replace(oldcall,newcall))
p=open('w1g_part2.c').read(); assert p.count(old)==2
open(sys.argv[4],'w').write(p.replace(old,new))
h=open('hdr2.h').read(); assert old in h
open(sys.argv[5],'w').write(h.replace(old,new))
