U=('    f32 unused[2];\n','')
T=('''            if (vol != set->rec[i].style) {
                set->rec[i].style = vol;
            } else {
                vol = -2.0f;
            }
            camera_clip_planes(set->rec[i].handle, pos, (s32) ref, pitch, vol);''','''            layer_place(&set->rec[i], pos, ref, pitch, vol);''')
H='''static void layer_place(LayerState *e, f32 *pos, f32 *ref, f32 level, f32 style) {
    if (style != e->style) {
        e->style = style;
    } else {
        style = -2.0f;
    }
    camera_clip_planes(e->handle, pos, (s32) ref, level, style);
}
void func_800E0050(MODELDAT *m) {'''
D=('void func_800E0050(MODELDAT *m) {',H)
V={'H':[U,T,D],'H_keepU':[T,D]}
