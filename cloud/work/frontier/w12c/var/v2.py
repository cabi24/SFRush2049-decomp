K=('    f32 k;\n','')
S2='            pitch = (rpm - t->b1) / (t->b2 - t->b1);'
S1='            pitch = (rpm - t->b0) / (t->b1 - t->b0);'
V={
 'e_lt_s2':[K,(S2,'            if (rpm < t->b1) {}\n'+S2)],
 'e_lt_s2b':[K,(S2,S2+'\n            if (rpm < t->b1) {}')],
 'e_f_s2':[K,(S2,'            if ((f32)t->b1) {}\n'+S2)],
 'e_f_s1':[K,(S1,'            if ((f32)t->b1) {}\n'+S1)],
 'e_lt_s1':[K,(S1,'            if (rpm < t->b1) {}\n'+S1)],
 'e_eq_s2':[K,(S2,'            if (rpm == t->b1) {}\n'+S2)],
}
