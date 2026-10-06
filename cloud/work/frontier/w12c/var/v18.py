B='''                weighted=D_8011F060[i]*model->power[i];
                value=weighted-D_8011F060[i]/2.0f+1;
'''
V={
 'wv':[('f32 weighted,value;','f32 weighted,value,w;'),(B,'''                w=D_8011F060[i];
                weighted=w*model->power[i];
                value=weighted-w/2.0f+1;
''')],
 'wv2':[('f32 weighted,value;','f32 w,weighted,value;'),(B,'''                w=D_8011F060[i];
                weighted=w*model->power[i];
                value=weighted-w/2.0f+1;
''')],
 'pv':[('f32 weighted,value;','f32 weighted,value,p;'),(B,'''                p=model->power[i];
                weighted=D_8011F060[i]*p;
                value=weighted-D_8011F060[i]/2.0f+1;
''')],
 'halfsub':[(B,'''                weighted=D_8011F060[i]*model->power[i];
                value=1+weighted-D_8011F060[i]/2.0f;
''')],
}
