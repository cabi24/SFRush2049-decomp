/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image-A aggregate statistics ABEC8: separate race/track/record pointers and
 * scan/display counters follow native lifetimes. The scaled quotient is a
 * shared expression; useful locals retain native declaration order. Buffer
 * text[48] is unchanged. Stock frame152, native text at sp+72 and x/y at
 * sp+146/sp+144. No new padding, unused local, forced register, volatile,
 * helper or call convention. All ten inherited members are unchanged. */
/* Image-A per-track statistics AD0D4: preserve the native zero-initialized
 * average and conditional division, unsigned-halfword bit-count arguments,
 * and local declaration order (x/y at sp+132/128, text at sp+76).
 * Its real text buffer remains 48 bytes; frame remains 144 bytes. No unused
 * local or artificial access is introduced. Original spelling is unknown.
 * All nine earlier exact statistics members remain unchanged and claimed. */
/* Image-A cumulative statistics ACA94: row-count initialization follows the
 * two native setup calls; bit masks retain unsigned-halfword narrowing.
 * Existing declaration order recovers native text/x/y homes without changing
 * the real 48-byte text buffer. One disclosed unused s32 precedes all locals:
 * stock UGEN M88 -> M92, T20/P4 unchanged, aligned frame120 ->128.
 * Without it x/y/text homes are four bytes low. Original local identity is
 * unknown; this is the exact Wave16 frame residual, not inferred behavior.
 * All eight earlier exact #349 bodies remain unchanged and claimed. */
/* Image-A unlock panel AB848: all six unsigned maximum-result arms retain
 * signed-halfword narrowing. Flag initialization follows profile restoration;
 * the first scan exits early for locked entries. The existing first flag
 * precedes saved_owner in the declaration order. The real 44-byte buffer is
 * safely above the 24-byte maximum across all six authentic languages;
 * original capacity and source types remain unknown. Profile setup writes
 * its pointer before count. No artificial local or helper/signature change.
 * Seven prior exact members from draft #344 stay claimed without new credit. */
/* Image-A extra-car panel AB5FC: two unsigned maximum-result arms retain
 * native signed-halfword narrowing, and profile-pointer setup precedes the
 * temporary one-player count. No signature, local capacity, helper body,
 * private call convention or other function is changed by this continuation.
 * Six prior exact candidates from draft #341 remain in their original group.
 * Their inherited numeric buffers are 40 bytes, safely above the 12/10/6-byte
 * write bounds for AC54C/AC7F4/ACD5C. Capacities and original types are inferred.
 * Only the genuine AD57C root is kept. No ROM or accepted-image claim. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Language { s32 unknown0; u8 **text; s32 unknown8; u16 *indices; u8 **indexed; } Language;
typedef struct Color4 { u32 word; } Color4;
typedef struct Slot { s32 kind,handle; f32 angle,matrix[3][3],position[3]; u8 alpha,unknown3D[3]; } Slot;
typedef struct Camera { f32 uv[3][3],position[3]; u8 unknown30[104]; } Camera;
typedef struct FileInfo { u8 unknown00[16],controller; } FileInfo;
typedef struct Profile Profile;
struct Profile { Profile **next,**previous; FileInfo **file; u8 unknown0C[8]; u8 name[24]; u8 **stats; };
typedef struct Player { u8 unknown00[72]; Profile **owner; } Player;
typedef struct FiveParts FiveParts;
typedef struct TextBox {s16 x,y,width,height;} TextBox;
extern Language countdown_state;
extern Slot D_803B5F28[];
extern Camera D_80150B70[];
extern Color4 D_801146BC,D_801146C0;
extern Player D_8014A118[];
extern s16 D_8014A108;
extern Profile **D_803BA800,**D_8012E6E0;
extern s32 D_803BA804,D_803BA808,D_803BA818[],D_803BA830[],D_803BA880;
extern s8 D_803BA84D,D_803BA850,D_803BA854,D_803BA858,D_803BA860,D_803B65A4,D_80146111;
extern u8 D_803BA840[];
extern f32 D_803B64E8,D_803B64FC,D_803BA90C;
extern TextBox D_803B8ACC;
extern FiveParts *D_803B6530;
extern u8 *D_8011EAD0[],*D_8011A8D8[],*D_80110D70[];
extern u8 D_80151410[],D_80150F88[],D_80151578[],D_801515F8[],D_80151618[];
extern s8 D_80150DD8[],D_80150E88[],D_80150E30[],D_80150ED8[],D_80150F40[],D_80150EB8[],D_80150F00[];
extern void render_helper(f32);
extern s32 object_create(s32);
extern void dispatch_handler(s32);
extern void func_800B669C(u32,u32);
extern void brake_light_update(s32,f32 *,Camera *,void *,s16 *);
extern s16 sound_pitch_diff_halved(void *,s16);
extern s16 sound_pitch_diff_calc(void *,s16);
extern s16 object_bytes_sum_global(void);
extern s32 object_manager_update(u8 *,s16);
extern void state_utility(s16,s16,void *);
extern void fcvt_wrapper(u8 *,const u8 *,...);
extern void func_800ED66C(f32);
extern void func_800BEA3C(Color4,Color4);
extern void func_800F6928(f32);
extern void mode_byte_set(s32);
extern void camera_auto_follow(s16,s16,s16,s16,s16,s16,u8 *);
extern s16 camera_zoom_fov(s16,u8 *);
extern void music_fade(s16,s16,s16,void *);
extern void crowd_cheer_play(FiveParts *,s32,s32,s32,s32);
extern u8 *func_800BE6A4(u8 *,u8 *);
extern void net_state_validate(void);
extern s32 func_800B78A4(u32,u8);
#define ABS(x) ((x)<0 ? -(x) : (x))
#define U16_AT(p,o) (*(u16 *)((u8 *)(p)+(o)))
#define U32_AT(p,o) (*(u32 *)((u8 *)(p)+(o)))
#define F32_AT(p,o) (*(f32 *)((u8 *)(p)+(o)))
#define NAME(g,i) (countdown_state.indexed[countdown_state.indices[g]+(i)])
#define PROFILE_DATA (*(*D_803BA800)->stats)
extern u8 D_803B8BF4[],D_803B8C00[],D_803B8B28[],D_803B8B2C[],D_803B8B30[],D_803B8B34[],D_803B8B38[],D_803B8B3C[],D_803B8B40[],D_803B8B44[],D_803B8B48[],D_803B8B4C[],D_803B8B50[],D_803B8B58[],D_803B8B60[],D_803B8B64[],D_803B8B68[],D_803B8B6C[],D_803B8B70[],D_803B8B74[],D_803B8B78[],D_803B8B7C[],D_803B8B80[],D_803B8B84[],D_803B8B88[],D_803B8B8C[],D_803B8B90[],D_803B8B94[],D_803B8B98[],D_803B8B9C[],D_803B8BA8[],D_803B8BB4[],D_803B8BB8[],D_803B8BBC[],D_803B8BC4[],D_803B8BCC[],D_803B8BD4[],D_803B8BDC[],D_803B8AE8[],D_803B8AEC[],D_803B8AF0[],D_803B8AF4[],D_803B8AF8[],D_803B8AFC[],D_803B8B00[],D_803B8B04[],D_803B8B08[],D_803B8B0C[],D_803B8B18[],D_803B8B20[],D_803B8AD8[],D_803B8AE0[];

/* Every genuine caller writes zero in outgoing word +8. Its original role is
 * unknown; the observed specialized body does not read it. */
