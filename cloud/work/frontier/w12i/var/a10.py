D='''    s32 d1,d2,d3;
'''
F='''        for(i=0;i<4;i++) {
            if(model->contact[i]==0) {'''
L='''                level=level<value?value:level;
            }
        }
'''
ND=(D,'''    s32 n;
    s32 d2,d3;
''')
V={
 'r1':[ND,(F,'''        i=0;
        do {
            n=4;
            if(model->contact[i]==0) {'''),(L,'''                level=level<value?value:level;
            }
        } while(++i!=n);
''')],
 'r2':[ND,(F,'''        for(i=0;;) {
            n=4;
            if(model->contact[i]==0) {'''),(L,'''                level=level<value?value:level;
            }
            if(++i==n) break;
        }
''')],
}
