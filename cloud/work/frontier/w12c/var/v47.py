B='''                weighted=model->power[i]*D_8011F060[i];
                value=weighted-D_8011F060[i]/2.0f+1;'''
def r(x): return [(B,x)]
V={
 'a':r('''                value=model->power[i]*D_8011F060[i];
                weighted=value;
                value=value-D_8011F060[i]/2.0f+1;'''),
 'b':r('''                weighted=value=model->power[i]*D_8011F060[i];
                value=value-D_8011F060[i]/2.0f+1;'''),
 'c':r('''                value=(weighted=model->power[i]*D_8011F060[i])-D_8011F060[i]/2.0f+1;'''),
 'd':r('''                value=weighted=model->power[i]*D_8011F060[i];
                value=value-D_8011F060[i]/2.0f+1;'''),
}
