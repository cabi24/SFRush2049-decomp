O='''                if(style!=entry->style) entry->style=style;
                else style=-2.0f;
                camera_clip_planes(entry->handle,(f32 *)((u8 *)model+556),(s32)D_801141B0,level,style);'''
def r(x): return [(O,x)]
V={
 'pos_first':r('''                f32 *pos=(f32 *)((u8 *)model+556);
                if(style!=entry->style) entry->style=style;
                else style=-2.0f;
                camera_clip_planes(entry->handle,pos,(s32)D_801141B0,level,style);'''),
 'neg':r('''                if(style==entry->style) style=-2.0f;
                else entry->style=style;
                camera_clip_planes(entry->handle,(f32 *)((u8 *)model+556),(s32)D_801141B0,level,style);'''),
 'addr':r('''                if(style!=entry->style) entry->style=style;
                else style=-2.0f;
                camera_clip_planes(entry->handle,(f32 *)((u8 *)model+556),(s32)&D_801141B0[0],level,style);'''),
 'ref_s32':r('''                if(style!=entry->style) entry->style=style;
                else style=-2.0f;
                camera_clip_planes(entry->handle,(f32 *)((u8 *)model+556),(s32)&D_801141B0x,level,style);'''),
}
V['ref_s32'].append(('extern f32 D_801141B0[3];','extern f32 D_801141B0[3];\nextern s32 D_801141B0x __asm__("D_801141B0");'))
del V['ref_s32']
