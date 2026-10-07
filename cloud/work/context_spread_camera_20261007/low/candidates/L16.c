/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8;
typedef unsigned short u16; typedef int s32; typedef unsigned int u32;
typedef float f32;
typedef struct Entry {
    s8 active,started,stopRequested,stopSent,flag4,blocked;
    u8 reserved6[2];
    s32 id;
    u16 bank,sound;
    u32 voice,word20;
} Entry;
typedef struct Node {
    struct Node *prev,*next;
    u8 kind,retries,reservedA[2];
    Entry *entry;
    f32 value,stream,priority,route;
} Node;
typedef struct List {u32 reserved[2];Node *first,*last;} List;
extern Entry *D_80144C48;
extern s32 D_80144DB8,D_801460F4;
extern s32 D_801460C8[5];
extern s8 D_80110284,D_8011028C;
extern List D_80146138,D_80146160;
void audio_timing_sync(s32);
void func_80091FBC(List *,Node *,Node *);
void func_8009211C(List *,Node *);
s32 func_8001FE58(u32);
s32 func_8001FEA4(u32,u8);
s32 func_8001FF4C(u32,u8);
s32 func_8001FFA0(u32,u16);
s32 func_8001FFF4(u32,u8);
s32 func_80020174(u16,u8,u8);
u32 func_800201D0(u32);
void func_80020518(u8);

s32 MP_TargetSteerPos(Entry *entry,f32 volume,f32 pan) {
    if ((entry->voice=func_80020174(entry->sound,255,255))==(u32)-1) return 0;
    func_8001FEA4(entry->voice,(u8)(u32)(volume*127.0f));
    func_8001FFF4(entry->voice,(u8)(u32)((pan+1.0f)*0.5f*127.0f));
    entry->started=1;
    return 1;
}
static s32 stop_entry(Entry *entry) {
    if (entry->voice==(u32)-1) {
        entry->active=0;
        entry->id=0;
        return 1;
    }
    if (!entry->stopSent && func_8001FE58(entry->voice)!=0) return 0;
    entry->stopSent=1;
    if (func_800201D0(entry->voice)!=(u32)-1) return 0;
    entry->active=0;
    entry->id=0;
    return 1;
}
static s32 set_volume(Entry *entry,f32 value) {
    f32 scaled;
    if (entry->voice==(u32)-1) return 1;
    scaled=value*127.0f;
    if (func_8001FEA4(entry->voice,(u8)(u32)scaled)!=0) return 0;
    return 1;
}
static s32 set_pan(Entry *entry,f32 value) {
    if (entry->voice==(u32)-1) return 1;
    return func_8001FFF4(entry->voice,(u8)(u32)((value+1.0f)*0.5f*127.0f))==0;
}
static s32 set_depth(Entry *entry,f32 value) {
    if (entry->voice==(u32)-1) return 1;
    return func_8001FF4C(entry->voice,(u8)(u32)((value+1.0f)*0.5f*127.0f))==0;
}
static s32 set_rate(Entry *entry,f32 value) {
    f32 rate;
    if (entry->voice==(u32)-1) return 1;
    rate=value*8192.0f-1.0f;
    if (rate<0.0f) rate=0.0f;
    return func_8001FFA0(entry->voice,(u16)(u32)rate)==0;
}
void camera_transform(void) {
    s32 *history;
    s32 i,selected;
    Node *node,*next;
    Entry *entry;
    if (!D_80110284) return;
    D_80144DB8+=D_801460C8[4];
    history=&D_801460C8[4];
    do {
        history[0]=history[-1];
        --history;
    } while (history>=&D_801460C8[1]);
    D_801460C8[0]=0;
    if (D_8011028C) {
        selected=0;
        for (i=0;i<D_801460F4;i++) {
            entry=&D_80144C48[i];
            if (entry->active && entry->flag4 && !entry->stopRequested) {
                if (++selected==D_801460F4/4) break;
                audio_timing_sync(entry->id);
            }
        }
        if (i==D_801460F4) D_8011028C=0;
    }
    for (i=0;i<D_801460F4;i++) {
        if (D_80144C48[i].active && D_80144C48[i].started &&
            func_800201D0(D_80144C48[i].voice)==(u32)-1) {
            node=D_80146160.last;
            while (node) {
                next=node->next;
                if (node->entry==&D_80144C48[i]) {
                    func_8009211C(&D_80146160,node);
                    func_80091FBC(&D_80146138,node,D_80146138.first);
                }
                node=next;
            }
            D_801460C8[0]++;
            D_80144C48[i].id=0;
            D_80144C48[i].active=0;
        }
    }
    for (node=D_80146160.last;node;node=next) {
        next=node->next;
        entry=node->entry;
        if (entry && entry->stopRequested && node->kind!=2) goto recycle;
        if (entry && entry->blocked) continue;
        switch (node->kind) {
        case 0:
            if (D_80144DB8<=0) goto retry;
            if (!MP_TargetSteerPos(entry,node->value,node->stream)) goto retry;
            D_80144DB8--;
            break;
        case 2:
            if (!stop_entry(entry)) goto retry;
            D_801460C8[0]++;
            break;
        case 3:
            if (node->value!=-2.0f && !set_volume(entry,node->value)) goto retry;
            if (node->stream!=-2.0f && !set_pan(node->entry,node->stream)) goto retry;
            if (node->priority!=-2.0f && !set_depth(node->entry,node->priority)) goto retry;
            if (node->route!=-2.0f && !set_rate(node->entry,node->route)) goto retry;
            break;
        case 4:
            func_80020518((u8)(s32)node->value);
            break;
        }
recycle:
        func_8009211C(&D_80146160,node);
        func_80091FBC(&D_80146138,node,D_80146138.first);
        continue;
retry:
        node->retries++;
        if (node->entry) node->entry->blocked=1;
        if (node->retries>=10 && node->kind!=0) goto recycle;
    }
    for (node=D_80146160.last;node;node=node->next) {
        if (node->entry) node->entry->blocked=0;
    }
}
