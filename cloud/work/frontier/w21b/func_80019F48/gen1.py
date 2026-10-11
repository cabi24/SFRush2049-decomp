import sys
s=open('w20b_best.c').read()
D=s[s.index("    Layer *layer;\n    u16 count;"):s.index("    result = 0xFFFFFFFFU;")]
lines=dict((l.split()[-1].rstrip(';').lstrip('*'),l+'\n') for l in D.strip('\n').split('\n'))
lines['mkey']='    u8 mkey;\n'
def mk(order):
    t=s.replace(D,''.join(lines[x] for x in order))
    if 'mkey' in order:
        a="            if (layer->id == 0xFFFF || layer->low > (key & 127) || layer->high < (key & 127)) continue;\n            note = (key & 127) + layer->transpose;"
        assert a in t
        t=t.replace(a,"            if (layer->id == 0xFFFF || layer->low > mkey || layer->high < mkey) continue;\n            note = mkey + layer->transpose;")
        t=t.replace("    layer = func_80016F80(","    mkey = key & 127;\n    layer = func_80016F80(")
    return t
