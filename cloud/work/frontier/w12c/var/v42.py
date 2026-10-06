H='''static void layer_place(LayerState *e, f32 *pos, s32 ref, f32 level, f32 style)
{
    if (style != e->style) {
        e->style = style;
    } else {
        style = -2.0f;
    }
    camera_clip_planes(e->handle, pos, ref, level, style);
}
void func_800E0050(MODELDAT *m) {'''
E50=('''            if (vol != set->rec[i].style) {
                set->rec[i].style = vol;
            } else {
                vol = -2.0f;
            }
            camera_clip_planes(set->rec[i].handle, pos, (s32) ref, pitch, vol);''','''            layer_place(&set->rec[i], pos, (s32) ref, pitch, vol);''')
E5F=('''                f32 *pos=(f32 *)((u8 *)model+556);
                if(style!=entry->style) entry->style=style;
                else style=-2.0f;
                camera_clip_planes(entry->handle,pos,(s32)D_801141B0,level,style);''','''                layer_place(entry,(f32 *)((u8 *)model+556),(s32)D_801141B0,level,style);''')
U=('    f32 unused[2];\n','')
V={'base':[], 'P_both':[('void func_800E0050(MODELDAT *m) {',H),E50,E5F,U], 'P_e5f':[('void func_800E0050(MODELDAT *m) {',H),E5F], 'P_e50':[('void func_800E0050(MODELDAT *m) {',H),E50,U]}