void func_803AC330(u8 *destination,f32 time,s32 unused_format)
{
    if(time==0.0f)func_800BE6A4(destination,D_803B8BF4);
    else {
        D_803BA90C=time;
        fcvt_wrapper(destination,D_803B8C00,(u32)(time/60.0f)%100,(u32)time%60,(u32)(time*1000.0f)%1000);
    }
}
void func_803AA3D8(void)
{
    s32 pad;
    s32 i, x, y, cx;
    u8 *str;
    u8 str1[64];
    s16 screenPos[2];
    func_800B669C(0, 1);
    brake_light_update(0, D_803B5F28[17].position, &D_80150B70[0], 0, screenPos);
    cx = screenPos[0];
    y = screenPos[1];
    func_800F6928(D_803B5F28[17].position[2] - 10.0f);
    dispatch_handler(1);
    object_create(13);
    dispatch_handler(1);
    str = countdown_state.text[47];
    state_utility(sound_pitch_diff_halved(str, cx), y, str);
    y += object_bytes_sum_global() * 2;
    object_create(10);
    if (D_803BA84D == 1) {
        render_helper(0.0f);
        y += object_bytes_sum_global();
        if (D_803BA800) {
            fcvt_wrapper(str1, countdown_state.text[143], D_803BA840);
            str = str1;
        } else str = countdown_state.text[144];
        crowd_cheer_play(D_803B6530, cx - D_803B8ACC.width / 2 - 4, y - 4,
                         cx + D_803B8ACC.width / 2 + 4, 4 + y + ((camera_zoom_fov(D_803B8ACC.width, str) + 2) * object_bytes_sum_global()));
        dispatch_handler(1);
        func_800B669C(1, 3);
        camera_auto_follow(cx, y, D_803B8ACC.width, D_803B8ACC.height, -1, 0, str);
        func_800B669C(0, 3);
        y += object_bytes_sum_global() * (camera_zoom_fov(D_803B8ACC.width, str) + 1);
        str = NAME(24, 0);
        dispatch_handler(D_803BA850 == 1 ? 22 : 1);
        state_utility(sound_pitch_diff_calc(str, cx - 5), y, str);
        str = NAME(24, 1);
        dispatch_handler(D_803BA850 == 0 ? 22 : 1);
        state_utility(cx + 5, y, str);
    } else if (D_803BA854 == 1) {
        render_helper(0.0f);
        y += object_bytes_sum_global();
        if (D_803BA800) {
            fcvt_wrapper(str1, countdown_state.text[146], D_803BA840);
            str = str1;
        } else str = countdown_state.text[147];
        crowd_cheer_play(D_803B6530, cx - D_803B8ACC.width / 2 - 4, y - 4,
                         cx + D_803B8ACC.width / 2 + 4, 4 + y + (camera_zoom_fov(D_803B8ACC.width, str) * object_bytes_sum_global()));
        dispatch_handler(1);
        func_800B669C(1, 3);
        camera_auto_follow(cx, y, D_803B8ACC.width, D_803B8ACC.height, -1, 0, str);
        func_800B669C(0, 3);
    } else {
        for (i = 0; i < 11; i++) {
            crowd_cheer_play(D_803B6530, -20, 0, -10, 10);
            func_800F6928(D_803B5F28[i].position[2] - 10.0f);
            brake_light_update(0, D_803B5F28[i].position, &D_80150B70[0], 0, screenPos);
            if (D_803B5F28[i].kind == 11) x = screenPos[0];
            else if (D_803B5F28[i].kind == 9) x = screenPos[0] - 65.0f * D_803B64E8 * 300.0f / D_803B64FC;
            else x = screenPos[0] - 45.0f * D_803B64E8 * 300.0f / D_803B64FC;
            y = screenPos[1];
            if (i == 1) {
                brake_light_update(0, D_803B5F28[14].position, &D_80150B70[0], 0, screenPos);
                cx = screenPos[0] - 25.0f * D_803B64E8 * 300.0f / D_803B64FC;
            } else {
                brake_light_update(0, D_803B5F28[13].position, &D_80150B70[0], 0, screenPos);
                cx = screenPos[0] - 45.0f * D_803B64E8 * 300.0f / D_803B64FC;
            }
            if (i >= 3 && i < 11 && i == D_803BA808 &&
                (D_803B5F28[13].angle < 0.6981317f || D_803B5F28[13].angle > 2.443461f) && D_803B5F28[13].alpha) {
                func_800ED66C(D_803B5F28[13].alpha);
                if (D_803B5F28[13].angle < 1.5707964f) {
                    func_800BEA3C(D_801146BC, D_801146C0);
                    dispatch_handler(1);
                } else {
                    func_800BEA3C(D_801146BC, D_801146C0);
                    dispatch_handler(22);
                }
                str = NAME(50, 2);
                object_create(11);
                state_utility(cx, y, str);
                object_create(10);
            }
            if (D_803B5F28[i].angle > 0.6981317f && D_803B5F28[i].angle < 2.443461f) continue;
            if (!D_803B5F28[i].alpha) continue;
            func_800ED66C(D_803B5F28[i].alpha);
            if (D_803B5F28[i].angle < 1.5707964f) {
                func_800BEA3C(D_801146BC, D_801146C0);
                dispatch_handler(1);
            } else {
                func_800BEA3C(D_801146BC, D_801146C0);
                dispatch_handler(22);
            }
            if (i == 0) {
                str = NAME(50, 0);
                state_utility(x, y, str);
            } else if (i == 1) {
                str = NAME(50, 1);
                state_utility(x, y, str);
                str = D_8011EAD0[D_803B65A4];
                state_utility(cx, y, str);
            } else if (i == 2) {
                switch (D_803BA830[D_803B65A4]) {
                case -4: str = countdown_state.text[165]; break;
                case -3: str = countdown_state.text[169]; break;
                case 0: str = countdown_state.text[148]; break;
                case -2: str = countdown_state.text[166]; break;
                case -1: str = countdown_state.text[168]; break;
                }
                dispatch_handler(0);
                state_utility(sound_pitch_diff_halved(str, x), y, str);
            } else if (i >= 3 && i < 11) {
                Profile **player;
                s32 playerIdx;
                player = D_803BA800;
                if (!player) {
                    for (playerIdx = 0, player = D_8012E6E0; player; player = (*player)->next) {
                        if ((*(*player)->file)->controller == D_803B65A4) {
                            if (playerIdx == i - 3) str = (*player)->name;
                            playerIdx++;
                        }
                    }
                } else {
                    for (playerIdx = 0; player; ) {
                        if ((*(*player)->file)->controller == D_803B65A4) {
                            if (playerIdx == (i - D_803BA808 >= 0 ? i - D_803BA808 : -(i - D_803BA808))) str = (*player)->name;
                            playerIdx++;
                        }
                        player = i < D_803BA808 ? (*player)->previous : (*player)->next;
                    }
                }
                if ((u32)object_manager_update(str, -1) > 140) mode_byte_set(0);
                music_fade(x, y, 98, str);
                mode_byte_set(-1);
            }
            object_bytes_sum_global();
        }
    }
}
void func_803AAF0C(void)
{
    Profile **saved_owner;
    s16 saved_players,i,y,bottom;
    s32 all_empty=1,empty;
    u8 *label;
    if(!D_803BA800)return;
    func_800B669C(0,3);
    saved_owner=D_8014A118[0].owner;saved_players=D_8014A108;
    D_8014A108=1;D_8014A118[0].owner=D_803BA800;
    net_state_validate();
    D_8014A118[0].owner=saved_owner;D_8014A108=saved_players;
    dispatch_handler(1);
    object_create(11);
    y=90;
    label=NAME(34,0);
    state_utility(sound_pitch_diff_halved(label,80),y,label);
    y+=object_bytes_sum_global();object_create(12);empty=1;
    for(i=3;i<9;i++)if(D_80150ED8[i]) {
        empty=0;label=NAME(6,i);
        state_utility(sound_pitch_diff_halved(label,80),y,label);y+=object_bytes_sum_global();
    }
    if(empty==1) {
        label=NAME(15,0);
        state_utility(sound_pitch_diff_halved(label,80),y,label);y+=object_bytes_sum_global();
    } else all_empty=0;
    y+=object_bytes_sum_global();
    object_create(11);
    label=NAME(34,1);
    state_utility(sound_pitch_diff_halved(label,80),y,label);
    y+=object_bytes_sum_global();object_create(12);empty=1;
    for(i=3;i<6;i++)if(D_80150F40[i]) {
        empty=0;label=NAME(8,i);
        state_utility(sound_pitch_diff_halved(label,80),y,label);y+=object_bytes_sum_global();
    }
    if(empty==1) {
        label=NAME(15,0);
        state_utility(sound_pitch_diff_halved(label,80),y,label);y+=object_bytes_sum_global();
    } else all_empty=0;
    bottom=y+4;
    object_create(11);
    y=90;
    label=NAME(34,2);
    state_utility(sound_pitch_diff_halved(label,240),y,label);
    y+=object_bytes_sum_global();object_create(12);empty=1;
    for(i=2;i<8;i++)if(D_80150EB8[i]) {
        empty=0;label=NAME(4,i);
        state_utility(sound_pitch_diff_halved(label,240),y,label);y+=object_bytes_sum_global();
    }
    if(empty==1) {
        label=NAME(15,0);
        state_utility(sound_pitch_diff_halved(label,240),y,label);y+=object_bytes_sum_global();
    } else all_empty=0;
    y+=object_bytes_sum_global();
    object_create(11);
    label=NAME(34,3);
    state_utility(sound_pitch_diff_halved(label,240),y,label);
    y+=object_bytes_sum_global();object_create(12);empty=1;
    for(i=1;i<5;i++)if(D_80150F00[i]) {
        empty=0;label=NAME(7,i);
        state_utility(sound_pitch_diff_halved(label,240),y,label);y+=object_bytes_sum_global();
    }
    if(empty==1) {
        label=NAME(15,0);
        state_utility(sound_pitch_diff_halved(label,240),y,label);y+=object_bytes_sum_global();
    } else all_empty=0;
    if(all_empty) {
        y+=object_bytes_sum_global();
        state_utility(sound_pitch_diff_halved(countdown_state.text[62],160),y,countdown_state.text[62]);
        y+=object_bytes_sum_global();
    }
    if(y+4>=bottom)bottom=y+4;
    crowd_cheer_play(D_803B6530,12,86,308,bottom);
}
void func_803AB848(void)
{
    s32 first;
    Profile **saved_owner;
    s16 saved_players,i,x,y,bottom,width=20;
    s32 second;
    u8 text[44];
    if(!D_803BA800)return;
    func_800B669C(0,3);
    saved_owner=D_8014A118[0].owner;saved_players=D_8014A108;
    D_8014A118[0].owner=D_803BA800;D_8014A108=1;
    net_state_validate();
    D_8014A118[0].owner=saved_owner;D_8014A108=saved_players;
    first=0;second=0;
    for(i=0;i<19;i++) {
        if(i==0 || i==1 || i==2 || i==3 || i==14 || i==6 || i==7 || i==8 || i==9)continue;
        if(!D_80150DD8[i])continue; if(i<14)first=1;else second=1;
    }
    for(i=1;i<4;i++)if(D_80150E88[i])second=1;
    object_create(12);dispatch_handler(1);
    y=90;x=first && second ? 80 : 160;bottom=90;
    for(i=0;i<19;i++) {
        if(i==14) {x=first && second ? 240 : 160;if(first && second)y=90;}
        if(i==0 || i==1 || i==2 || i==3 || i==14 || i==6 || i==7 || i==8 || i==9)continue;
        if(D_80150DD8[i]) {
            width=(u32)object_manager_update(D_8011A8D8[i],-1)<(u32)width ? (u32)width : (u32)object_manager_update(D_8011A8D8[i],-1);
            state_utility(sound_pitch_diff_halved(D_8011A8D8[i],x),y,D_8011A8D8[i]);
            y+=object_bytes_sum_global();
        }
        if(y>=bottom)bottom=y;
    }
    for(i=1;i<4;i++)if(D_80150E88[i]) {
        fcvt_wrapper(text,D_803B8BDC,NAME(44,i),countdown_state.text[42]);
        width=(u32)object_manager_update(text,-1)<(u32)width ? (u32)width : (u32)object_manager_update(text,-1);
        state_utility(sound_pitch_diff_halved(text,x),y,text);y+=object_bytes_sum_global();
    }
    if(!first && !second) {
        width=(u32)object_manager_update(countdown_state.text[57],-1)<(u32)width ? (u32)width : (u32)object_manager_update(countdown_state.text[57],-1);
        state_utility(sound_pitch_diff_halved(countdown_state.text[57],160),y,countdown_state.text[57]);y+=object_bytes_sum_global();
        width=(u32)object_manager_update(countdown_state.text[58],-1)<(u32)width ? (u32)width : (u32)object_manager_update(countdown_state.text[58],-1);
        state_utility(sound_pitch_diff_halved(countdown_state.text[58],160),y,countdown_state.text[58]);y+=object_bytes_sum_global();
        width=(u32)object_manager_update(countdown_state.text[59],-1)<(u32)width ? (u32)width : (u32)object_manager_update(countdown_state.text[59],-1);
        state_utility(sound_pitch_diff_halved(countdown_state.text[59],160),y,countdown_state.text[59]);y+=object_bytes_sum_global();
        width=(u32)object_manager_update(countdown_state.text[60],-1)<(u32)width ? (u32)width : (u32)object_manager_update(countdown_state.text[60],-1);
        state_utility(sound_pitch_diff_halved(countdown_state.text[60],160),y,countdown_state.text[60]);y+=object_bytes_sum_global();
    } else if(first && second)width+=160;
    if(y>=bottom)bottom=y;
    crowd_cheer_play(D_803B6530,152-width/2,86,168+width/2,bottom+4);
}
void func_803AC330(u8 *,f32,s32);
void func_803AD0D4(void)
{
    s32 track,i,x,y;
    u8 *label,text[48],*data;
    u32 distance;
    f32 average;
    func_800B669C(0,3);
    track=D_803BA858/2;
    if(D_803BA858&1)track+=6;
    if(D_803BA800)data=PROFILE_DATA+140+track*96;
    else data=D_80150F88+track*96;
    distance=U32_AT(data,88);
    if(D_80146111)distance=distance*10/6;
    object_create(11);
    crowd_cheer_play(D_803B6530,12,86,308,94+object_bytes_sum_global()*8);
    dispatch_handler(1);
    for(i=0;i<14;i++) {
        if(i==0) {x=100;y=90;}
        else if(i==3) {x=110;y+=object_bytes_sum_global();}
        else if(i==7) {x=270;y=90+object_bytes_sum_global()*4;}
        else if(i==10) {x=260;y=90;continue;}
        else if((i==12 || i==13) && !D_803BA800)continue;
        label=NAME(28,i);
        if(i==11 && D_80146111)label=NAME(28,14);
        state_utility(sound_pitch_diff_calc(label,x-4),y,label);
        switch(i) {
        case 0:func_803AC330(text,F32_AT(data,64),0);break;
        case 1:fcvt_wrapper(text,D_803B8AE8,U16_AT(data,68));break;
        case 2:
            average=0.0f;
            if(U16_AT(data,68)>0)
                average=F32_AT(data,64)/(f32)(u32)U16_AT(data,68);
            func_803AC330(text,average,0);
            break;
        case 3:fcvt_wrapper(text,D_803B8AEC,U16_AT(data,70));break;
        case 4:fcvt_wrapper(text,D_803B8AF0,U16_AT(data,72));break;
        case 5:fcvt_wrapper(text,D_803B8AF4,U16_AT(data,74));break;
        case 6:fcvt_wrapper(text,D_803B8AF8,U16_AT(data,76));break;
        case 7:fcvt_wrapper(text,D_803B8AFC,U16_AT(data,78));break;
        case 8:fcvt_wrapper(text,D_803B8B00,U16_AT(data,80));break;
        case 9:fcvt_wrapper(text,D_803B8B04,U16_AT(data,82));break;
        case 10:fcvt_wrapper(text,D_803B8B08,U16_AT(data,84));break;
        case 11: {u32 divisor=10;fcvt_wrapper(text,D_803B8B0C,distance/divisor,distance%divisor);break;}
        case 12:fcvt_wrapper(text,D_803B8B18,func_800B78A4((u16)(U16_AT(data,92)&255),16));break;
        case 13:fcvt_wrapper(text,D_803B8B20,func_800B78A4((u16)(U16_AT(data,92)&65280),16));break;
        }
        if(i==0 || i==2)object_create(2);
        state_utility(x+4,y,text);
        if(i==0 || i==2)object_create(11);
        y+=object_bytes_sum_global();
    }
}
void func_803ABEC8(void)
{
    u32 divisor;
    s16 x,y;
    u32 distance=0;
    u32 total=0;
    u16 found=0,race_a=0,race_b=0,track_a=0,track_b=0;
    s16 i,row;
    u8 text[48],*label,*data,*record,*track;
    if(!D_803BA800)return;
    func_800B669C(0,3);
    for(i=0;i<12;i++) {
        data=PROFILE_DATA+140+i*96;
        distance+=U32_AT(data,88);
        if(i<6) {
            race_a+=func_800B78A4(U16_AT(data,92)&255,16);
            race_b+=func_800B78A4(U16_AT(data,92)&65280,16);
        }
    }
    for(i=0;i<4;i++) {
        track=PROFILE_DATA+1292+i*64;
        total+=U32_AT(track,12);
        track_a+=func_800B78A4(U16_AT(track,60)&255,16);
        track_b+=func_800B78A4(U16_AT(track,60)&65280,16);
    }
    for(i=0;i<8;i++) {
        record=PROFILE_DATA+1548+i*12;
        found+=U16_AT(record,8);
    }
    object_create(11);dispatch_handler(1);
    divisor=10;
    for(row=0;row<7;row++) {
        if(row==0) {x=180;y=90;}
        label=NAME(33,row);
        if(row==0 && D_80146111)label=NAME(28,7);
        state_utility(sound_pitch_diff_calc(label,x-4),y,label);
        switch(row) {
        case 0:
            if(D_80146111) {
                fcvt_wrapper(text,D_803B8B9C,(distance*divisor/6)/divisor,(distance*divisor/6)%divisor);
            } else fcvt_wrapper(text,D_803B8BA8,distance/divisor,distance%divisor);
            break;
        case 1:fcvt_wrapper(text,D_803B8BB4,total);break;
        case 2:fcvt_wrapper(text,D_803B8BB8,found);break;
        case 3:fcvt_wrapper(text,D_803B8BBC,race_a,48);break;
        case 4:fcvt_wrapper(text,D_803B8BC4,race_b,48);break;
        case 5:fcvt_wrapper(text,D_803B8BCC,track_a,32);break;
        case 6:fcvt_wrapper(text,D_803B8BD4,track_b,32);break;
        }
        state_utility(x+4,y,text);y+=object_bytes_sum_global();
    }
    crowd_cheer_play(D_803B6530,12,86,308,94+object_bytes_sum_global()*7);
}
void func_803AB5FC(void)
{
    Profile **saved_owner;
    s16 saved_players,i,y,width=20;
    if(!D_803BA800)return;
    func_800B669C(0,3);
    saved_owner=D_8014A118[0].owner;saved_players=D_8014A108;
    D_8014A118[0].owner=D_803BA800;D_8014A108=1;
    net_state_validate();
    D_8014A118[0].owner=saved_owner;D_8014A108=saved_players;
    object_create(12);dispatch_handler(1);y=90;
    for(i=6;i<13;i++) {
        if(i==0 || i==1 || i==2 || i==3 || i==4 || i==5)continue;
        if(D_80150E30[i]) {
            width=(u32)object_manager_update(D_80110D70[i],-1)<(u32)width ? (u32)width : (u32)object_manager_update(D_80110D70[i],-1);
            state_utility(sound_pitch_diff_halved(D_80110D70[i],160),y,D_80110D70[i]);
            y+=object_bytes_sum_global();
        }
    }
    if(y==90) {
        width=(u32)object_manager_update(countdown_state.text[61],-1)<(u32)width ? (u32)width : (u32)object_manager_update(countdown_state.text[61],-1);
        state_utility(sound_pitch_diff_halved(countdown_state.text[61],160),y,countdown_state.text[61]);
        y+=object_bytes_sum_global();
    }
    crowd_cheer_play(D_803B6530,152-width/2,86,168+width/2,y+4);
}
void func_803AC330(u8 *,f32,s32);
void func_803ACF5C(s32 category)
{
    s32 track,y,i;
    u8 *data;
    u8 text[48];
    func_800B669C(1,3);
    track=D_803BA858/2;
    if(D_803BA858&1)track+=6;
    if(D_803BA800)data=PROFILE_DATA+140+track*96;
    else data=D_80150F88+track*96;
    object_create(11);dispatch_handler(1);
    state_utility(160,90,NAME(27,category));
    y=90+object_bytes_sum_global();
    object_create(2);
    for(i=0;i<5;i++) {
        func_803AC330(text,F32_AT(data,4+category*20+i*4),0);
        state_utility(160,y,text);
        y+=object_bytes_sum_global();
    }
    crowd_cheer_play(D_803B6530,100,86,220,94+object_bytes_sum_global()*6);
}
void func_803ACD5C(void)
{
    u8 *data;
    s32 x,y;
    u8 *label;
    s32 i;
    u8 text[40];
    if(D_803BA800)data=PROFILE_DATA+1404+D_803BA858*12;
    else data=D_80151578+D_803BA858*12-144;
    object_create(11);dispatch_handler(1);
    for(i=0;i<3;i++) {
        if(i==0) {x=190;y=90;}
        label=NAME(29,i);
        state_utility(sound_pitch_diff_calc(label,x-4),y,label);
        switch(i) {
        case 0:fcvt_wrapper(text,D_803B8B28,U16_AT(data,4));break;
        case 1:fcvt_wrapper(text,D_803B8B2C,U16_AT(data,6));break;
        case 2:fcvt_wrapper(text,D_803B8B30,U16_AT(data,8));break;
        case 3:fcvt_wrapper(text,D_803B8B34,U16_AT(data,10));break;
        }
        state_utility(x+4,y,text);
        y+=object_bytes_sum_global();
    }
    crowd_cheer_play(D_803B6530,80,86,240,y+4);
}
void func_803ACA94(void)
{
    s32 unresolved_local;
    s32 x,y;
    u8 *label,text[48],*data;
    s32 i,rows;
    if(D_803BA800)data=PROFILE_DATA+12+D_803BA858*64;
    else data=D_80151410+D_803BA858*64-1280;
    object_create(11);dispatch_handler(1);
    rows=0;
    for(i=0;i<8;i++) {
        if(i==0) {x=190;y=90;}
        else if(i==4 || i==5)continue;
        else if((i==6 || i==7) && !D_803BA800)continue;
        label=NAME(30,i);
        state_utility(sound_pitch_diff_calc(label,x-4),y,label);
        switch(i) {
        case 0:fcvt_wrapper(text,D_803B8B38,U32_AT(data,4));break;
        case 1:fcvt_wrapper(text,D_803B8B3C,U32_AT(data,8));break;
        case 2:fcvt_wrapper(text,D_803B8B40,U32_AT(data,12));break;
        case 3:fcvt_wrapper(text,D_803B8B44,U16_AT(data,16));break;
        case 4:fcvt_wrapper(text,D_803B8B48,U16_AT(data,18));break;
        case 5:fcvt_wrapper(text,D_803B8B4C,U32_AT(data,20));break;
        case 6:fcvt_wrapper(text,D_803B8B50,func_800B78A4((u16)(U16_AT(data,60)&255),16));break;
        case 7:fcvt_wrapper(text,D_803B8B58,func_800B78A4((u16)(U16_AT(data,60)&65280),16));break;
        }
        state_utility(x+4,y,text);y+=object_bytes_sum_global();rows++;
    }
    crowd_cheer_play(D_803B6530,80,86,240,94+object_bytes_sum_global()*rows);
}
void func_803AC54C(void)
{
    u8 *data;
    s32 x,y;
    u8 *label;
    s32 i;
    u8 text[40];
    if(D_803BA800)data=PROFILE_DATA+1668+D_803BA860*28;
    else data=D_80151618+D_803BA860*28;
    object_create(11);dispatch_handler(1);
    for(i=0;i<8;i++) {
        if(i==0) {x=180;y=90;}
        label=NAME(32,i);
        state_utility(sound_pitch_diff_calc(label,x-4),y,label);
        switch(i) {
        case 0:fcvt_wrapper(text,D_803B8B7C,U16_AT(data,4));break;
        case 1:fcvt_wrapper(text,D_803B8B80,U16_AT(data,6));break;
        case 2:fcvt_wrapper(text,D_803B8B84,U16_AT(data,8));break;
        case 3:fcvt_wrapper(text,D_803B8B88,U16_AT(data,10));break;
        case 4:fcvt_wrapper(text,D_803B8B8C,U16_AT(data,12));break;
        case 5:fcvt_wrapper(text,D_803B8B90,U16_AT(data,14));break;
        case 6:fcvt_wrapper(text,D_803B8B94,U16_AT(data,16));break;
        case 7:func_803AC330(text,F32_AT(data,20),0);break;
        case 8:fcvt_wrapper(text,D_803B8B98,U32_AT(data,24));break;
        }
        if(i==7)object_create(2);
        state_utility(x+4,y,text);
        if(i==7)object_create(11);
        y+=object_bytes_sum_global();
    }
    crowd_cheer_play(D_803B6530,50,86,270,y+4);
}
void func_803AC7F4(void)
{
    u8 *data;
    s32 x,y;
    u8 *label;
    s32 i;
    u8 text[40];
    if(D_803BA800)data=PROFILE_DATA+1068+D_803BA858*24;
    else data=D_801515F8+D_803BA858*24-576;
    object_create(11);dispatch_handler(1);
    for(i=0;i<4;i++) {
        if(i==0) {x=190;y=90;}
        else if(i==4)continue;
        label=NAME(31,i);
        state_utility(sound_pitch_diff_calc(label,x-4),y,label);
        switch(i) {
        case 0:func_803AC330(text,F32_AT(data,4),0);break;
        case 1:func_803AC330(text,F32_AT(data,8),0);break;
        case 2:fcvt_wrapper(text,D_803B8B68,U16_AT(data,12));break;
        case 3:fcvt_wrapper(text,D_803B8B6C,U16_AT(data,14));break;
        case 4:fcvt_wrapper(text,D_803B8B70,U16_AT(data,16));break;
        case 5:fcvt_wrapper(text,D_803B8B74,U16_AT(data,18));break;
        case 6:fcvt_wrapper(text,D_803B8B78,U16_AT(data,20));break;
        }
        if(i==0 || i==1)object_create(2);
        state_utility(x+4,y,text);
        if(i==0 || i==1)object_create(11);
        y+=object_bytes_sum_global();
    }
    crowd_cheer_play(D_803B6530,40,86,280,94+object_bytes_sum_global()*4);
}
void func_803AA3D8(void);
void func_803AAF0C(void);
void func_803AB5FC(void);
void func_803AB848(void);
void func_803ABEC8(void);
void func_803AC54C(void);
void func_803AC7F4(void);
void func_803ACA94(void);
void func_803ACD5C(void);
void func_803ACF5C(s32);
void func_803AD0D4(void);
/* Image-A recordscr animText callback AD57C (GC recordscr.c animText 0x8008451C): HAL_SetTextDepth(0); switch (gRecordState)
 * { case 0: drawSelect() [stays the call func_803AA3D8]; case 1: drawView() [inlined by umerge] } HAL_SetTextDepth(-1); return 1.
 * drawView is a direct typed MAT3 port (GC 0x80081C78): buttonInfo[i + 11] indexing, locals i,x,y,cx,str,str1[64],screenPos[2]
 * in GC order, `str` holds every label (title, both button labels, the track/stat names).  The kViewStuntTotals arm is the
 * inlined drawStuntTotals (GC 0x80082B30; locals statIdx,x,y,stats,str[40]).  N64 differences: screenPos is not halved,
 * tab/taba/single indices (51/52/44/37, 176/42/6), 90 not 92, ChangePaneSize arguments, flash colours passed by value.
 * `Color4 c1, c2;` in both arms of the flash-colour if/else are GC's tRGBA locals; the N64 call passes flashColors[0/1]
 * directly so they are dead, but block-scope locals are allocated after the function-level ones and account for exactly
 * the 4 words between screenPos and the totals locals (312 B frame).  drawView is NON-static and not on `keep` so that
 * its deleted-procedure stub does not become 'extra words' of func_803AD0D4 (the retail stub is func_803AD574).
 * NOT A MATCH: 523-word target, 524 emitted (+1: as1 hoists `li at,1` into the state dispatch's load stall), 35 words
 * in difflib-aligned runs, strict 497/523. See cloud/work/frontier/w35e/RESULTS.md section 3. */
