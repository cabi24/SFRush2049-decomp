L='''                level=level<value?value:level;
            }
        }
'''
F='''        for(i=0;i<4;i++) {'''
V={
 'a1':[(L,L+'        if (i < 4) {}\n')],
 'a2':[(L,L+'        if (i != 4) {}\n')],
 'a3':[(F,'''        i=0;
        do {'''),(L,'''                level=level<value?value:level;
            }
            i++;
        } while(i<4);
''')],
 'a4':[(F,'''        for(i=0;i<4;) {'''),(L,'''                level=level<value?value:level;
            }
            i++;
        }
''')],
}
