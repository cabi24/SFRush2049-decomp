V1='                value=weighted-D_8011F060[i]/2.0f+1;\n                style=style<value?value:style;\n'
W='                weighted=D_8011F060[i]*model->power[i];\n'
V={
 'ifv_after':[(V1,V1+'                if(value){}\n')],
 'ifv_mid':[(V1,'                value=weighted-D_8011F060[i]/2.0f+1;\n                if(value){}\n                style=style<value?value:style;\n')],
 'split_v':[(V1,'                value=weighted-D_8011F060[i]/2.0f;\n                value+=1;\n                style=style<value?value:style;\n')],
 'split_w':[(W,'                weighted=D_8011F060[i];\n                weighted*=model->power[i];\n')],
 'vfirst':[('                weighted=D_8011F060[i]*model->power[i];\n                value=weighted-D_8011F060[i]/2.0f+1;\n','                value=0;\n                weighted=D_8011F060[i]*model->power[i];\n                value=weighted-D_8011F060[i]/2.0f+1;\n')],
}
