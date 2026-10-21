#!/usr/bin/env python3
"""mk.py BASE_GROUP_DIR SRC_FUNC.c OUT_DIR : build a test group = BASE group with func_803A1EAC replaced by SRC and promoted to member."""
import sys, json, re, os
base, src, out = sys.argv[1:4]
b = open(os.path.join(base, 'group.c')).read()
s = open(src).read()
i = b.index('\nvoid func_803A1EAC(void)\n') + 1
j = b.index('\n}\n', i) + 3
b = b[:i] + s + b[j:]
# shifted-array externs: retail indexes the [5][13] arrays at row 0 with p+1; fix the old 1-D decls
b = re.sub(r'extern s8 D_80110E78\[13\],D_8011103C\[13\],D_80111080\[13\],D_8011119C\[13\],D_80111230\[13\],D_8011128C\[13\];',
           'extern s8 D_80110E78[5][13],D_8011103C[5][13],D_80111080[5][13],D_8011119C[5][13],D_80111230[5][13],D_8011128C[5][13];', b)
b = re.sub(r'extern s8 D_80111604\[13\],D_80111648\[13\],D_8011168C\[13\],D_8011157C\[13\],D_801115C0\[13\];',
           'extern s8 D_80111604[5][13],D_80111648[5][13],D_8011168C[5][13],D_8011157C[5][13],D_801115C0[5][13];', b)
b = b.replace('extern u32 object_manager_update(u8 *,s16);', 'extern u32 object_manager_update(u8 *,s16);')
b = b.replace('extern s32 object_manager_update(u8 *,s16);', 'extern u32 object_manager_update(u8 *,s16);')
b = b.replace('extern s32 D_801174BC,D_801174B4,', 'extern u32 D_801174BC,D_801174B4;\nextern s32 ')
b = b.replace('typedef struct PreparedCar {u8 unknown0,car,color[3],unknown5,flags,unknown7;} PreparedCar;', 'typedef struct PreparedCar {u8 unknown0,car;s8 color[3];u8 unknown5,flags,unknown7;} PreparedCar;')
b = b.replace('extern void resource_type_select(s32);', 'extern void resource_type_select(u32);')
os.makedirs(out, exist_ok=True)
open(os.path.join(out, 'group.c'), 'w').write(b)
j = json.load(open(os.path.join(base, 'group.json')))
if 'func_803A1EAC' not in j['members']: j['members'] = j['members'] + ['func_803A1EAC']
j['context'] = [c for c in j['context'] if c != 'func_803A1EAC']
j.pop('claims', None); j.pop('provenance', None)
json.dump(j, open(os.path.join(out, 'group.json'), 'w'), indent=1)
