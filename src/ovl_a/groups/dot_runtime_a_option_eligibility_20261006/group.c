/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Color4 { u32 word; } Color4;
typedef struct Language { s32 unknown0; char **text; s32 unknown8; u16 *indices; char **indexed; } Language;
typedef struct Slot60 { f32 x,y,angle; u8 unknown0C[36]; f32 position[3]; } Slot60;
typedef struct Camera { f32 uv[3][3],position[3]; u8 unknown30[104]; } Camera;
extern Language countdown_state;
extern Slot60 D_803B6610[];
extern Camera D_80150B70[];
extern f32 D_803B68D4[3],D_803B6898[3];
extern s32 D_803B6A48,D_803B6A68,D_803B6A6C;
extern s16 D_8014A108;
extern Color4 D_801146BC,D_801146C0;
extern char D_803B8CD8[],D_803B8CDC[];
extern void render_helper(f32);
extern s32 object_create(s32);
extern void dispatch_handler(s32);
extern void func_800B669C(u32,u32);
extern void brake_light_update(s32,f32 *,Camera *,void *,s16 *);
extern s16 sound_pitch_diff_halved(void *,s16);
extern s16 object_bytes_sum_global(void);
extern void state_utility(s16,s16,void *);
extern void fcvt_wrapper(char *,char *,...);
extern s8 func_8038F744(s32);
extern void func_800ED66C(f32);
extern void func_800BEA3C(Color4,Color4);
extern void func_800B7360(u8,u8,u8,u8);

void func_803ADDA8(void)
{
    s32 j,x;
    f32 y;
    Slot60 *slot;
    s32 i;
    char text[48];
    s16 point[2];
    render_helper(0.0f);
    func_800B669C(0,1);
    brake_light_update(0,D_803B68D4,&D_80150B70[0],0,point);
    x=point[0]; y=point[1];
    object_create(13);
    dispatch_handler(1);
    state_utility(sound_pitch_diff_halved(countdown_state.text[39],x),(s16)y,countdown_state.text[39]);
    object_create(10);
    for(i=0;i<10;i++) {
        if(!func_8038F744(i))continue;
        slot=&D_803B6610[i];
        if(slot->angle>0.6981316804885864f && slot->angle<2.4434609413146973f)continue;
        if(slot->angle>0.3490658402442932f && slot->angle<2.7925267219543457f)func_800ED66C(128.0f);
        else func_800ED66C(-1.0f);
        if(slot->angle<1.5707963705062866f) {
            func_800BEA3C(D_801146BC,D_801146C0);
            func_800B7360(224,224,224,255);
        } else {
            func_800BEA3C(D_801146BC,D_801146C0);
            dispatch_handler(22);
        }
        brake_light_update(0,slot->position,&D_80150B70[0],0,point);
        x=point[0]-35; y=point[1];
        state_utility(x,(s16)y,countdown_state.indexed[countdown_state.indices[20]+i]);
        if(i==0) {
            brake_light_update(0,D_803B6898,&D_80150B70[0],0,point);
            x=point[0]-18; y=point[1];
            for(j=0;j<4;j++) {
                fcvt_wrapper(text,D_803B8CDC,j+1);
                if(D_803B6A68==0) {
                    if(j>=D_803B6A6C)dispatch_handler(3);
                    else if(j+1==D_8014A108)dispatch_handler(22);
                    else dispatch_handler(0);
                } else {
                    if(j>=D_803B6A6C)dispatch_handler(3);
                    else dispatch_handler(0);
                }
                state_utility(sound_pitch_diff_halved(text,x),(s16)y,text);
                x+=16;
            }
        }
    }
    func_800ED66C(-1.0f);
    func_800B669C(0,3);
    func_800BEA3C(D_801146BC,D_801146C0);
    render_helper(-1.0f);
}
void func_803ADDA8(void);
s32 func_803AE1C0(void *callback_context)
{
    s32 i;
    char text[48];
    s32 j,x;
    f32 y;
    if(D_803B6A48==1) {
        func_803ADDA8();
        return 1;
    }
    render_helper(0.0f);
    func_800B669C(1,3);
    object_create(13);
    dispatch_handler(1);
    y=10.0f;
    state_utility(160,(s16)y,countdown_state.text[39]);
    object_bytes_sum_global();
    object_create(10);
    y=40.0f;
    for(i=0;i<10;i++) {
        if(!func_8038F744(i))continue;
        if(i==D_803B6A68)dispatch_handler(22);
        else func_800B7360(224,224,224,255);
        state_utility(160,(s16)y,countdown_state.indexed[countdown_state.indices[20]+i]);
        if(i==0) {
            x=230;
            for(j=0;j<4;j++) {
                fcvt_wrapper(text,D_803B8CD8,j+1);
                if(D_803B6A68==0) {
                    if(j+1==D_8014A108)dispatch_handler(22);
                    else func_800B7360(64,64,64,255);
                } else if(j+1==D_8014A108) {
                    func_800B7360(0,0,0,255);
                    state_utility(x+1,(s16)(y+1.0f),text);
                    func_800B7360(224,224,224,255);
                } else {
                    func_800B7360(64,64,64,255);
                }
                state_utility(x,(s16)y,text);
                x+=16;
            }
        }
        y+=22.5f;
    }
    func_800ED66C(-1.0f);
    func_800B669C(0,3);
    func_800BEA3C(D_801146BC,D_801146C0);
    render_helper(-1.0f);
    return 1;
}

extern s8 D_803B65E4[];
extern s32 D_80156944;
extern s8 D_801164A1;

s8 func_8038F744(s32 index)
{
    s8 *entry = &D_803B65E4[index];
    s32 value = *entry;

    if (index == 7 && !(D_80156944 & 0x100)) {
        value = 0;
    }
    if (index == 6) {
        value = D_801164A1;
    }
    return value;
}
