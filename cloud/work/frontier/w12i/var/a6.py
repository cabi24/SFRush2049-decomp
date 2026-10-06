D='''    s32 d1,d2,d3;
'''
S='''        style=0.5f; level=value=0.0f;
'''
B='''                weighted=model->power[i]*D_8011F060[i];
                value=weighted-D_8011F060[i]/2.0f+1;'''
B2='''                weighted=model->power[i]*tbl[i];
                value=weighted-tbl[i]/2.0f+1;'''
V={
 'p1':[(D,'''    f32 *tbl;
    s32 d2,d3;
'''),(S,S+'        tbl=D_8011F060;\n'),(B,B2)],
 'p2':[(D,'''    s32 d2,d3;
    f32 *tbl;
'''),(S,S+'        tbl=D_8011F060;\n'),(B,B2)],
 'p3':[(D,'''    f32 *tbl;
    s32 d2,d3;
'''),(S,'        tbl=D_8011F060;\n'+S),(B,B2)],
 'p4':[(D,'''    f32 *tbl;
    s32 d2,d3;
'''),('''    slot=model->index;
''','''    slot=model->index;
    tbl=D_8011F060;
'''),(B,B2)],
}
