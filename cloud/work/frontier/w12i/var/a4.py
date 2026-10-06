F='''        for(i=0;i<4;i++) {
            if(model->contact[i]==0) {'''
V={
 'b1':[(F,'''        for(i=0;;i++) {
            if(i>=4) break;
            if(model->contact[i]==0) {''')],
 'b2':[(F,'''        i=0;
        top:
        if(i<4) {
            if(model->contact[i]==0) {'''),('''                level=level<value?value:level;
            }
        }
''','''                level=level<value?value:level;
            }
            i++;
            goto top;
        }
''')],
}
