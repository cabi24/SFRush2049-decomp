L=('''                value=weighted*D_8012438C;
                level=level<value?value:level;''','''                level=level<weighted*D_8012438C?weighted*D_8012438C:level;''')
S=('''                value=weighted-D_8011F060[i]*0.5f+1.0f;
                style=style<value?value:style;''','''                style=style<weighted-D_8011F060[i]*0.5f+1.0f?weighted-D_8011F060[i]*0.5f+1.0f:style;''')
L2=('''                value=weighted*D_8012438C;
                level=level<value?value:level;''','''                if(level<weighted*D_8012438C)level=weighted*D_8012438C;''')
V={'L':[L],'LS':[L,S],'L2':[L2]}
