B='''                weighted=model->power[i]*D_8011F060[i];
                value=weighted-D_8011F060[i]/2.0f+1;
                style=style<value?value:style;
                value=weighted*0.6f;
                level=level<value?value:level;'''
def r(x): return [(B,x)]
V={
 'base':[],
 'noweighted':r('''                value=model->power[i]*D_8011F060[i]-D_8011F060[i]/2.0f+1;
                style=style<value?value:style;
                value=model->power[i]*D_8011F060[i]*0.6f;
                level=level<value?value:level;'''),
 'v_split':r('''                weighted=model->power[i]*D_8011F060[i];
                value=weighted;
                value-=D_8011F060[i]/2.0f;
                value+=1;
                style=style<value?value:style;
                value=weighted*0.6f;
                level=level<value?value:level;'''),
 'v2':r('''                weighted=model->power[i]*D_8011F060[i];
                value=weighted-D_8011F060[i]/2.0f+1;
                style=style<value?value:style;
                weighted*=0.6f;
                level=level<weighted?weighted:level;'''),
 'vinit':[('        style=0.5f; level=0.0f;\n','        value=0; style=0.5f; level=0.0f;\n')],
 'w_split':r('''                weighted=model->power[i];
                weighted*=D_8011F060[i];
                value=weighted-D_8011F060[i]/2.0f+1;
                style=style<value?value:style;
                value=weighted*0.6f;
                level=level<value?value:level;'''),
}