void drawView(void);
void drawView(void)
{
    s32 i, x, y, cx;
    u8 *str;
    u8 str1[64];
    s16 screenPos[2];
    s32 stat_index, stat_x, stat_y;
    u8 *stats;
    u8 totals_text[40];
        func_800B669C(1,1);
        brake_light_update(0,D_803B5F28[17].position,&D_80150B70[0],0,screenPos);
        cx=screenPos[0];y=screenPos[1];
        func_800F6928(D_803B5F28[17].position[2]-10.0f);
        dispatch_handler(1);object_create(13);dispatch_handler(1);
        str=!D_803BA800 ? countdown_state.text[176] : (*D_803BA800)->name;music_fade(cx,y,140,str);
        func_800B669C(0,1);object_bytes_sum_global();object_create(11);
        for(i=0;i<2;i++) {
            if(D_803B5F28[i+11].angle>0.6981316804885864f && D_803B5F28[i+11].angle<2.4434609413146973f)continue;
            if(!D_803B5F28[i+11].alpha)continue;
            func_800ED66C((f32)(u32)D_803B5F28[i+11].alpha);
            if(D_803B5F28[i+11].angle<1.5707963705062866f) {
                Color4 c1,c2;
                func_800BEA3C(D_801146BC,D_801146C0);dispatch_handler(1);
            } else {
                Color4 c1,c2;
                func_800BEA3C(D_801146BC,D_801146C0);dispatch_handler(22);
            }
            func_800F6928(D_803B5F28[i+11].position[2]-10.0f);
            brake_light_update(0,D_803B5F28[i+11].position,&D_80150B70[0],0,screenPos);
            x=screenPos[0]-35.0f*D_803B64E8*300.0f/D_803B64FC;y=screenPos[1];
            brake_light_update(0,D_803B5F28[15].position,&D_80150B70[0],0,screenPos);
            cx=screenPos[0]-55.0f*D_803B64E8*300.0f/D_803B64FC;
            str=NAME(51,i);state_utility(x,y,str);
            x=cx;
            if(i==0) {
                if(D_803BA880==4) {
                    fcvt_wrapper(str1,D_803B8AD8,NAME(44,D_803BA860),countdown_state.text[42]);str=str1;
                } else if(D_803BA880==5)str=countdown_state.text[176];
                else if(D_803BA858<12 && (D_803BA858&1)) {
                    fcvt_wrapper(str1,D_803B8AE0,D_8011A8D8[D_803BA858>=13 ? D_803BA858-6 : D_803BA858/2],countdown_state.text[6]);str=str1;
                } else str=D_8011A8D8[D_803BA858>=13 ? D_803BA858-6 : D_803BA858/2];
                state_utility(x,y,str);
            } else if(i==1){str=NAME(52,D_803BA818[D_803BA880]);state_utility(x,y,str);}
        }
        func_800B669C(0,3);func_800ED66C(-1.0f);render_helper(0.0f);
        switch(D_803BA818[D_803BA880]) {
        case 0:func_803AD0D4();break;
        case 1:func_803ACF5C(0);break;
        case 2:func_803ACF5C(1);break;
        case 3:func_803ACF5C(2);break;
        case 4:func_803ACD5C();break;
        case 5:func_803ACA94();break;
        case 6: {
            if(D_803BA800)stats=PROFILE_DATA+12+D_803BA858*64;
            else stats=D_80151410+D_803BA858*64-1280;
            object_create(11);dispatch_handler(1);
            for(stat_index=0;stat_index<10;stat_index++) {
                if(stat_index==0) {stat_x=110;stat_y=90;}
                else if(stat_index==5) {stat_x=250;stat_y=90;}
                fcvt_wrapper(totals_text,D_803B8B60,NAME(37,stat_index));
                state_utility(sound_pitch_diff_calc(totals_text,stat_x-4),stat_y,totals_text);
                fcvt_wrapper(totals_text,D_803B8B64,U32_AT(stats,20+stat_index*4));
                state_utility(stat_x+4,stat_y,totals_text);stat_y+=object_bytes_sum_global();
            }
            crowd_cheer_play(D_803B6530,20,86,300,94+object_bytes_sum_global()*5);
            break;
        }
        case 7:func_803AC7F4();break;
        case 8:func_803AC54C();break;
        case 9:func_803ABEC8();break;
        case 10:func_803AB848();break;
        case 11:func_803AB5FC();break;
        case 12:func_803AAF0C();break;
        }
        func_800B669C(0,3);
}
s32 func_803AD57C(void *callback_context)
{
    render_helper(0.0f);
    switch(D_803BA804) {
    case 0:func_803AA3D8();break;
    case 1:drawView();break;
    }
    render_helper(-1.0f);
    return 1;
}
