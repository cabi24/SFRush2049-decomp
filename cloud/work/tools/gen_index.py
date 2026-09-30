#!/usr/bin/env python3
"""gen_index.py: regenerate cloud/work/ipa-groups/INDEX.md by rescoring every group.

Groups whose group.json has a "targets" key (tail-function groups, targets not in score.py's
sections) are scored with `python3 cloud/work/tools/extscore.py --norm <dir>`; all others with
`python3 cloud/work/tools/zbuild.py <dir> --as1=-r4300_mul`. Context functions are
not counted. Extent = sum of the members' target words. Notes and the no-directory
groups are hand-maintained in NOTES / NO_DIR below. Run from the repo root:

    python3 cloud/work/tools/gen_index.py            # rewrites INDEX.md
    python3 cloud/work/tools/gen_index.py --stdout   # print only
"""
import re, subprocess, sys, json
from pathlib import Path
ROOT = Path(__file__).resolve().parents[3]
G = ROOT / 'cloud/work/ipa-groups'
SUPERSEDED = {'car_cg_height_set': 'zlib_deflate', 'car_collision_update': 'zlib_deflate',
              'car_crash_response': 'zlib_deflate'}
SUSPICIOUS = {'menu_back', 'highscore_entry_anim'}

NOTES = {
 'MP_TargetSteerPos': 'camera_transform is context (seed).',
 'audio_frame_sync': 'resource loader thread; five members still differ.',
 'audio_frame_update': 'func_800B0A88 matches; func_800B08FC 4 words off. No closure gap.',
 'audio_pitch_adjust': 'Real extent 18 words (INDEX listed the caller). Stand-in callers give IPA registers; not spliceable as is.',
 'best_times_display': 'Real extent 56 words (listed 1068 = caller mode_select_handler). Parameter lands in s1 not s2.',
 'billboard_render': 'Real extent 244 words (listed 665). Mode state machine; func_800F1210 is a stand-in context. .rodata jump-table relocations unverified.',
 'camera_aspect_ratio': 'No closure gap left (camera_process_input, camera_track_spline in unit). Only the three small members match.',
 'camera_scene_manager': 'Trick/stunt scoring, not a camera. No closure gap. Three members match; camera_scene_manager itself is far.',
 'car_cg_height_set': 'Superseded by zlib_deflate; still fails to build (expected). Do not splice.',
 'car_collision_init': 'Duplicate of two zlib_deflate members; drop when zlib_deflate is used.',
 'car_collision_update': 'Superseded by zlib_deflate; still fails to build (expected). Do not splice.',
 'car_crash_response': 'Superseded by zlib_deflate; still fails to build (expected). Do not splice.',
 'catchup_logic': 'Really func_801084D4 (unregistered head, 375 words; listed 57 insns is the tail). Two dead move s0,v0 after slot_state_setup.',
 'championship_standings': 'func_800DC120 matches; func_800DC1AC 3/39 off.',
 'controller_poll': 'func_800C9590, player_mode_set, player_state_set match. Closure gap func_800E73D8. Defines D_80149B64/D_80149B74; splicing must not define them elsewhere.',
 'cpak_init': 'Tyre marks, not Controller Pak. func_800AF8C0 13 words off.',
 'draw_number': 'Real extent 110 words (listed 973 = caller). Stand-in callers; not spliceable as is.',
 'dynamic_difficulty': 'Really func_80107EDC (unregistered head, 158 words; listed 47 insns is the tail). Two dead move s0,v0 remain.',
 'func_8008705C': 'Real extent 45 words (listed 869). Needs func_80086A50 stand-in in group; not spliceable as is.',
 'func_8008B640': 'model_bounds_calc and physics_velocity_integrate_b..f match; closure complete. func_8008B640 and _a blocked by IPA parameter register choice.',
 'func_800AD4C8': 'Closure gaps: camera_play_script, camera_trigger_check, camera_victory, entity_update, func_800C36A0, input_deadzone_apply; func_800AD650/func_800AD5D0 also missing.',
 'func_800B9B64': 'Path graph; 4/4 with -r4300_mul (100/129 for func_800B9B64 without it).',
 'func_800D2FA8': 'time_of_day_select matches; split_time_display 14/60 off.',
 'func_800E4300': 'Closure gaps: func_800E4B58, func_800E398C (link to func_800E56F8; consider merging). func_800E451C is a hand rewrite, 388/397.',
 'func_800E56F8': 'Closure gap: func_800E4B58 (567w) and behind it func_800E398C. func_800E56F8 has the ROM size, register naming only.',
 'func_800E681C': 'Control input; func_800E627C matches, func_800E6460 32/239.',
 'func_800E92C8': 'func_800EA2DC matches; rest far.',
 'func_800F0F44': 'Real extent 116 words (listed 815 = caller func_800F1210). Register allocation differs; not spliceable as is.',
 'func_8010A7A4': 'Real extent 75 words (listed 242). Register allocation differs.',
 'highscore_entry_anim': 'Not a work item: tail of an unregistered function; needs head registered by maintainers.',
 'menu_back': 'Not a work item: callers (func_800CBF2C) missed by the generator; closure gap func_800CBF2C and its callers.',
 'reconnect_attempt': 'Really func_8010BC84 (unregistered head, 232 words; listed 106 insns is the tail).',
 'zlib_deflate': 'Spliced into the ROM; needs --allow-unverified (static_init_done in .data).',
}
NO_DIR = [
 ('name_entry_screen', '~628 words (real head 0x80103D28)', 'no dir',
  'Listed 247 insns is a tail; the real head is unregistered and large. No draft yet.'),
]
LINE = re.compile(r'^\s{2}(\w+)\s+(\(context\) )?size\s+(\d+)/(\d+)\s+(.*)$')

