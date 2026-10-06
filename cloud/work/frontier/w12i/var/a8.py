D='''    s32 d1,d2,d3;
'''
S='''        style=0.5f; level=value=0.0f;
'''
F='''        for(i=0;i<4;i++) {'''
ND=(D,'''    s32 n;
    s32 d2,d3;
''')
V={
 'm1':[ND,(S,S+'        n=4;\n'),(F,'''        for(i=0;i!=n;i++) {''')],
 'm2':[ND,(F,'''        n=4;
        for(i=0;i!=n;i++) {''')],
 'm3':[ND,(F,'''        for(i=0,n=4;i!=n;i++) {''')],
 'm4':[ND,(F,'''        for(n=4,i=0;i<n;i++) {''')],
 'm5':[ND,('''    if (!D_8010FFC0) return;''','''    n=4;
    if (!D_8010FFC0) return;'''),(F,'''        for(i=0;i<n;i++) {''')],
}
