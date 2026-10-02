/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef int s32;
typedef struct Resource {u8 opaque0[20];u16 tag;s16 previous,next;u8 opaque26[42];} Resource;
extern Resource D_8012E700[];
extern s16 D_8015B254;
extern s32 D_80156990;
extern s16 func_8009002C(s16);
extern s16 func_8008FFD0(s16);
void entity_spawn_callback(s16 handle,s32 remove_previous,s32 remove_next) {
    Resource *resource;
    if(remove_next) {
        resource=&D_8012E700[handle];
        if(resource->next>=0) {
            entity_spawn_callback(resource->next,1,1);
            resource->next=-1;
        }
    }
    resource=&D_8012E700[handle];
    if(remove_previous) {
        if(resource->previous>=0) {
            entity_spawn_callback(resource->previous,1,1);
            resource->previous=-1;
        }
    }
    {
    s16 index=func_8009002C(handle);
    if(index>=0) D_8012E700[index].previous=resource->next;
    else {
        if(handle==D_8015B254) D_8015B254=resource->next;
        else {
            s16 index=func_8008FFD0(handle);
            if(index>=0) D_8012E700[index].next=resource->next;
        }
    }
    }
    resource->previous=-1;
    resource->next=-1;
    resource->tag=0xffff;
    if(handle+1==D_80156990) {
        do {
            D_80156990--;
            if(D_8012E700[D_80156990-1].tag!=0xffff) break;
        } while(D_80156990>0);
    }
}
