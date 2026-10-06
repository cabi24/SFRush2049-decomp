import sys
pre=open('pre.c').read(); post=open('post.c').read()
def body(h):
    H={}
    H['slot']=('''s32 func_800AB7D0(void)
{
    Slot136 *slot;
    s32 index;
    slot=(Slot136 *)D_80150E68[5];
    for(index=0;index<16;index++,slot++) {
        if(!slot->active && slot->object[0]==-1 && slot->object[1]==-1 && slot->object[2]==-1) break;
    }
    slot->active=1;
    return index;
}
''')
    H['anim']=('''void func_800AB7D0(Node112 *node,Metadata48 *metadata)
{
    Animation24 *animation;
    animation=func_80090284();
    if(animation) {
        animation->state=0;
        animation->node=node;
        animation->time=0.0f;
        animation->data=metadata->animation;
        animation->next=D_801391F0;
        D_801391F0=animation;
        if((metadata->flags&4) && metadata->callback) metadata->callback(node);
    }
}
''')
    H['matrix']=('''void func_800AB7D0(Node112 *node)
{
    Matrix64 *matrix;
    s32 j,index;
    node->index=D_80150F80++;
    matrix=&D_80150F38[node->index];
    math_utility(D_8011418C,matrix->matrix);
    for(j=0;j<3;j++) {
        ((f32 *)&matrix->velocity)[j]=0.0f;
        ((f32 *)&matrix->position)[j]=0.0f;
    }
    index=(s32)D_80117530[node->metadata].value;
    matrix->record=D_80118C10+index*32;
}
''')
    H['none']=''
    out=H[h]
    out+='''void engine_torque_calc(Node112 *node)
{
    Metadata48 *metadata;
    s32 category,index,j;
    Matrix64 *matrix;
    Scene36 *scene;
    Slot136 *slot;
    Animation24 *animation;
    Vec3 transform;
    metadata=&D_80117530[node->metadata];
    category=metadata->category;
    if((s32)(D_801174B4<<9)>=0) {
        if(metadata->flags&8) {
            node->state=D_80150E98[category];
            if(!(node->flags&16) && category==4) {
                scene=&D_801493D0[node->index];
                *(Scene36 **)node->state=scene;
                if((*(Scene36 **)node->state)->flags&16) node->flags&=~8;
            }
            D_80150E98[category]+=D_80117510[category];
        } else if(category!=7) {
            node->state=D_80118DDC[category]+D_80117510[category]*metadata->element;
        }
        if(metadata->kind==4) {
'''
    if h=='matrix': out+='            func_800AB7D0(node);\n'
    else: out+='''            node->index=D_80150F80++;
            matrix=&D_80150F38[node->index];
            math_utility(D_8011418C,matrix->matrix);
            for(j=0;j<3;j++) {
                ((f32 *)&matrix->velocity)[j]=0.0f;
                ((f32 *)&matrix->position)[j]=0.0f;
            }
            index=(s32)D_80117530[node->metadata].value;
            matrix->record=D_80118C10+index*32;
'''
    out+='''        }
    } else if((metadata->flags&8) && category==5) {
'''
    if h=='slot': out+='        index=func_800AB7D0();\n'
    else: out+='''        slot=(Slot136 *)D_80150E68[5];
        for(index=0;index<16;index++,slot++) {
            if(!slot->active && slot->object[0]==-1 && slot->object[1]==-1 && slot->object[2]==-1) break;
        }
        slot->active=1;
'''
    out+='''        node->state=D_80150E98[category]+D_80117510[category]*index;
    }
    if(node->texture!=-1) *(u16 *)((u8 *)&D_8012E700[node->resource]+20)=D_801427C0[node->texture];
    if(metadata->flags&2) {
'''
    if h=='anim': out+='        func_800AB7D0(node,metadata);\n'
    else: out+='''        animation=func_80090284();
        if(animation) {
            animation->state=0;
            animation->node=node;
            animation->time=0.0f;
            animation->data=metadata->animation;
            animation->next=D_801391F0;
            D_801391F0=animation;
            if((metadata->flags&4) && metadata->callback) metadata->callback(node);
        }
'''
    out+='''    }
    if(metadata->flags&0x4000) {
        if(!(node->flags&16)) metadata->callback(node);
        else {
            transform=D_80118DFC;
            func_800AB750(node->index,&transform,&node->normal,&node->position);
        }
    }
}
'''
    return out
for h in sys.argv[1:]:
    open('v_%s.c'%h,'w').write(pre+body(h)+post)
