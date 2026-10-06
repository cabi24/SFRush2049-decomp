VAL='value=weighted-D_8011F060[i]*0.5f+1.0f;'
def v(x): return [(VAL,x)]
V={
 'i1':v('value=weighted-D_8011F060[i]*0.5f+1;'),
 'd2f':v('value=weighted-D_8011F060[i]/2.0f+1.0f;'),
 'd2':v('value=weighted-D_8011F060[i]/2+1.0f;'),
 'd2_i1':v('value=weighted-D_8011F060[i]/2+1;'),
 'd2f_i1':v('value=weighted-D_8011F060[i]/2.0f+1;'),
 'paren':v('value=(weighted-D_8011F060[i]*0.5f)+1.0f;'),
 'onefirst':v('value=1.0f+(weighted-D_8011F060[i]*0.5f);'),
 'onefirst_i':v('value=1+(weighted-D_8011F060[i]*0.5f);'),
 'factor':v('value=(model->power[i]-0.5f)*D_8011F060[i]+1.0f;'),
}
