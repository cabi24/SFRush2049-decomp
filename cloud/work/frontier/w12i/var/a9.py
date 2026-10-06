D='''    s32 d1,d2,d3;
'''
S='''        style=0.5f; level=value=0.0f;
'''
F='''        for(i=0;i<4;i++) {'''
ND=(D,'''    s32 n;
    s32 d2,d3;
''')
V={
 'q1':[ND,(F,'''        for(n=4,i=0;i!=n;i++) {''')],
 'q2':[ND,(S,'''        style=0.5f; n=4; level=value=0.0f;
''') ,(F,'''        for(i=0;i!=n;i++) {''')],
 'q3':[ND,(S,'''        n=4; style=0.5f; level=value=0.0f;
''') ,(F,'''        for(i=0;i!=n;i++) {''')],
 'q4':[ND,(F,'''        for(i=0,n=4;i<n;i++) {''')],
}
