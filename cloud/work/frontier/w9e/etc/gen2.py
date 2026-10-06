import sys
pre=open('pre.c').read(); post=open('post.c').read()
STATE='''    if((s32)(D_801174B4<<9)>=0) {
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
    } else if((metadata->flags&8) && category==5) {
        slot=(Slot136 *)D_80150E68[5];
        for(index=0;index<16;index++,slot++) {
            if(!slot->active && slot->object[0]==-1 && slot->object[1]==-1 && slot->object[2]==-1) break;
        }
        slot->active=1;
        node->state=D_80150E98[category]+D_80117510[category]*index;
    }
'''
REST='''    if(node->texture!=-1) *(u16 *)((u8 *)&D_8012E700[node->resource]+20)=D_801427C0[node->texture];
    if(metadata->flags&2) {
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
    if(metadata->flags&0x4000) {
        if(!(node->flags&16)) metadata->callback(node);
        else {
            transform=D_80118DFC;
            func_800AB750(node->index,&transform,&node->normal,&node->position);
        }
    }
'''
V={}
V['s1']=('void func_800AB7D0(Node112 *node,Metadata48 *metadata,s32 category)\n{\n    s32 index,j;\n    Matrix64 *matrix;\n    Scene36 *scene;\n    Slot136 *slot;\n'+STATE+'}\n'
 +'void engine_torque_calc(Node112 *node)\n{\n    Metadata48 *metadata;\n    s32 category;\n    Animation24 *animation;\n    Vec3 transform;\n    metadata=&D_80117530[node->metadata];\n    category=metadata->category;\n    func_800AB7D0(node,metadata,category);\n'+REST+'}\n')
V['s2']=('void func_800AB7D0(Node112 *node)\n{\n    Metadata48 *metadata;\n    s32 category,index,j;\n    Matrix64 *matrix;\n    Scene36 *scene;\n    Slot136 *slot;\n    metadata=&D_80117530[node->metadata];\n    category=metadata->category;\n'+STATE+'}\n'
 +'void engine_torque_calc(Node112 *node)\n{\n    Metadata48 *metadata;\n    Animation24 *animation;\n    Vec3 transform;\n    func_800AB7D0(node);\n    metadata=&D_80117530[node->metadata];\n'+REST+'}\n')
V['s3']=('void func_800AB7D0(Node112 *node,Metadata48 *metadata)\n{\n    s32 category,index,j;\n    Matrix64 *matrix;\n    Scene36 *scene;\n    Slot136 *slot;\n    category=metadata->category;\n'+STATE+'}\n'
 +'void engine_torque_calc(Node112 *node)\n{\n    Metadata48 *metadata;\n    Animation24 *animation;\n    Vec3 transform;\n    metadata=&D_80117530[node->metadata];\n    func_800AB7D0(node,metadata);\n'+REST+'}\n')
for k in sys.argv[1:]:
    open('v_%s.c'%k,'w').write(pre+V[k]+post)
