/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image-A profile-selection renderer A7B58, complete native3196-byte extent.
 * Actual96FA0 controller supplies eligibility-helper call frequency; the two
 * one-formal helpers use the stable player-count bound, preserving their
 * complete92-byte native bodies and genuine skip=a1 private convention.
 * Indexed slots, guarded walks, split seen postincrement and direct selection
 * reads recover native control/data lifetimes. A real profile pointer is
 * reused across traversal and label selection. The u16 width-choice boundary
 * and one compiled-out label read are explicit source shaping: the latter
 * raises the traced name-pointer allocation priority above width. Neither
 * changes the consumed values or emits an extra native instruction.
 * The one temporary text writer is bounded by native format and all six
 * supported languages:10 bytes including NUL for player0..3,20 for the
 * formatter's independent signed32 domain.32-byte storage plus native home
 * order is a safe reconstruction; exact original capacity remains UNKNOWN.
 * No forced compiler, false formal, padding or unused scalar is introduced.
 * All2508 inherited bytes are exact. A9088/A9EEC/96FA0 remain NONMATCH context.
 */
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
struct Profile { Profile **next,**previous; FileInfo **file; u8 unknown0C[8]; u8 name[1]; };
typedef struct Player { u8 unknown00[4];u32 pressed;u8 unknown08[4];u32 repeat;u8 unknown10[56]; Profile **owner; } Player;
typedef struct Layout { s32 unknown0,y,width,height,x,unknown14; } Layout;
typedef struct FiveParts FiveParts;
extern Language countdown_state;
extern Slot D_803B470C[][22];
extern Camera D_80150B70[];
extern Layout D_803B5D50[][4];
extern f32 D_803B5D10,D_803B5D24;
extern s16 D_8014A108;
extern Player D_8014A118[];
extern Profile *D_80146150[];
extern s32 D_801146AC[];
extern Color4 D_801146BC,D_801146C0;
extern Profile **D_8012E6E0,**D_8012E6E4,**D_803BAC80[];
extern void *D_803BAC68[];
extern FiveParts *D_803BAC08[];
extern s32 D_803BACE0[],D_803BAD80[];
extern s8 D_803BADB8[],D_803BADBC[],D_803BADC0[],D_803BADC8[],D_803BADCC[],D_803BADD0[],D_803BADD4[];
extern s8 D_803BAD00[][14];
extern u8 D_803BAC98[][13],D_803BADD8[][13];
extern s16 D_803BACD0[];
extern u8 *D_80117420,*D_8011EAD0[];
extern u8 D_803B5EDC[],D_803B5EE0[],D_803B5EE4[];
extern u8 D_803B899C[],D_803B89A4[],D_803B89AC[],D_803B89B4[],D_803B89C0[],D_803B89C8[];
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
/* The native integer absolute-value expression repeats its operand. */
#define ABS(x) ((x)<0 ? -(x) : (x))
#define PROFILE_DISTANCE(x) ((x)>=0 ? (x) : -(x))

/* The native owner helper checks for a null handle before reading count.
 * Directly using the stable player-count bound removes its eager snapshot
 * and lets the actual index and bound receive their native IPA registers.
 * owner is passed in a0 and skip in a2 by both real renderer children.
 * The two sibling eligibility helpers are preserved exact members. */
