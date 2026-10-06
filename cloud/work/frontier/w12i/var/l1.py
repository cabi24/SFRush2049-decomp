C='''camera_clip_planes(e->handle,pos,(s32)D_801141B0,level,style);'''
T='''camera_target_track((f32 *)((u8 *)model+556),(s32)D_801141B0,'''
V={
 'L1':[(C,C.replace('(s32)D_801141B0','(u32)D_801141B0'))],
 'L2':[(T,T.replace('(s32)D_801141B0','(u32)D_801141B0'))],
 'L3':[(C,C.replace('(s32)D_801141B0','(s32)&D_801141B0'))],
 'L4':[(T,T.replace('(s32)D_801141B0','(s32)&D_801141B0'))],
}
