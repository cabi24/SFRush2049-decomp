/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
typedef struct Player { u8 unknown00[72]; Profile **owner; } Player;
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
extern void *object_create(s32);
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

/* Existing semantic helpers, required genuine private-call context only. */
s32 func_80395E24(Profile **owner,s32 skip)
{
    s8 i;
    s32 count=D_8014A108,found=0;
    if(owner)for(i=0;i<count;i++)if(i!=skip && owner==D_8014A118[i].owner)found=1;
    return found;
}
s32 func_80396EE8(s32 skip)
{
    s8 i;
    s32 count=D_8014A108,found=0;
    for(i=0;i<count;i++)if(i!=skip && D_803BACE0[i]==4)found=1;
    return found;
}
s32 func_80396F44(s32 skip)
{
    s8 i;
    s32 count=D_8014A108,found=0;
    for(i=0;i<count;i++)if(i!=skip && D_803BACE0[i]==3)found=1;
    return found;
}
s32 func_80395E24(Profile **,s32);
s32 func_80396F44(s32);
s32 func_80396EE8(s32);
void func_803A7B58(s32 player)
{
    f32 scale=D_803B5D10,position[3];
    s16 point[2],limit;
    u8 text[40],*label;
    s32 x,y,draw_x,i,seen,selected;
    Slot *slot;
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
    for(i=0,slot=D_803B470C[player];i<14;i++,slot++) {
        if(slot->angle>0.6981316804885864f && slot->angle<2.4434609413146973f)continue;
        if(!slot->alpha)continue;
        func_800ED66C((f32)(u32)slot->alpha);
        if(slot->angle<1.5707963705062866f) {
            func_800BEA3C(D_801146BC,D_801146C0);
            dispatch_handler(1);
        } else {
            func_800BEA3C(D_801146BC,D_801146C0);
            dispatch_handler(22);
        }
        func_800F6928(slot->position[2]-10.0f);
        brake_light_update(player,slot->position,&D_80150B70[player],0,point);
        if(slot->kind==9)draw_x=point[0];
        else if(slot->kind==7)draw_x=point[0]-65.0f*scale*300.0f/D_803B5D24;
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
            if(i==D_803BAD80[player])dispatch_handler(D_803BADB8[player] ? 1 : 22);
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
                selected=D_803BAD80[player];
                while(node) {
                    if((*node)->file && (*(*node)->file)->controller==D_803BADBC[player]) {
                        if(ABS(i-selected)==seen)break;
                        seen++;
                    }
                    if(i<selected)node=(*node)->previous;else node=(*node)->next;
                }
            }
            if(!node) {
                selected=D_803BAD80[player];
                if(selected<i || D_803BADC0[player]<8)node=D_8012E6E0;
                else node=D_8012E6E4;
                while(node) {
                    if((*node)->file && (*(*node)->file)->controller==D_803BADBC[player]) {
                        if(ABS(i-((selected<i || D_803BADC0[player]<8) ? 3 : 10))==seen++)break;
                    }
                    if(selected<i || D_803BADC0[player]<8)node=(*node)->next;
                    else node=(*node)->previous;
                }
            }
            if(node) {
                label=(*node)->name;
                if(D_8014A108==1)limit=98;
                else limit=D_8014A108==2 ? 70 : 55;
                if((u32)limit<(u32)object_manager_update(label,-1))mode_byte_set(0);
                if(i==D_803BAD80[player] && node==D_803BAC80[player])
                    dispatch_handler(D_803BADB8[player] ? 1 : 22);
                else {
                    dispatch_handler(1);
                    if(func_80395E24(node,player))dispatch_handler(0);
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
        slot=&D_803B470C[player][14];
        if((slot->angle<=0.6981316804885864f || slot->angle>=2.4434609413146973f) && slot->alpha) {
            func_800ED66C((f32)(u32)slot->alpha);
            if(slot->angle<1.5707963705062866f) {
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
    s16 point[2],top,width;
    s32 x,y,i,count,other;
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
    crowd_cheer_play(D_803BAC08[player],x-width/2-4,top,x+width/2+4,top+object_bytes_sum_global()*2+8);
}
void func_803A8B74(s32 player)
{
    u8 text[2];
    s16 point[2],top;
    s32 x,y,i,columns,characters;
    if(D_8014A108>=2)object_create(11);else object_create(13);
    func_800B669C(1,1);
    brake_light_update(player,D_803B470C[player][16].position,&D_80150B70[player],0,point);
    x=point[0];y=point[1];
    func_800F6928(D_803B470C[player][16].position[2]-10.0f);
    dispatch_handler(22);
    music_fade(x,y,D_8014A108==1 ? 150 : 69,D_803BAC98[player]);
    func_800B669C(0,1);
    object_create(11);
    func_800F6928(0.0f);
    func_800B669C(0,3);
    y=D_803B5D50[D_8014A108-1][player].y+32;
    x=D_803B5D50[D_8014A108-1][player].x;
    text[1]=0;
    if(D_8014A108>=2)y-=26;
    if(D_8014A108==1) {
        top=y-4;
        dispatch_handler(1);
        state_utility(sound_pitch_diff_halved(countdown_state.text[150],x),y,countdown_state.text[150]);
    }
    object_create(12);
    x=D_803B5D50[D_8014A108-1][player].x-48;
    y=D_803B5D50[D_8014A108-1][player].y+46;
    if(D_8014A108>=2) {y-=26;top=y-4;}
    columns=9;characters=54;
    for(i=0;i<characters;i++) {
        text[0]=D_80117420[i];
        dispatch_handler(i==D_803BACD0[player] ? 22 : 1);
        state_utility(sound_pitch_diff_halved(text,x),y,text);
        x+=12;
        if((i+1)%columns==0) {x=D_803B5D50[D_8014A108-1][player].x-48;y+=10;}
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
s32 func_80395E24(Profile **,s32);
void func_803A9088(s32 player)
{
    u8 text[128],*label;
    s16 point[2],top,lines,limit;
    s32 x,y,draw_x,i,seen,selected;
    f32 scale=D_803B5D10,delta;
    Slot *slot;
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
        y+=object_bytes_sum_global();top=y-4;
        fcvt_wrapper(text,D_803B89A4,countdown_state.text[141],D_803BADD8[player],countdown_state.text[142]);
        dispatch_handler(1);
        func_800B669C(1,3);
        camera_auto_follow(x,y,D_803B5D50[D_8014A108-1][player].width,
                           D_803B5D50[D_8014A108-1][player].height+D_803B5D50[D_8014A108-1][player].y-y,-1,0,text);
        func_800B669C(0,3);
        lines=camera_zoom_fov(D_803B5D50[D_8014A108-1][player].width,text);
        y+=object_bytes_sum_global()*(lines+1);
        label=countdown_state.indexed[countdown_state.indices[24]];
        dispatch_handler(D_803BADD0[player]==1 ? 22 : 1);
        state_utility(sound_pitch_diff_calc(label,x-5),y,label);
        label=countdown_state.indexed[countdown_state.indices[24]+1];
        dispatch_handler(D_803BADD0[player] ? 1 : 22);
        state_utility(x+5,y,label);
        y+=object_bytes_sum_global();
        crowd_cheer_play(D_803BAC08[player],x-D_803B5D50[D_8014A108-1][player].width/2-4,top,
                         x+D_803B5D50[D_8014A108-1][player].width/2+4,y+4);
    } else if(D_803BADD4[player]==1) {
        func_800B669C(0,3);
        func_800F6928(0.0f);
        y+=object_bytes_sum_global();top=y-4;
        fcvt_wrapper(text,countdown_state.text[145],D_803BADD8[player]);
        dispatch_handler(1);
        func_800B669C(1,3);
        camera_auto_follow(x,y,D_803B5D50[D_8014A108-1][player].width,
                           D_803B5D50[D_8014A108-1][player].height+D_803B5D50[D_8014A108-1][player].y-y,-1,0,text);
        func_800B669C(0,3);
        lines=camera_zoom_fov(D_803B5D50[D_8014A108-1][player].width,text);
        y+=object_bytes_sum_global()*lines;
        crowd_cheer_play(D_803BAC08[player],x-D_803B5D50[D_8014A108-1][player].width/2-4,top,
                         x+D_803B5D50[D_8014A108-1][player].width/2+4,y+4);
    } else {
        crowd_cheer_play(D_803BAC08[player],-20,0,-10,10);
        for(i=0,slot=D_803B470C[player];i<14;i++,slot++) {
            if(slot->angle>0.6981316804885864f && slot->angle<2.4434609413146973f)continue;
            if(!slot->alpha)continue;
            func_800ED66C((f32)(u32)slot->alpha);
            if(slot->angle<1.5707963705062866f) {
                func_800BEA3C(D_801146BC,D_801146C0);
                dispatch_handler(1);
            } else {
                func_800BEA3C(D_801146BC,D_801146C0);
                dispatch_handler(22);
            }
            func_800F6928(slot->position[2]-10.0f);
            brake_light_update(player,slot->position,&D_80150B70[player],0,point);
            if(slot->kind==9)draw_x=point[0]-105.0f*scale*300.0f/D_803B5D24;
            else if(slot->kind==7)draw_x=point[0]-65.0f*scale*300.0f/D_803B5D24;
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
                state_utility(draw_x,y,countdown_state.indexed[countdown_state.indices[21]+i]);
                label=D_8011EAD0[D_803BADBC[player]];
                state_utility(x,y,label);
            } else if(i==2) {
                delta=65.0f*scale*300.0f/D_803B5D24;
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
                    while(node) {
                        if((*node)->file && (*(*node)->file)->controller==D_803BADBC[player]) {
                            if(ABS(i-selected)==seen)break;
                            seen++;
                        }
                        if(i<selected)node=(*node)->previous;else node=(*node)->next;
                    }
                }
                if(!node) {
                    selected=D_803BADC8[player];
                    if(selected<i || D_803BADC0[player]<8)node=D_8012E6E0;
                    else node=D_8012E6E4;
                    while(node) {
                        if((*node)->file && (*(*node)->file)->controller==D_803BADBC[player]) {
                            if(ABS(i-((selected<i || D_803BADC0[player]<8) ? 3 : 10))==seen++)break;
                        }
                        if(selected<i || D_803BADC0[player]<8)node=(*node)->next;
                        else node=(*node)->previous;
                    }
                }
                if(node) {
                    label=(*node)->name;
                    if(D_8014A108==1)limit=98;
                    else limit=D_8014A108==2 ? 70 : 55;
                    if((u32)limit<(u32)object_manager_update(label,-1))mode_byte_set(0);
                    if(i==D_803BADC8[player] && node==D_803BAC80[player])dispatch_handler(22);
                    else {
                        dispatch_handler(1);
                        if(func_80395E24(node,player))dispatch_handler(0);
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
void func_803A7B58(s32);
void func_803A87D4(s32);
void func_803A8B74(s32);
void func_803A9088(s32);
s32 func_803A9EEC(void *callback_context)
{
    s32 player,x,y;
    render_helper(0.0f);
    for(player=0;player<D_8014A108;player++) {
        switch(D_803BACE0[player]) {
        case 0:func_803A7B58(player);break;
        case 1:func_803A87D4(player);break;
        case 2:crowd_cheer_play(D_803BAC08[player],-20,0,-10,10);break;
        case 3:func_803A8B74(player);break;
        case 4:func_803A9088(player);break;
        case 5: {
            u8 text[64];
            y=D_803B5D50[D_8014A108-1][player].y;
            x=D_803B5D50[D_8014A108-1][player].x;
            if(D_8014A108>=3)object_create(11);
            else if(D_8014A108==2)object_create(10);
            else object_create(13);
            fcvt_wrapper(text,D_803B89AC,countdown_state.text[29],player+1);
            dispatch_handler(D_801146AC[player]);
            state_utility(sound_pitch_diff_halved(text,x),y,text);
            y+=object_bytes_sum_global()*2;
            if(D_8014A108>=3)object_create(12);
            else if(D_8014A108==2)object_create(11);
            else object_create(10);
            dispatch_handler(17);
            state_utility(sound_pitch_diff_halved(D_803B89B4,x),y,D_803B89B4);
            break;
        }
        case 6: {
            u8 text[64];
            y=D_803B5D50[D_8014A108-1][player].y;
            x=D_803B5D50[D_8014A108-1][player].x;
            if(D_8014A108>=3)object_create(11);
            else if(D_8014A108==2)object_create(10);
            else object_create(13);
            fcvt_wrapper(text,D_803B89C0,countdown_state.text[29],player+1);
            dispatch_handler(D_801146AC[player]);
            state_utility(sound_pitch_diff_halved(text,x),y,text);
            y+=object_bytes_sum_global()*2;
            if(D_8014A108>=3)object_create(12);
            else if(D_8014A108==2)object_create(11);
            else object_create(10);
            dispatch_handler(17);
            state_utility(sound_pitch_diff_halved(D_803B89C8,x),y,D_803B89C8);
            break;
        }
        }
    }
    render_helper(-1.0f);
    return 1;
}