s32 func_80395E24(s32 skip,Profile **owner)
{
    s8 i;
    s32 found=0;
    if(owner)for(i=0;i<D_8014A108;i++)if(i!=skip && owner==D_8014A118[i].owner)found=1;
    return found;
}
s32 func_80396EE8(s32 skip)
{
    s8 i;
    s32 found=0;
    for(i=0;i<D_8014A108;i++)if(i!=skip && D_803BACE0[i]==4)found=1;
    return found;
}
s32 func_80396F44(s32 skip)
{
    s8 i;
    s32 found=0;
    for(i=0;i<D_8014A108;i++)if(i!=skip && D_803BACE0[i]==3)found=1;
    return found;
}
s32 func_80395E24(s32,Profile **);
s32 func_80396F44(s32);
s32 func_80396EE8(s32);
void func_803A7B58(s32 player)
{
    s32 draw_x,y,x;
    f32 scale=D_803B5D10;
    Profile *profile;
    u8 *label,text[32];
    s16 limit,point[2];
    s32 i,seen;
    f32 position[3];
    Profile **node;
    if(D_8014A108>=2)scale*=0.5f;
    crowd_cheer_play(D_803BAC08[player],-20,0,-10,10);
    if(D_8014A108==1)object_create(13);
    else if(D_8014A108==2)object_create(11);
    else object_create(12);
    fcvt_wrapper(text,D_803B899C,countdown_state.text[29],player+1);
    dispatch_handler(D_801146AC[player]);
    func_800B669C(0,1);
    brake_light_update(player,D_803B470C[player][16].position,&D_80150B70[player],0,point);
    x=point[0];y=point[1];
    func_800F6928(D_803B470C[player][16].position[2]-10.0f);
    dispatch_handler(1);
    state_utility(sound_pitch_diff_halved(text,x),y,text);
    object_create(12);
    if(D_803BAD80[player]==0) {
        position[2]=D_803B470C[player][0].position[2];
        position[0]=0.0f;
        if(D_8014A108==1)position[1]=130.0f;else position[1]=115.0f;
        brake_light_update(player,position,&D_80150B70[player],0,point);
        x=point[0];y=point[1];
        func_800F6928(position[2]-10.0f);
        dispatch_handler(1);
        label=countdown_state.text[95];
        dispatch_handler(22);
        state_utility(sound_pitch_diff_halved(label,x),y,label);
    }
    object_bytes_sum_global();
    if(D_8014A108==1)object_create(10);else object_create(12);
    for(i=0;i<14;i++) {
        if(D_803B470C[player][i].angle>0.6981316804885864f && D_803B470C[player][i].angle<2.4434609413146973f)continue;
        if(!D_803B470C[player][i].alpha)continue;
        func_800ED66C((f32)(u32)D_803B470C[player][i].alpha);
        if(D_803B470C[player][i].angle<1.5707963705062866f) {
            func_800BEA3C(D_801146BC,D_801146C0);
            dispatch_handler(1);
        } else {
            func_800BEA3C(D_801146BC,D_801146C0);
            dispatch_handler(22);
        }
        func_800F6928(D_803B470C[player][i].position[2]-10.0f);
        brake_light_update(player,D_803B470C[player][i].position,&D_80150B70[player],0,point);
        if(D_803B470C[player][i].kind==9)draw_x=point[0];
        else if(D_803B470C[player][i].kind==7)draw_x=point[0]-65.0f*scale*300.0f/D_803B5D24;
        else draw_x=point[0]-45.0f*scale*300.0f/D_803B5D24;
        y=point[1];
        if(i==1) {
            brake_light_update(player,D_803B470C[player][15].position,&D_80150B70[player],0,point);
            x=point[0]-25.0f*scale*300.0f/D_803B5D24;
        }
        if(i<4)label=countdown_state.indexed[countdown_state.indices[21]+i];
        else if(i<11)label=countdown_state.indexed[countdown_state.indices[21]+3];
        else label=countdown_state.indexed[countdown_state.indices[21]+i-7];
        if(i==0) {
            if(i==D_803BAD80[player])dispatch_handler(D_803BADB8[player]==0 ? 22 : 1);
            else dispatch_handler(1);
            state_utility(draw_x,y,label);
        } else if(i==1) {
            dispatch_handler(D_803BAD80[player]==1 ? 22 : 1);
            state_utility(draw_x,y,label);
            label=D_8011EAD0[D_803BADBC[player]];
            state_utility(x,y,label);
        } else if(i==2) {
            switch(D_803BADC0[player]) {
            case -4:label=countdown_state.text[165];break;
            case -3:label=countdown_state.text[169];break;
            case 0:label=countdown_state.text[148];break;
            case -2:label=countdown_state.text[166];break;
            case -5:label=countdown_state.text[161];break;
            case -1:label=countdown_state.text[168];break;
            }
            if(D_803BAC68[D_803BADBC[player]])label=countdown_state.text[161];
            dispatch_handler(0);
            state_utility(sound_pitch_diff_halved(label,draw_x),y,label);
        } else if(i>=3 && i<11) {
            node=D_803BAC80[player];seen=0;
            if(node) {
                do {
                    profile=*node;
                    if(profile->file && (*profile->file)->controller==D_803BADBC[player]) {
                        if(PROFILE_DISTANCE(i-D_803BAD80[player])==seen)break;
                        seen++;
                    }
                    if(i<D_803BAD80[player])node=profile->previous;else node=profile->next;
                } while(node);
            }
            if(!node) {
                if(D_803BAD80[player]<i || D_803BADC0[player]<8)node=D_8012E6E0;
                else node=D_8012E6E4;
                while(node) {
                    profile=*node;
                    if(profile->file && (*profile->file)->controller==D_803BADBC[player]) {
                        if(PROFILE_DISTANCE(i-((D_803BAD80[player]<i || D_803BADC0[player]<8) ? 3 : 10))==seen) {seen++;break;}
                        seen++;
                    }
                    if(D_803BAD80[player]<i || D_803BADC0[player]<8)node=profile->next;
                    else node=profile->previous;
                }
            }
            if(node) {
                profile=*node;
                label=profile->name;
                if(D_8014A108==1)limit=98;
                else limit=(u16)(D_8014A108==2 ? 70 : 55);
                if((u32)limit<(u32)object_manager_update(label,-1))mode_byte_set(0);
                if(label) {}
                if(i==D_803BAD80[player] && node==D_803BAC80[player])
                    dispatch_handler(D_803BADB8[player]==0 ? 22 : 1);
                else {
                    dispatch_handler(1);
                    if(func_80395E24(player,node))dispatch_handler(0);
                }
                music_fade(draw_x,y,limit,label);
                mode_byte_set(-1);
            }
        } else {
            dispatch_handler(i==D_803BAD80[player] ? 22 : 1);
            if((i==11 && func_80396F44(player)) || (i==12 && func_80396EE8(player)))dispatch_handler(0);
            state_utility(draw_x,y,label);
        }
        object_bytes_sum_global();
    }
    if(D_803BAD80[player]==0 || (D_803BAD80[player]>=3 && D_803BAD80[player]<11)) {
        brake_light_update(player,D_803B470C[player][14].position,&D_80150B70[player],0,point);
        x=point[0]-45.0f*scale*300.0f/D_803B5D24;y=point[1];
        dispatch_handler(D_803BADB8[player]==1 ? 22 : 1);
        if((D_803B470C[player][14].angle<=0.6981316804885864f || D_803B470C[player][14].angle>=2.4434609413146973f) && D_803B470C[player][14].alpha) {
            func_800ED66C((f32)(u32)D_803B470C[player][14].alpha);
            if(D_803B470C[player][14].angle<1.5707963705062866f) {
                func_800BEA3C(D_801146BC,D_801146C0);
                dispatch_handler(1);
            } else {
                func_800BEA3C(D_801146BC,D_801146C0);
                dispatch_handler(22);
            }
            state_utility(x,y,countdown_state.text[51]);
        }
    }
    func_800B669C(0,3);
    func_800ED66C(-1.0f);
    render_helper(0.0f);
}
void func_803A87D4(s32 player)
{
    s16 top,width;
    s32 x,y;
    s16 point[2];
    s32 i,count,other;
    u8 *text;
    if(D_8014A108>=2)object_create(11);else object_create(13);
    if(D_8014A118[player].owner)text=(*D_8014A118[player].owner)->name;
    else text=D_80146150[player]->name;
    dispatch_handler(D_801146AC[player]);
    func_800B669C(1,1);
    brake_light_update(player,D_803B470C[player][16].position,&D_80150B70[player],0,point);
    x=point[0];y=point[1];
    func_800F6928(D_803B470C[player][16].position[2]-10.0f);
    dispatch_handler(1);
    music_fade(x,y,69,text);
    y+=object_bytes_sum_global()*2;
    top=y-4;
    func_800B669C(0,3);
    func_800F6928(0.0f);
    text=countdown_state.text[151];
    dispatch_handler(1);
    state_utility(sound_pitch_diff_halved(text,x),y,text);
    width=object_manager_update(text,-1);
    y+=object_bytes_sum_global();
    count=0;
    for(i=0;i<D_8014A108;i++) {
        if(i!=player && D_803BACE0[i]!=1) {other=i;count++;}
    }
    if(count==1) {
        dispatch_handler(D_801146AC[other]);
        if(D_8014A118[other].owner)text=(*D_8014A118[other].owner)->name;
        else text=D_80146150[other]->name;
    } else text=countdown_state.text[31];
    state_utility(sound_pitch_diff_halved(text,x),y,text);
    if((u32)width<(u32)object_manager_update(text,-1))width=object_manager_update(text,-1);
    x=D_803B5D50[D_8014A108-1][player].x;
    crowd_cheer_play(D_803BAC08[player],x-width/2-4,top,(x+4)+width/2,top+object_bytes_sum_global()*2+8);
}
/* drawControls@playerscr.c (MAT3 GC 0x8007d3f4): inlined into animText; deleted stub func_803A8B6C */
void func_803A8B6C(s32 p) { crowd_cheer_play(D_803BAC08[p], -20, 0, -10, 10); }
void func_803A8B74(s32 player)
{
    s32 i;
    u8 text[2];
    s32 x,y,columns,width;
    s16 point[2],top;
    if(D_8014A108>=2)object_create(11);else object_create(13);
    func_800B669C(1,1);
    brake_light_update(player,D_803B470C[player][16].position,&D_80150B70[player],0,point);
    x=point[0];y=point[1];
    func_800F6928(D_803B470C[player][16].position[2]-10.0f);
    dispatch_handler(22);
    if(D_8014A108==1U)width=150;else width=69;
    music_fade(x,y,width,D_803BAC98[player]);
    func_800B669C(0,1);
    object_create(11);
    func_800F6928(0.0f);
    func_800B669C(0,3);
    text[1]=0;
    x=D_803B5D50[D_8014A108-1][player].x;
    y=D_803B5D50[D_8014A108-1][player].y+32;
    if(D_8014A108>=2)y-=26;
    if(D_8014A108==1U) {
        top=y-4;
        dispatch_handler(1);
        state_utility(sound_pitch_diff_halved(countdown_state.text[150],x),y,countdown_state.text[150]);
    }
    object_create(12);
    x=D_803B5D50[D_8014A108-1][player].x-48;
    y=D_803B5D50[D_8014A108-1][player].y+46;
    if(D_8014A108>=2) {y-=26;top=y-4;}
    columns=9;
    for(i=0;i<54;i++) {
        text[0]=D_80117420[i];
        dispatch_handler(i==D_803BACD0[player] ? 22 : 1);
        state_utility(sound_pitch_diff_halved(text,x),y,text);
        if((i+1)%columns==0) {x=D_803B5D50[D_8014A108-1][player].x-48;y+=10;}
        else x+=12;
    }
    x=D_803B5D50[D_8014A108-1][player].x-36;
    dispatch_handler(D_803BACD0[player]==-3 ? 22 : 16);
    state_utility(sound_pitch_diff_halved(D_803B5EE4,x),y,D_803B5EE4);
    x+=36;
    dispatch_handler(D_803BACD0[player]==-2 ? 22 : 16);
    state_utility(sound_pitch_diff_halved(D_803B5EDC,x),y,D_803B5EDC);
    x+=36;
    dispatch_handler(D_803BACD0[player]==-1 ? 22 : 16);
    state_utility(sound_pitch_diff_halved(D_803B5EE0,x),y,D_803B5EE0);
    y+=10;
    crowd_cheer_play(D_803BAC08[player],D_803B5D50[D_8014A108-1][player].x-56,top,D_803B5D50[D_8014A108-1][player].x+56,y+4);
}
s32 func_80395E24(s32,Profile **);
void func_803A9088(s32 player)
{
    f32 scale=D_803B5D10;
    s32 y,x;
    u8 *label,text[128];
    s32 draw_x,i,seen;
    s16 point[2],top,lines,limit;
    s8 selected;
    f32 delta;
    Profile **node;
    if(D_8014A108>=2)scale*=0.5f;
    if(D_8014A108>=2)object_create(12);else object_create(13);
    label=countdown_state.text[140];
    dispatch_handler(D_801146AC[player]);
    func_800B669C(0,1);
    brake_light_update(player,D_803B470C[player][16].position,&D_80150B70[player],0,point);
    x=point[0];y=point[1];
    func_800F6928(D_803B470C[player][16].position[2]-10.0f);
    dispatch_handler(1);
    state_utility(sound_pitch_diff_halved(label,x),y,label);
    y+=object_bytes_sum_global();
    if(D_8014A108>=2)object_create(12);else object_create(10);
    if(D_803BADCC[player]==1) {
        func_800B669C(0,3);
        func_800F6928(0.0f);
        y+=object_bytes_sum_global();
        top=y-4;
        fcvt_wrapper(text,D_803B89A4,countdown_state.text[141],D_803BADD8[player],countdown_state.text[142]);
        dispatch_handler(1);
        func_800B669C(1,3);
        camera_auto_follow(x,y,D_803B5D50[D_8014A108-1][player].width,
                           D_803B5D50[D_8014A108-1][player].y+D_803B5D50[D_8014A108-1][player].height-y,-1,0,text);
        func_800B669C(0,3);
        lines=camera_zoom_fov(D_803B5D50[D_8014A108-1][player].width,text);
        y+=object_bytes_sum_global()*(lines+1);
        label=countdown_state.indexed[countdown_state.indices[24]];
        dispatch_handler(D_803BADD0[player]==1 ? 22 : 1);
        state_utility(sound_pitch_diff_calc(label,x-5),y,label);
        label=countdown_state.indexed[countdown_state.indices[24]+1];
        dispatch_handler(D_803BADD0[player]==0 ? 22 : 1);
        state_utility(x+5,y,label);
        y+=object_bytes_sum_global();
        crowd_cheer_play(D_803BAC08[player],x-D_803B5D50[D_8014A108-1][player].width/2-4,top,
                         x+D_803B5D50[D_8014A108-1][player].width/2+4,y+4);
    } else if(D_803BADD4[player]==1) {
        func_800B669C(0,3);
        func_800F6928(0.0f);
        y+=object_bytes_sum_global();
        top=y-4;
        fcvt_wrapper(text,countdown_state.text[145],D_803BADD8[player]);
        dispatch_handler(1);
        func_800B669C(1,3);
        camera_auto_follow(x,y,D_803B5D50[D_8014A108-1][player].width,
                           D_803B5D50[D_8014A108-1][player].y+D_803B5D50[D_8014A108-1][player].height-y,-1,0,text);
        func_800B669C(0,3);
        lines=camera_zoom_fov(D_803B5D50[D_8014A108-1][player].width,text);
        y+=object_bytes_sum_global()*lines;
        crowd_cheer_play(D_803BAC08[player],x-D_803B5D50[D_8014A108-1][player].width/2-4,top,
                         x+D_803B5D50[D_8014A108-1][player].width/2+4,y+4);
    } else {
        crowd_cheer_play(D_803BAC08[player],-20,0,-10,10);
        for(i=0;i<14;i++) {
            if(D_803B470C[player][i].angle>0.6981316804885864f && D_803B470C[player][i].angle<2.4434609413146973f)continue;
            if(!D_803B470C[player][i].alpha)continue;
            func_800ED66C((f32)(u32)D_803B470C[player][i].alpha);
            if(D_803B470C[player][i].angle<1.5707963705062866f) {
                func_800BEA3C(D_801146BC,D_801146C0);
                dispatch_handler(1);
            } else {
                func_800BEA3C(D_801146BC,D_801146C0);
                dispatch_handler(22);
            }
            func_800F6928(D_803B470C[player][i].position[2]-10.0f);
            brake_light_update(player,D_803B470C[player][i].position,&D_80150B70[player],0,point);
            if(D_803B470C[player][i].kind==9)draw_x=point[0]-105.0f*scale*300.0f/D_803B5D24;
            else if(D_803B470C[player][i].kind==7)draw_x=point[0]-65.0f*scale*300.0f/D_803B5D24;
            else draw_x=point[0]-45.0f*scale*300.0f/D_803B5D24;
            y=point[1];
            if(i==0 || (i>=3 && i<11)) {
                brake_light_update(player,D_803B470C[player][14].position,&D_80150B70[player],0,point);
                x=point[0]-45.0f*scale*300.0f/D_803B5D24;
            } else if(i==1) {
                brake_light_update(player,D_803B470C[player][15].position,&D_80150B70[player],0,point);
                x=point[0]-25.0f*scale*300.0f/D_803B5D24;
            }
            if(i==1) {
                dispatch_handler(D_803BADC8[player]==1 ? 22 : 1);
                label=countdown_state.indexed[countdown_state.indices[21]+i];
                state_utility(draw_x,y,label);
                label=D_8011EAD0[D_803BADBC[player]];
                state_utility(x,y,label);
            } else if(i==2) {
                delta=65.f*scale*300.0f/D_803B5D24;
                switch(D_803BADC0[player]) {
                case -4:label=countdown_state.text[165];break;
                case -3:label=countdown_state.text[169];break;
                case 0:label=countdown_state.text[148];break;
                case -2:label=countdown_state.text[166];break;
                case -5:label=countdown_state.text[161];break;
                case -1:label=countdown_state.text[168];break;
                }
                draw_x+=delta;
                dispatch_handler(0);
                state_utility(sound_pitch_diff_halved(label,draw_x),y,label);
            } else if(i>=3 && i<11) {
                node=D_803BAC80[player];seen=0;
                if(node) {
                    selected=D_803BADC8[player];
                    do {
                        if((*node)->file && (*(*node)->file)->controller==D_803BADBC[player]) {
                            if(PROFILE_DISTANCE(i-selected)==seen)break;
                            seen++;
                        }
                        if(i<selected)node=(*node)->previous;else node=(*node)->next;
                    } while(node);
                }
                if(!node) {
                    selected=D_803BADC8[player];
                    if(selected<i || D_803BADC0[player]<8)node=D_8012E6E0;
                    else node=D_8012E6E4;
                    while(node) {
                        if((*node)->file && (*(*node)->file)->controller==D_803BADBC[player]) {
                            if(PROFILE_DISTANCE(i-((selected<i || D_803BADC0[player]<8) ? 3 : 10))==seen) {seen++;break;}
                            seen++;
                        }
                        if(selected<i || D_803BADC0[player]<8)node=(*node)->next;
                        else node=(*node)->previous;
                    }
                }
                if(node) {
                    label=(*node)->name;
                    if(D_8014A108==1)limit=98;
                    else limit=(u16)(D_8014A108==2 ? 70 : 55);
                    if((u32)limit<(u32)object_manager_update(label,-1))mode_byte_set(0);
                    if(i==D_803BADC8[player] && node==D_803BAC80[player])dispatch_handler(22);
                    else {
                        dispatch_handler(1);
                        if(func_80395E24(player,node))dispatch_handler(0);
                        if(!D_803BAD00[player][i])dispatch_handler(4);
                    }
                    music_fade(draw_x,y,limit,label);
                    mode_byte_set(-1);
                }
            }
            object_bytes_sum_global();
        }
    }
    func_800B669C(0,3);
    func_800ED66C(-1.0f);
    render_helper(0.0f);
}
/* drawPakFull@playerscr.c (MAT3 GC 0x8007eab4) */
void func_803A9EDC(s32 p)
{
    s32 y = D_803B5D50[D_8014A108 - 1][p].y;
    s32 cx = D_803B5D50[D_8014A108 - 1][p].x;
    u8 *str;
    u8 str1[44];
    if (D_8014A108 > 2) object_create(11);
    else if (D_8014A108 == 2) object_create(10);
    else object_create(13);
    fcvt_wrapper(str1, "%s %d", countdown_state.text[29], p + 1);
    dispatch_handler(D_801146AC[p]);
    state_utility(sound_pitch_diff_halved(str1, cx), y, str1);
    y += object_bytes_sum_global() * 2;
    if (D_8014A108 > 2) object_create(12);
    else if (D_8014A108 == 2) object_create(11);
    else object_create(10);
    str = (u8 *)"PAK FULL";
    dispatch_handler(17);
    state_utility(sound_pitch_diff_halved(str, cx), y, str);
}
/* drawPakDamaged@playerscr.c (MAT3 GC 0x8007ebf8) */
void func_803A9EE4(s32 p)
{
    s32 y = D_803B5D50[D_8014A108 - 1][p].y;
    s32 cx = D_803B5D50[D_8014A108 - 1][p].x;
    u8 *str;
    u8 str1[44];
    if (D_8014A108 > 2) object_create(11);
    else if (D_8014A108 == 2) object_create(10);
    else object_create(13);
    fcvt_wrapper(str1, "%s %d", countdown_state.text[29], p + 1);
    dispatch_handler(D_801146AC[p]);
    state_utility(sound_pitch_diff_halved(str1, cx), y, str1);
    y += object_bytes_sum_global() * 2;
    if (D_8014A108 > 2) object_create(12);
    else if (D_8014A108 == 2) object_create(11);
    else object_create(10);
    str = (u8 *)"PAK DAMAGED";
    dispatch_handler(17);
    state_utility(sound_pitch_diff_halved(str, cx), y, str);
}
void func_803A7B58(s32);
void func_803A87D4(s32);
void func_803A8B74(s32);
void func_803A9088(s32);
void func_803A9EDC(s32 p);
void func_803A9EE4(s32 p);
void func_803A8B6C(s32 p);
/* animText@playerscr.c (MAT3 GC 0x8007ed3c): the blit callback. N64: no updatePlayer call, no FormatMC case,
 * drawControls written in place (case 2); drawPakFull / drawPakDamaged are the inlined statics whose deleted
 * stubs are func_803A9EDC / func_803A9EE4 (defined after the caller, non-static, not on keep). */
