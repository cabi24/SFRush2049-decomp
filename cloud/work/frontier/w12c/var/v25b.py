exec(open('v24.py').read().split('V=')[0])
H='''static void layer_set(LayerState *state, f32 level, f32 style)
{
    if (level!=state->level) {
        state->level=level;
        client_sync(state->handle,level);
    }
    if (style!=state->style) {
        state->style=style;
        entity_hierarchy_update(state->handle,style);
    }
}
#define MODE(m)'''
M2=(M[0],'''                layer_set(entry,level,style);''')
V={'ls':[M2,('#define MODE(m)',H)]}
#'ls_pcc':[M2,P,('#define MODE(m)',H)]}
