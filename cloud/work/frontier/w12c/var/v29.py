D='''    s32 original_slot=model->index;
    s32 slot;
    LayerState *entry;
    f32 style,level;
    f32 weighted,value;
    s32 i;
'''
V={
 'lvl_last':[(D,'''    s32 original_slot=model->index;
    s32 slot;
    LayerState *entry;
    f32 style;
    f32 weighted,value;
    s32 i;
    f32 level;
''')],
 'lvl_after_i':[(D,'''    s32 original_slot=model->index;
    s32 slot;
    LayerState *entry;
    f32 weighted,value;
    s32 i;
    f32 style,level;
''')],
}
