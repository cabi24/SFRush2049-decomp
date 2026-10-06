L='''                level=level<value?value:level;
            }
        }
'''
F='''        for(i=0;i<4;i++) {'''
V={
 'w1':[(F,'''        i=0;
        while(i<4) {'''),(L,'''                level=level<value?value:level;
            }
            i++;
        }
''')],
 'w2':[(F,'''        for(i=0;4>i;i++) {''')],
}
