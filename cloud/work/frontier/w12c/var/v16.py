W='                weighted=D_8011F060[i]*model->power[i];\n'
V1='                value=weighted-D_8011F060[i]/2.0f+1;\n                style=style<value?value:style;\n'
V2='                value=weighted*0.6f;\n                level=level<value?value:level;\n'
V={
 'noweighted':[(W,''),(V1,'                value=D_8011F060[i]*model->power[i]-D_8011F060[i]/2.0f+1;\n                style=style<value?value:style;\n'),(V2,'                value=D_8011F060[i]*model->power[i]*0.6f;\n                level=level<value?value:level;\n')],
 'novalue':[(V1,'                style=style<weighted-D_8011F060[i]/2.0f+1?weighted-D_8011F060[i]/2.0f+1:style;\n'),(V2,'                level=level<weighted*0.6f?weighted*0.6f:level;\n')],
 'val2':[('f32 weighted,value;','f32 weighted,value,value2;'),(V2,'                value2=weighted*0.6f;\n                level=level<value2?value2:level;\n')],
 'declvw':[('f32 weighted,value;','f32 value,weighted;')],
 'pw':[(W,'                weighted=model->power[i]*D_8011F060[i];\n')],
 'pw_d':[(W,'                weighted=model->power[i]*D_8011F060[i];\n'),('f32 weighted,value;','f32 value,weighted;')],
}
