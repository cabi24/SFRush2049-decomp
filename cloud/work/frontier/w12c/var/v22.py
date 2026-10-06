O='''                if(style!=entry->style) entry->style=style;
                else style=-2.0f;
                camera_clip_planes(entry->handle,(f32 *)((u8 *)model+556),(s32)D_801141B0,level,style);'''
H='''static void layer_place(LayerState *e, f32 *pos, f32 level, f32 style) {
    if (style != e->style) e->style = style;
    else style = -2.0f;
    camera_clip_planes(e->handle, pos, (s32)D_801141B0, level, style);
}
#define MODE(m)'''
H2=H.replace('static void layer_place(LayerState *e, f32 *pos, f32 level, f32 style)','static void layer_place(LayerState *e, f32 *pos, s32 ref, f32 level, f32 style)').replace('(s32)D_801141B0, level','ref, level')
V={
 'H4':[(O,'                layer_place(entry,(f32 *)((u8 *)model+556),level,style);'),('#define MODE(m)',H)],
 'H5':[(O,'                layer_place(entry,(f32 *)((u8 *)model+556),(s32)D_801141B0,level,style);'),('#define MODE(m)',H2)],
}
