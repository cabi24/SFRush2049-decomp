void engine_sound_update(void)
{
    Vec3 zero;
    s32 i,j,offset,enabled,other,found;
    Scene36 *scene;
    Resource68 *resource;
    Node112 *node;
    void *key;
    Slot136 *slot;
    if((D_801174B4&8) && !D_80156994) return;
    if(D_801493D4) {
        zero.z=zero.x=zero.y=0.0f;
        for(i=0,offset=0;i<D_801493D4;i++,offset+=36) {
            scene=(Scene36 *)((u8 *)D_801493D0+offset);
            if(D_8014A110==2 && scene->action>0) {
                found=0;
                resource=scene->resources;
                for(j=0;j<scene->count;j++,resource++) if(resource->flags&0x01000000) {found=1;break;}
                if(!found) { if(scene->action>0) func_800B2CB4(scene->action); continue; }
            }
            if(((scene->flags&4) && (scene->flags&8)) ||
               (D_80152570 && (scene->flags&8)) || (!D_80152570 && (scene->flags&4))) {
                enabled=(scene->flags&0x8000)?0:1;
                other=(scene->flags&16)?0:1;
                for(j=0;j<scene->count;j++) {
                    if(D_80150DD0) {
                        scene->resources[j].flags&=~0x1000;
                        scene=(Scene36 *)((u8 *)D_801493D0+offset);
                    }
                    if(scene->resources[j].flags&1) {
                        transmission_ratio_get(scene,-1,&zero,0,i,enabled,other);
                        scene=(Scene36 *)((u8 *)D_801493D0+offset);
                    }
                }
                if(D_80150DD0 && scene->action>0) listener_position_set(scene->action);
            } else if(scene->flags&0x800) func_800B2CB4(scene->action);
        }
    }
    if(!D_80150DD0) {
        for(i=0;i<8;i++) if(D_80150E28[i]) D_80150E68[i]=audio_dma_sync(0,D_80150E28[i]*D_80117510[i]);
        if(D_80150F78) D_80150F38=audio_dma_sync(0,D_80150F78*64);
        for(i=0;i<D_801497EC;i++) {
            key=D_801497C0[i];
            node=D_80143FC8.head;
            while(node) {
                if((node->flags&8) && node->key==key) {D_801497C0[i]=node; break;}
                node=node->next;
            }
            if(key==D_801497C0[i]) D_801497C0[i]=0;
        }
    }
    if(D_8014A110==6) {
        for(offset=0;offset<2176;offset+=544) {
            ((Slot136 *)(D_80150E68[5]+offset))[0].active=0;
            ((Slot136 *)(D_80150E68[5]+offset))[0].object[0]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[0].object[1]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[0].object[2]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[1].active=0;
            ((Slot136 *)(D_80150E68[5]+offset))[1].object[0]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[1].object[1]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[1].object[2]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[2].active=0;
            ((Slot136 *)(D_80150E68[5]+offset))[2].object[0]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[2].object[1]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[2].object[2]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[3].active=0;
            ((Slot136 *)(D_80150E68[5]+offset))[3].object[0]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[3].object[1]=-1;
            ((Slot136 *)(D_80150E68[5]+offset))[3].object[2]=-1;
        }
    }
    D_80150F80=0;
    for(i=0;i<8;i++) if(D_80150E28[i]) D_80150E98[i]=D_80150E68[i];
    node=D_80143FC8.head;
    while(node) {engine_torque_calc(node); node=node->next;}
    if(D_8014A110==6) func_8039133C(D_80150DD0);
}

s32 transmission_ratio_get(Scene36 *scene,s16 a,Vec3 *v,void *p,s32 i,s32 e,s32 o)
{
    Node112 *n;
    n=func_8008E3C0(&D_80143FC8);
    if(n==0) return 1;
    n->index=i;
    engine_torque_calc(n);
    return 1;
}
