"""Twelve bounded source/branch hypotheses; generated files stay under build/."""
from pathlib import Path
ROOT=Path(__file__).resolve().parents[4]
BASE=ROOT/'cloud/work/frontier/w23a/func_80012D18/best.c'
NAMED=BASE.with_name('probe_named_position.c')
ORDER=ROOT/'cloud/work/frontier/w26b/func_80012D18/q4_decl_order.c'

def replace(s,before,after,count=None):
    actual=s.count(before)
    if not actual or count is not None and actual!=count:raise ValueError(('source anchor',before,actual,count))
    return s.replace(before,after)

def sources():
    base=BASE.read_text();n=NAMED.read_text()
    p=replace(n,'            address[0] = position;\n','',5)
    p=replace(p,'            address[0] = V->loop_start;\n','',1)
    p=replace(p,'            offset = position - V->loop_start;\n','            offset = position - V->loop_start;\n            position = V->loop_start;\n',1)
    p=replace(p,'    padded[0] = length[0] + 6;','    address[0] = position;\n    padded[0] = length[0] + 6;',1)
    def excess(s):
        s=replace(s,'length[1] = 0;','excess = 0;',5)
        s=replace(s,'length[1] = excess - 6;','excess -= 6;',1)
        return replace(s,'    padded[0] = length[0] + 6;','    length[1] = excess;\n    padded[0] = length[0] + 6;',1)
    # Load real loop fields into two named carriers. These are not presumed to
    # be the original variables merely because their declaration sizes fit.
    f=replace(base,'    unsigned int end;','    unsigned int end;\n    unsigned int loopStart;',1)
    f=replace(f,'    unsigned char state;','    unsigned char state;\n    unsigned int loopLength;',1)
    f=replace(f,'    state = 2;','    loopStart = V->loop_start;\n    loopLength = V->loop_length;\n    state = 2;',1)
    a,b=f.split('    state = 2;',1)
    f=a+'    state = 2;'+b.replace('V->loop_start','loopStart').replace('V->loop_length','loopLength')
    g=replace(n,'    if (V->loop_length != 0 && (V->pending != 0 || V->tail == 0) && end >= (position = pos)) {','    if (V->loop_length != 0 && (V->pending != 0 || V->tail == 0)) {\n        position = pos;\n        if (end < position) goto plain;',1)
    g=replace(g,'    } else {\n        mode = -1;','    } else {\nplain:\n        mode = -1;',1)
    r=replace(n,'            address[0] = V->loop_start;\n            offset = position - V->loop_start;','            offset = position - V->loop_start;\n            position = V->loop_start;\n            address[0] = position;',1)
    e=replace(n,'        } else if (V->loop_length < (excess = frames - end + position) || excess >= position - V->loop_start) {','        } else {\n            excess = frames - end + position;\n            if (V->loop_length < excess || excess >= position - V->loop_start) {',1)
    e=replace(e,'            offset = 0;\n        }\n    } else {','            offset = 0;\n            }\n        }\n    } else {',1)
    return dict(baseline=base,named=n,declaration_order=ORDER.read_text(),position_merge=p,
                excess_merge=excess(n),both_merge=excess(p),field_carriers=f,
                subtract_remaining=replace(n,'frames - end + position','frames - (end - position)',2),
                add_before_subtract=replace(n,'frames - end + position','position + frames - end',2),
                nested_guard=g,position_rebase=r,excess_nested_branch=e)
