K=('    f32 k;\n','')
S2='            pitch = (rpm - t->b1) / (t->b2 - t->b1);'
C3='''        } else if (rpm < t->b2) {
            pitch = (rpm - t->b1) / (t->b2 - t->b1);
            pitch = (t->p2 - t->p1) * pitch + t->p1;
        } else {
            pitch = t->p2;
        }'''
C3b='''        } else {
            if (rpm < t->b1) {}
            if (rpm < t->b2) {
                pitch = (rpm - t->b1) / (t->b2 - t->b1);
                pitch = (t->p2 - t->p1) * pitch + t->p1;
            } else {
                pitch = t->p2;
            }
        }'''
C3c=C3b.replace('            if (rpm < t->b1) {}\n','').replace('''                pitch = t->p2;
            }''','''                pitch = t->p2;
            }
            if (rpm < t->b1) {}''')
C3d=C3.replace('''            pitch = t->p2;
        }''','''            if (rpm < t->b1) {}
            pitch = t->p2;
        }''')
V={
 'e_lt_s2b':[K,(S2,S2+'\n            if (rpm < t->b1) {}')],
 'c3b':[K,(C3,C3b)],
 'c3c':[K,(C3,C3c)],
 'c3d':[K,(C3,C3d)],
}
