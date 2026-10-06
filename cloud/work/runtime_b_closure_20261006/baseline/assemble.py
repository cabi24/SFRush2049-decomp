#!/usr/bin/env python3
"""Assemble the one approved source baseline without editing its inputs."""
import hashlib
import json
from pathlib import Path

HERE = Path(__file__).resolve().parent
WORKSPACE = HERE.parent
ORDER = [
    ('runtime-b-e114-parent','runtime_b_e114_parent_20261006','children.c','D200'),
    ('runtime-b-e114-parent','runtime_b_e114_parent_20261006','children.c','D328'),
    ('runtime-b-d498-visual','runtime_b_d498_visual_20261006','visual.c','D498'),
    ('runtime-b-da78-position','runtime_b_da78_position_20261006','position.c','DA78'),
    ('runtime-b-e114-parent','runtime_b_e114_parent_20261006','children.c','E088'),
    ('runtime-b-e114-update','runtime_b_e114_update_20261006','update.c','E114'),
    ('runtime-b-f938-setup','runtime_b_f938_setup_20261006','setup.c','F938'),
    ('runtime-b-fce0-root','runtime_b_fce0_root_20261006','root.c','FCE0'),
]
SIGNATURES = {
    'D200':'void func_8038D200(', 'D328':'BObject *func_8038D328(',
    'D498':'void func_8038D498(', 'DA78':'BPlayer *func_8038DA78(',
    'E088':'void func_8038E088(', 'E114':'void func_8038E114(',
    'F938':'void func_8038F938(', 'FCE0':'void func_8038FCE0(',
}

def sha(data): return hashlib.sha256(data).hexdigest()

def body(source, marker):
    start = source.index(marker)
    opening = source.index('{',start)
    level = 1
    end = opening+1
    while level:
        if source[end] == '{': level += 1
        elif source[end] == '}': level -= 1
        end += 1
    return source[start:end]

def main():
    inventory = json.loads((HERE/'inventory.json').read_text())
    source_hashes = {x['path']:x['sha256'] for x in inventory['sources']}
    header = (HERE/'shared_schema.proposed.h').read_text()
    assert sha(header.encode()) == inventory['schema_sha256']
    pieces = ['/* flags: -g0 -O3 -mips2 -G 0 -non_shared */',
              '/* One approved genuine-closure diagnostic. Research only, no original-TU claim. */',
              header]
    changes = []
    for directory, packet, filename, key in ORDER:
        p = WORKSPACE/directory/'cloud/work'/packet/filename
        raw = p.read_bytes()
        assert sha(raw) == source_hashes[str(p)], ('source drift',str(p))
        original = body(raw.decode(),SIGNATURES[key])
        text = original
        replacements = {}
        if key == 'FCE0':
            replacements.update({'record->unknown05':'record->hit_mask',
                'record->elapsed':'record->collision_extent',
                'release->handle':'(s16 *)release->quad'})
        if key == 'F938': replacements['BEffect *effect;'] = 'BStatusEffect *effect;'
        if key == 'DA78': replacements['record->radius'] = 'record->collision_extent'
        if key == 'E114':
            replacements.update({'func_800B24EC(D_80394D08, &D_80394F88,':
                'func_800B24EC((char *)D_80394D08, (s16 *)&D_80394F88,'})
        for child in ('D200','D328','D498','DA78','E088','E114','F938'):
            if 'private_'+child in text:
                replacements['private_'+child] = 'func_8038'+child
        if 'sound_call_minimal' in text:
            replacements['sound_call_minimal'] = 'func_80090254'
        applied = []
        for before, after in replacements.items():
            count = text.count(before)
            assert count, (key,before)
            text = text.replace(before,after)
            applied.append(dict(before=before,after=after,count=count))
        if key != 'FCE0': text = 'static '+text
        pieces.append('/* Complete '+key+' body; source-preserving approved reconciliation only. */\n'+text)
        changes.append(dict(member=key,source=str(p),original_body_sha256=sha(original.encode()),
                            reconciled_body_sha256=sha(text.encode()),
                            made_static=key!='FCE0',replacements=applied))
    combined = '\n\n'.join(pieces)+'\n'
    (HERE/'closure.c').write_text(combined)
    (HERE/'assembly.json').write_text(json.dumps(dict(status='one approved source assembly; not a match',
        source_order=[x[-1] for x in ORDER],keep=['func_8038FCE0'],
        combined_sha256=sha(combined.encode()),schema_sha256=sha(header.encode()),changes=changes),indent=2)+'\n')
    (HERE/'group.json').write_text(json.dumps(dict(files=['closure.c'],keep=['func_8038FCE0'],
        flags='-g0 -O3 -mips2 -G 0 -non_shared'),indent=2)+'\n')
    print(json.dumps(dict(source='closure.c',sha256=sha(combined.encode()),members=len(ORDER)),indent=2))

if __name__ == '__main__': main()
