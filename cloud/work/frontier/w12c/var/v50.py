O='''                f32 *pos=(f32 *)((u8 *)model+556);
                if(style!=entry->style) entry->style=style;
                else style=-2.0f;
                camera_clip_planes(entry->handle,pos,(s32)D_801141B0,level,style);'''
def r(x): return [(O,x)]
V={
 'argT':r('''                f32 *pos=(f32 *)((u8 *)model+556);
                camera_clip_planes(entry->handle,pos,(s32)D_801141B0,level,style!=entry->style?(entry->style=style):-2.0f);'''),
 'argT2':r('''                camera_clip_planes(entry->handle,(f32 *)((u8 *)model+556),(s32)D_801141B0,level,style!=entry->style?(entry->style=style):-2.0f);'''),
 'argT3':r('''                f32 *pos=(f32 *)((u8 *)model+556);
                camera_clip_planes(entry->handle,pos,(s32)D_801141B0,level,style!=entry->style?(entry->style=style):(style=-2.0f));'''),
}
