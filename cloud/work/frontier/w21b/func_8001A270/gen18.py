import itertools
hdr=open('v/r11/ke_id_pa_in_pr_no_ki_re.c').read()
pre=hdr[:hdr.index("u32 func_8001A270(")]
def body(decls, idx, ent, kid, pan, res):
    # idx: name of index expr; ent: entry expr template; kid: kid expr/var
    E=lambda f: f"{ent}->{f}" if ent!='keymap[%s]' else f"keymap[{idx}].{f}"
    K = kid
    return f"""u32 func_8001A270(u32 packed, u8 key, u8 volume, u8 panning,
                 u8 channel, u8 set, u16 offset, u16 section, u8 group, s16 priorityOffset)
{{
{decls}    id = packed >> 16;
    priority = (int)((packed >> 8) & 255) + priorityOffset;
    priority = priority < 0 ? 0 : priority > 255 ? 255 : priority;
    packed = (packed & 0xFFFF00FFU) | ((u32)priority << 8);
    switch (id & 0xC000) {{
    case 0:
        {res} = func_80019ED0(key, channel, set);
        if ({res} != 0xFFFFFFFFU) return {res};
        return func_80024988(packed, packed >> 16, key, volume, panning, channel, set, offset, section, 1, group);
    case 0x4000:
        keymap = func_80016EE0(id);
        if (keymap != 0) {{
@IDXASSIGN@@ENTASSIGN@            if (@KIDTEST@ != 0xFFFF) {{
                if (({E('panning')} & 128) == 0) {{
                    {pan} = keymap[key].panning - 64;
                    {pan} += panning;
                    if ({pan} < 0) panning = 0;
                    else if ({pan} > 127) panning = 127;
                    else panning = {pan};
                }} else panning = 128;
                note = (key & 127) + {E('transpose')};
                note = note > 127 ? 127 : note < 0 ? 0 : note;
                priority += {E('priority')};
                priority = priority > 255 ? 255 : priority < 0 ? 0 : priority;
                packed &= 255;
                packed |= (u32)priority << 8;
                packed |= (u32){K} << 16;
                if (({K} & 0xC000) == 0) {{
                    {res} = func_80019ED0(note, channel, set);
                    if ({res} != 0xFFFFFFFFU) return {res};
                    return func_80024988(packed, id, note | (key & 128), volume, panning, channel, set, offset, section, 1, group);
                }}
                return func_80019F48(packed, id, note | (key & 128), volume, panning, channel, set, offset, section, group);
            }}
        }}
        break;
    case 0x8000:
        return func_80019F48(packed, packed >> 16, key, volume, panning, channel, set, offset, section, group);
    }}
    return 0xFFFFFFFFU;
}}
"""
D={'keymap':'    Keymap *keymap;\n','id':'    u16 id;\n','pan':'    int pan;\n','index':'    u8 index;\n','priority':'    int priority;\n','note':'    int note;\n','kid':'    int kid;\n','entry':'    Keymap *entry;\n','result':'    u32 result;\n'}
n=0
for extra in itertools.combinations(['pan','index','entry','kid'],2):
    for order in itertools.permutations(extra):
        decl=''.join(D[x] for x in ['keymap','id',order[0],order[1],'priority','note'])
        idx='index' if 'index' in extra else 'key & 127'
        ent='entry' if 'entry' in extra else 'keymap[%s]'
        kid='kid' if 'kid' in extra else E_KID if False else None
        for res in ['priority','note'] + (['pan'] if 'pan' in extra else []) + (['kid'] if 'kid' in extra else []):
            pan='pan' if 'pan' in extra else 'note'
            if 'kid' in extra: kidexpr='kid'; kidtest='(kid = %s)'
            else: kidexpr=None
            b=body(decl, idx, ent, 'KIDX', pan, res)
            entexpr = "(*entry)" if ent=="entry" else f'keymap[{idx}]'
            if 'kid' in extra:
                b=b.replace('@KIDTEST@', f'(kid = {entexpr}.id)').replace('KIDX','kid')
            else:
                b=b.replace('@KIDTEST@', f'{entexpr}.id').replace('KIDX', f'{entexpr}.id')
            b=b.replace('@IDXASSIGN@', '            index = key & 127;\n' if 'index' in extra else '')
            b=b.replace('@ENTASSIGN@', f'            entry = &keymap[{idx}];\n' if ent=='entry' else '')
            open(f'v/r18b/{"_".join(x[:2] for x in order)}_r{res[:2]}.c','w').write(pre+b); n+=1
print(n)
