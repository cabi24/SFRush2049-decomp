#!/usr/bin/env python3
"""fx.py NAME [NAME...]: retail disassembly of a game function with every jal target
annotated by N64 name, matched flag and MAT3 identity (w36n map + w38a/w39a overrides),
plus the function's callers. Read-only. Usage from repo root."""
import json, re, struct, subprocess, sys, tempfile
from pathlib import Path
ROOT = Path(__file__).resolve().parents[5]
sys.path.insert(0, str(ROOT / 'tools' / 'cloud')); import score
syms = {k: int(v, 16) for k, v in json.load(open(ROOT / 'asm/us/blob/symbols.json'))['symbols'].items()}
rev = {}
for k, v in syms.items(): rev.setdefault(v, []).append(k)
lock = json.load(open(ROOT / 'blob_matched.lock.json'))
mp = {}
for r in json.load(open(ROOT / 'cloud/work/frontier/w36n/map_functions.json'))['rows']:
    if r['population'] == 'game' and r.get('mat3_name'):
        mp[r['n64_name']] = f"{r['mat3_name']}@{r['mat3_file']}[{r['confidence'][0]}]"
OVR = {'func_8010E8B4': 'animPanes@videoscr.c[w38a]', 'world_effect_update': 'InitGraphics@Init.c[w38a]',
       'func_80098FB8': 'AudioMain[w38a]', 'func_800A3640': '_RemovePak[w38a]', 'func_800B66B0': '_TextLen[w38a]',
       'object_bytes23_sum': 'TB_GetFontHeight[w38a]', 'car_lod_select': 'PS_InitPakSys[w38a]',
       'world_velocity_integrate': 'HAL_FinishText[w38a]', 'menu_input_process': 'HAL_DrawText[w38a]',
       'audio_doppler_calc': '_DrawText[w36t]', 'camera_look_at_point': '_PlayMoveSound@targetpaths.c[w38a]',
       'drone_collision_avoid': 'calcMenuRect@audioscr.c[w38a]', 'audio_frame_update': 'StartCar@visuals.c[w38a]',
       'object_create': 'HAL_SetFont[w36o]', 'slot_state_setup': '_HAL_SetFont[w36o]', 'func_800B6748': '_SetDefaults[w38a]',
       'func_800B70EC': 'HAL_SetTextPos[w38a]', 'func_800ED66C': 'HAL_SetTextTranslucency[w38a]'}
mp.update(OVR)
T = score.targets()
def label(a):
    ns = rev.get(a, ['?'])
    n = ns[0]
    m = 'M' if n in lock else ('u' if n in T else '-')
    return f"{n}({m}){'=' + mp[n] if n in mp else ''}"
def callers(name):
    va = syms[name]; out = []
    for fn, words in T.items():
        for w in words:
            if (w >> 26) == 3 and ((w & 0x3ffffff) << 2 | 0x80000000) == va:
                out.append(fn); break
    return out
for name in sys.argv[1:]:
    words = T[name]; va = syms[name]
    with tempfile.NamedTemporaryFile(suffix='.bin') as f:
        f.write(struct.pack(f'>{len(words)}I', *words)); f.flush()
        out = subprocess.run(['mips-linux-gnu-objdump', '-D', '-b', 'binary', '-m', 'mips:4300', '-EB',
                              f'--adjust-vma=0x{va:x}', '-M', 'gpr-names=32', f.name], capture_output=True, text=True).stdout
    print(f'== {name} @ 0x{va:08X} ({len(words)} words, {4*len(words)} B) {mp.get(name, "")}')
    print('   callers:', ', '.join(label(syms[c]) for c in callers(name)))
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+([0-9a-f]{8})\s+(.*)', line)
        if not m: continue
        ins = m.group(3)
        j = re.search(r'\bjal?\s+(0x)?([0-9a-f]+)', ins)
        if j and ins.startswith('jal'): ins += '   <' + label(int(j.group(2), 16)) + '>'
        print(f'  {m.group(1)}: {ins}')