s32 func_803A9EEC(void *callback_context)
{
    s32 playerIdx;
    render_helper(0.0f);
    for (playerIdx = 0; playerIdx < D_8014A108; playerIdx++) {
        switch (D_803BACE0[playerIdx]) {
        case 0: func_803A7B58(playerIdx); break;
        case 1: func_803A87D4(playerIdx); break;
        case 2: func_803A8B6C(playerIdx); break;
        case 3: func_803A8B74(playerIdx); break;
        case 4: func_803A9088(playerIdx); break;
        case 5: func_803A9EDC(playerIdx); break;
        case 6: func_803A9EE4(playerIdx);
        default: break;
        }
        
    }
    render_helper(-1.0f);
    return 1;
}

/* Genuine image-A profile-selection controller. Native entry consumes player in
 * s0 under its actual parent. This kept reconstruction is context, not a match; its genuine parent is absent. */
extern void resource_type_select(u32);
extern void audio_doppler(u32),audio_distance_atten(u32);
extern void func_80394564(s32),func_80394B00(s32,s32);
extern s32 func_800CCEFC(Profile **);
extern void func_800A3424(FileInfo **,s8);
extern s8 D_8015418C[];
extern s32 D_803BA888[],D_803B5ED4;

void func_80396FA0(s32 player)
{
    while (!D_803BAD00[player][D_803BAD80[player]] ||
           func_80395E24(player,D_803BAC80[player])==1 ||
           (D_803BAD80[player]==11 && func_80396F44(player)==1) ||
           (D_803BAD80[player]==12 && func_80396EE8(player)==1)) {
        D_803BADB8[player]=0;
        D_803BAD80[player]--;
        if(D_803BAD80[player]<0)D_803BAD80[player]=13;
        if(D_803BAD80[player]>=3 && D_803BAD80[player]<11 && D_803BAD00[player][D_803BAD80[player]]) {
            if(D_803BAC80[player]) {
                do {
                    D_803BAC80[player]=(*D_803BAC80[player])->previous;
                } while(D_803BAC80[player] && (*(*D_803BAC80[player])->file)->controller!=D_803BADBC[player]);
            }
            if(!D_803BAC80[player]) {
                D_803BAC80[player]=D_8012E6E4;
                while(D_803BAC80[player] && (*(*D_803BAC80[player])->file)->controller!=D_803BADBC[player])
                    D_803BAC80[player]=(*D_803BAC80[player])->previous;
            }
        } else D_803BAC80[player]=0;
    }
    if(D_8014A118[player].pressed&1) {
        resource_type_select(D_8014A118[player].pressed);
        if(D_803BAD80[player]>=3 && D_803BAD80[player]<11) {
            D_8014A118[player].owner=D_803BAC80[player];
            func_80394564(-1);
        } else {
            D_8014A118[player].owner=&D_80146150[player];
            func_80394564(-1);
        }
        func_80394B00(player,1);
    } else if(D_8014A118[player].pressed&2) {
        resource_type_select(D_8014A118[player].pressed);
        if(D_803BAD80[player]>=3 && D_803BAD80[player]<11) {
            D_8014A118[player].owner=D_803BAC80[player];
            func_80394564(-1);
            if(D_803BADB8[player]==1) {
                if(func_800CCEFC(D_8014A118[player].owner)) {
                    D_8015418C[player]=1;
                    func_800A3424((*D_8014A118[player].owner)->file,1);
                    func_80394B00(player,2);
                }
            } else func_80394B00(player,1);
        } else switch(D_803BAD80[player]) {
        case 0:
            D_8014A118[player].owner=&D_80146150[player];
            func_80394564(-1);
            if(D_803BADB8[player]==1) {
                D_8015418C[player]=1;
                func_800A3424((*D_8014A118[player].owner)->file,1);
                func_80394B00(player,2);
            } else func_80394B00(player,1);
            break;
        case 11:func_80394B00(player,3);break;
        case 12:D_803BADC8[player]=0;func_80394B00(player,4);break;
        case 13:func_80394B00(player,6);break;
        }
    } else if(D_8014A118[player].pressed&4) {
        resource_type_select(D_8014A118[player].pressed);
        D_803B5ED4=2;
    }
    if(D_8014A118[player].repeat&0x2000) {
        if(D_803BAD80[player]==0 || (D_803BAD80[player]>=3 && D_803BAD80[player]<11)) {
            D_803BADB8[player]=!D_803BADB8[player];
            audio_doppler(D_8014A118[player].pressed);
        } else if(D_803BAD80[player]==1) {
            D_803BADBC[player]++;
            if(D_803BADBC[player]>=4)D_803BADBC[player]=0;
            func_80394564(player);
            D_803BA888[player]=1;
            audio_doppler(D_8014A118[player].pressed);
        }
    } else if(D_8014A118[player].repeat&0x1000) {
        if(D_803BAD80[player]==0 || (D_803BAD80[player]>=3 && D_803BAD80[player]<11)) {
            D_803BADB8[player]=!D_803BADB8[player];
            audio_doppler(D_8014A118[player].pressed);
        } else if(D_803BAD80[player]==1) {
            D_803BADBC[player]--;
            if(D_803BADBC[player]<0)D_803BADBC[player]=3;
            func_80394564(player);
            D_803BA888[player]=1;
            audio_doppler(D_8014A118[player].pressed);
        }
    } else if(D_8014A118[player].repeat&0x400) {
        audio_distance_atten(D_8014A118[player].pressed);
        D_803BADB8[player]=0;
        do {
            if(D_803BAD80[player]==3 && D_803BAD00[player][D_803BAD80[player]]) {
                if(D_803BAC80[player]) {
                    do {
                        D_803BAC80[player]=(*D_803BAC80[player])->previous;
                    } while(D_803BAC80[player] && (*(*D_803BAC80[player])->file)->controller!=D_803BADBC[player]);
                }
                if(D_803BAC80[player])break;
            }
            D_803BAD80[player]--;
            if(D_803BAD80[player]<0)D_803BAD80[player]=13;
            if(D_803BAD80[player]>=3 && D_803BAD80[player]<11 && D_803BAD00[player][D_803BAD80[player]]) {
                if(D_803BAC80[player]) {
                    do {
                        D_803BAC80[player]=(*D_803BAC80[player])->previous;
                    } while(D_803BAC80[player] && (*(*D_803BAC80[player])->file)->controller!=D_803BADBC[player]);
                }
                if(!D_803BAC80[player]) {
                    D_803BAC80[player]=D_8012E6E4;
                    while(D_803BAC80[player] && (*(*D_803BAC80[player])->file)->controller!=D_803BADBC[player])
                        D_803BAC80[player]=(*D_803BAC80[player])->previous;
                }
            } else D_803BAC80[player]=0;
        } while(!D_803BAD00[player][D_803BAD80[player]] || func_80395E24(player,D_803BAC80[player])==1 ||
                (D_803BAD80[player]==11 && func_80396F44(player)==1) ||
                (D_803BAD80[player]==12 && func_80396EE8(player)==1));
    } else if(D_8014A118[player].repeat&0x800) {
        audio_distance_atten(D_8014A118[player].pressed);
        D_803BADB8[player]=0;
        do {
            if(D_803BAD80[player]==10 && D_803BAD00[player][D_803BAD80[player]]) {
                if(D_803BAC80[player]) {
                    do {
                        D_803BAC80[player]=(*D_803BAC80[player])->next;
                    } while(D_803BAC80[player] && (*(*D_803BAC80[player])->file)->controller!=D_803BADBC[player]);
                }
                if(D_803BAC80[player])break;
            }
            D_803BAD80[player]++;
            if(D_803BAD80[player]>=14)D_803BAD80[player]=0;
            if(D_803BAD80[player]>=3 && D_803BAD80[player]<11 && D_803BAD00[player][D_803BAD80[player]]) {
                if(D_803BAC80[player]) {
                    do {
                        D_803BAC80[player]=(*D_803BAC80[player])->next;
                    } while(D_803BAC80[player] && (*(*D_803BAC80[player])->file)->controller!=D_803BADBC[player]);
                }
                if(!D_803BAC80[player]) {
                    D_803BAC80[player]=D_8012E6E0;
                    while(D_803BAC80[player] && (*(*D_803BAC80[player])->file)->controller!=D_803BADBC[player])
                        D_803BAC80[player]=(*D_803BAC80[player])->next;
                }
            } else D_803BAC80[player]=0;
        } while(!D_803BAD00[player][D_803BAD80[player]] || func_80395E24(player,D_803BAC80[player])==1 ||
                (D_803BAD80[player]==11 && func_80396F44(player)==1) ||
                (D_803BAD80[player]==12 && func_80396EE8(player)==1));
    }
}