def run(d):
    if 'targets' in json.loads((d / 'group.json').read_text()):
        cmd = ['python3', str(ROOT / 'cloud/work/tools/extscore.py'), '--norm', str(d)]
    else:
        cmd = ['python3', str(ROOT / 'cloud/work/tools/zbuild.py'), str(d), '--as1=-r4300_mul']
    p = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, timeout=900)
    return cmd[1].endswith('extscore.py'), p

def row(d):
    name = d.name
    spec = json.loads((d / 'group.json').read_text())
    members = set(spec['members'])
    note = NOTES.get(name, '')
    if name in SUPERSEDED:
        return (name, '-', f'superseded by {SUPERSEDED[name]}', note)
    ext, p = run(d)
    out = p.stdout + p.stderr
    tot = ok = 0; details = []
    for l in out.splitlines():
        m = LINE.match(l)
        if not m or m.group(2) or m.group(1) not in members: continue
        w = int(m.group(4)); tot += w
        if 'MATCH' in m.group(5) and 'differ' not in m.group(5).split('MATCH')[0]:
            ok += w
    if not tot:
        err = re.sub(r'\s+', ' ', out.strip().splitlines()[-1] if out.strip() else 'no output')[:70]
        return (name, '-', f'BUILD ERROR: {err}', note)
    nm = sum(1 for l in out.splitlines() if (m := LINE.match(l)) and not m.group(2)
             and m.group(1) in members and 'MATCH' in m.group(5))
    status = f'MATCH {nm}/{len(members)}, builds' if nm else 'builds, no member matches'
    if nm == len(members): status = f'MATCH {nm}/{len(members)}'
    al = re.search(r'exact words after alignment (\d+)/(\d+)', out)
    if al and not nm: status += f' ({al.group(1)}/{al.group(2)} words aligned)'
    if name in SUSPICIOUS: status = 'suspicious, ' + status
    return (name, f'{tot} w', status, note)

def main():
    rows = [row(d) for d in sorted(G.iterdir()) if (d / 'group.json').exists()]
    rows += [(n, e, s, note) for n, e, s, note in NO_DIR]
    rows.sort(key=lambda r: r[0].lower())
    L = ['# IPA call groups: index', '',
         'Generated by `python3 cloud/work/tools/gen_index.py` (rescores every group; do not edit by hand,',
         'edit NOTES in the script). Scores are strict: groups via `zbuild.py --as1=-r4300_mul`, tail-function',
         'groups via `extscore.py --norm`. "Extent" is the sum of the members\' target words (real extent, not the',
         'original m2c "insns" figure). Context and stand-in functions are not counted. See each dir\'s',
         '`STATUS.md` for detail and [../../../CloudHandoffV2.md](../../../CloudHandoffV2.md) for the workflow.', '',
         '| group | extent | status | note |', '|---|---|---|---|']
    for n, e, s, note in rows:
        L.append(f'| {n} | {e} | {s} | {note} |')
    text = '\n'.join(L) + '\n'
    if '--stdout' in sys.argv: print(text)
    else: (G / 'INDEX.md').write_text(text); print('wrote', G / 'INDEX.md', len(rows), 'rows')
main()
