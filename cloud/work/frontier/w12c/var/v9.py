INIT='        style=0.5f; level=0.0f;\n        for(i=0;i<4;i++) {'
END='''                if(level<value)level=value;
            }
        }'''
V={
 'lvl_first':[(INIT,'        level=0.0f; style=0.5f;\n        for(i=0;i<4;i++) {')],
 'dowhile':[(INIT,'        style=0.5f; level=0.0f;\n        i=0;\n        do {'),(END,'''                if(level<value)level=value;
            }
        } while(++i<4);''')],
 'i_first':[(INIT,'        i=0;\n        style=0.5f; level=0.0f;\n        for(;i<4;i++) {')],
 'ternary':[('if(style<value)style=value;','style=style<value?value:style;')],
 'ternary2':[('if(style<value)style=value;','style=style<value?value:style;'),('if(level<value)level=value;','level=level<value?value:level;')],
 'pre_mode':[('    if (MODE(model)==2) {\n        style=0.5f; level=0.0f;\n','    style=0.5f; level=0.0f;\n    if (MODE(model)==2) {\n')],
}
