O='''                if(style!=entry->style) entry->style=style;
                else style=-2.0f;
                camera_clip_planes(entry->handle,(f32 *)((u8 *)model+556),(s32)D_801141B0,level,style);'''
V={
 'argT':[(O,'''                camera_clip_planes(entry->handle,(f32 *)((u8 *)model+556),(s32)D_801141B0,level,style!=entry->style?(entry->style=style):-2.0f);''')],
 'asgT':[(O,'''                style=style!=entry->style?(entry->style=style):-2.0f;
                camera_clip_planes(entry->handle,(f32 *)((u8 *)model+556),(s32)D_801141B0,level,style);''')],
 'braces':[(O,'''                if(style!=entry->style) {
                    entry->style=style;
                } else {
                    style=-2.0f;
                }
                camera_clip_planes(entry->handle,(f32 *)((u8 *)model+556),(s32)D_801141B0,level,style);''')],
}
