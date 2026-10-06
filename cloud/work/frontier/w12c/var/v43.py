F='''        for(i=0;i<4;i++) {
            if(model->contact[i]==0) {
                weighted=D_8011F060[i]*model->power[i];
                value=weighted-D_8011F060[i]/2.0f+1;
                style=style<value?value:style;
                value=weighted*0.6f;
                level=level<value?value:level;
            }
        }'''
B='''            if(model->contact[i]==0) {
                weighted=D_8011F060[i]*model->power[i];
                value=weighted-D_8011F060[i]/2.0f+1;
                style=style<value?value:style;
                value=weighted*0.6f;
                level=level<value?value:level;
            }'''
V={
 'while':[(F,'        i=0;\n        while(i<4) {\n'+B+'\n            i++;\n        }')],
 'dowhile':[(F,'        i=0;\n        do {\n'+B+'\n        } while(++i<4);')],
 'ne4':[(F,F.replace('i<4;','i!=4;'))],
 'le3':[(F,F.replace('i<4;','i<=3;'))],
}
