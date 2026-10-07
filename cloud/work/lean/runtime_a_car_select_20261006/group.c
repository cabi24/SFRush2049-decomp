/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct FileInfo {u8 unknown00[16],controller;} FileInfo;
typedef struct Profile Profile;
struct Profile {Profile **next,**previous;FileInfo **file;u32 id0,id1;u8 name[24];u8 **stats;};
typedef struct Player {u32 held,pressed,unknown08,repeated;u8 unknown10[50];u8 paint,unknown43[5];Profile **owner;} Player;
typedef struct Record Record;
struct Record {Record **next;FileInfo **file;s8 track;u8 mode,name[14];u32 id0,id1;f32 time;u32 unknown24;void *active;};
extern Player D_8014A118[];
extern Record **D_80152028,**D_803BA010,**D_80152698[4];
extern s16 D_803BA01A[3];
extern s32 D_8014A110;
extern s8 D_801426EC,D_8014978C,D_80152570,D_80156994;
extern s32 func_8008AD04(u8 *,u8 *);
extern s32 menu_item_value_get(Record **);
extern void menu_transition(void **);
typedef struct Language {s32 unknown0;u8 **text;s32 unknown8;u16 *indices;u8 **indexed;} Language;
typedef struct Camera {f32 uv[3][3],position[3];u8 unknown30[104];} Camera;
typedef struct Slot {s32 kind,handle;f32 angle,matrix[3][3],position[3];u8 alpha,unknown3D[3];} Slot;
typedef struct CarSlot {u32 unknown0;f32 angle;s8 position;u8 car,unknown0A[2];} CarSlot;
typedef struct FiveParts FiveParts;
typedef struct Blit Blit;
typedef struct MultiBlit {char *name;s16 x,y,width,height,top,bottom,left,right;u32 z,alpha;s32 (*callback)(Blit *);u32 descriptor;} MultiBlit;
extern Language countdown_state;
extern s16 D_8014A108,D_80151AD0;
extern Camera D_80150B70[];
extern s8 D_803BA02C,D_803BA028[],D_803BA048[],D_803B9FD0[],D_803B9FD4[],D_803B9FD8[][13],D_803B3010[];
extern s16 D_803BA020[],D_803BA018[];
extern s32 D_803B28B8[],D_803BA038[],D_803BA1D0[],D_803B25CC[][2];
extern void *D_803B9AA0[48];
extern u8 D_80146180[4],D_80140BDC;
extern s8 D_801407D0,D_803B28A8;
extern f32 D_80154188,D_803B25A8,D_803B25AC,D_803B25B0,D_803B25B4,D_803B25B8,D_803B25BC,D_803B25C0,D_803B25C4;
extern f32 D_803B9E40[][2][12],D_8011418C[12];
extern FiveParts *D_803B9FC0[];
extern s8 D_80110E85[][13],D_80111049[][13],D_8011108D[][13],D_801111A9[][13],D_8011123D[][13],D_80111299[][13];
extern s8 D_80111611[][13],D_80111655[][13],D_80111699[][13],D_80111589[][13],D_801115CD[][13];
extern f32 D_801112DC[][13],D_801113E0[][13];
extern CarSlot D_803B9BC8[][13];
extern MultiBlit D_803B2AB8[];
extern Blit *D_803B28AC;
extern char D_803B85B0[],D_803B85BC[];
extern void audio_frame_sync(s32,s32,s32,s32,s32);
extern void *func_800A7D6C(void);
extern void init_state_begin(void);
extern void func_800A5A40(void);
extern void car_render_full(s16);
extern void audio_interrupt_handler(s32);
extern void sfx_position_3d(s32);
extern void particle_lifetime_set(void);
extern FiveParts *ambient_sound_set(s32,s32,s32,s32,s32,s32,s32,s32);
extern void math_utility(void *,void *);
extern s32 string_copy_format(char *,s8,s8,s8);
extern s32 sign_extend_call(s32,void *,s32,u32);
extern void model_data_load(s32,s32,u32);
extern void model_transform_setup(s32,s32,u32);
extern s8 func_800CDA60(Profile **,u8,u8);
extern void func_800C7578(Profile **,u8,u8,s8);
extern u8 func_800CDE88(Profile **);
extern u8 func_800CDDE8(void **);
extern void func_800CCA04(Profile **,u8);
extern s8 func_800F7604(s32,s32);
extern s8 func_800F75D0(s32,s32);
extern s8 func_800F75B0(s32,s32);
extern s8 func_800F7620(s32,s32);
extern s8 func_800F75EC(s32,s32);
extern Blit *sound_control(s16,s16,MultiBlit *,s16);
extern void particle_velocity_set(void);
extern void effects_update_emitters(void);
extern void set_race_state(void);
/* Genuine low-image services; definitions below recover part of their shared context. */
extern void func_8039FF04(s8);
extern void func_8039E3BC(s32);
typedef struct Vec3 {f32 v[3];} Vec3;
typedef struct PreparedCar {u8 unknown0,car,color[3],unknown5,flags,unknown7;} PreparedCar;
extern Record **D_8012E6F8;
extern s32 D_801174BC,D_801174B4,D_803BA030,D_803BA050[],D_803BA060[][19],D_803B9BA0[],D_803AF980;
extern s8 D_80157244,D_803BA04C[],D_8012E67C[],D_80138664[];
extern s16 D_801164BE,D_803B28B0[];
extern Slot D_803AF9A8[][44];
extern Vec3 D_803B3030;
extern PreparedCar D_80153E88[];
extern u8 D_803B857C[],D_803B8590[],D_803B85A0[];
extern s8 D_80110E78[13],D_8011103C[13],D_80111080[13],D_8011119C[13],D_80111230[13],D_8011128C[13];
extern s8 D_80111604[13],D_80111648[13],D_8011168C[13],D_8011157C[13],D_801115C0[13];
extern s8 func_800A1A3C(s32);
extern s8 func_800A361C(s32);
extern void func_800B5570(s32);
extern void func_8039D05C(void);
extern void func_8039EF08(void);
extern void func_8039D6A4(s8);
extern Record **func_8039E200(Record **,s8);
extern s32 entity_flags_apply(u32,u32,u32,u8);
extern void audio_doppler(s32);
extern void resource_type_select(s32);
extern void scheduler_recv(s32);
extern void func_800CDCAC(Profile **,u8);
extern void sfx_stop(s16,s16,s32);
extern void speed_mode0_wrapper(f32,f32);
extern void net_session_update(void);
extern void pause_resume(s32);
extern void brake_light_update(s32,f32 *,Camera *,void *,s16 *);
extern void *object_create(s32);
extern s16 object_bytes_sum_global(void);
extern s32 object_manager_update(u8 *,s16);
extern void crowd_cheer_play(FiveParts *,s32,s32,s32,s32);
extern s8 D_803BA00C;

extern s8 D_803B9FD0[];
extern s8 D_80110E85[][13];
extern s8 D_80111049[][13];
extern s8 D_8011108D[][13];
extern s8 D_801111A9[][13];
extern s8 D_8011123D[][13];
extern float D_803B28C8[][4];
extern float D_803B2948[][4];
extern float D_803B2978[][4];
extern float D_803B2A08[][4];
extern float D_803B2A58[][4];
extern float D_803BA190[][4];

void func_8039D300(s32 player)
{
    float *dest;
    float *first;
    float *second;
    float *third;
    float *fourth;
    float *fifth;
    s16 component;

    dest = D_803BA190[player];
    first = D_803B28C8[D_80110E85[player][D_803B9FD0[player]]];
    second = D_803B2948[D_80111049[player][D_803B9FD0[player]]];
    third = D_803B2978[D_8011108D[player][D_803B9FD0[player]]];
    fourth = D_803B2A08[D_801111A9[player][D_803B9FD0[player]]];
    fifth = D_803B2A58[D_8011123D[player][D_803B9FD0[player]]];
    for (component = 0; component < 4; component++) {
        dest[component] = (first[component] * second[component]) * third[component];
        dest[component] *= fourth[component];
        dest[component] *= fifth[component];
    }
}
void func_803A0498(void)
{
    Record **nearby[5];
    Record **node,**next,**best=0;
    Record *record;
    Profile *profile=*D_8014A118[0].owner;
    s32 i,j,count;
    for(i=0;i<4;i++)D_80152698[i]=0;
    for(i=0;i<5;i++)nearby[i]=0;
    for(i=0;i<3;i++)D_803BA01A[i]=-1;
    if(D_8014A110!=2)return;
    if(!D_801426EC) {
        node=D_80152028;
        while(node) {
            record=*node;next=record->next;
            if(record->track==D_8014978C && record->mode==D_80152570 &&
               !func_8008AD04(record->name,profile->name) && record->id0==profile->id0 && record->id1==profile->id1 &&
               (!best || record->time<(*best)->time)) {
                if(menu_item_value_get(node))best=node;
            }
            node=next;
        }
        for(i=0;i<3;i++) {
            node=&D_803BA010[i];record=*node;
            if(record->active && record->track==D_8014978C && record->mode==D_80152570 &&
               !func_8008AD04(record->name,profile->name) && record->id0==profile->id0 && record->id1==profile->id1 &&
               (!best || record->time<(*best)->time)) {
                if(menu_item_value_get(node))best=node;
            }
        }
        if(!best) {
            node=D_80152028;
            while(node) {
                record=*node;next=record->next;
                if(record->track==D_8014978C && record->mode==D_80152570) {
                    for(i=3;i!=0;i--) {
                        if(!D_80152698[i] || (*D_80152698[i])->time<record->time) {
                            if(menu_item_value_get(node)) {
                                for(j=1;j<i;j++)D_80152698[j]=D_80152698[j+1];
                                D_80152698[i]=node;break;
                            }
                        }
                    }
                }
                node=next;
            }
            for(node=D_803BA010;node<D_803BA010+3;node++) {
                record=*node;
                if(record->active && record->track==D_8014978C && record->mode==D_80152570) {
                    for(i=3;i!=0;i--) {
                        if(!D_80152698[i] || (*D_80152698[i])->time<record->time) {
                            if(menu_item_value_get(node)) {
                                for(j=1;j<i;j++)D_80152698[j]=D_80152698[j+1];
                                D_80152698[i]=node;break;
                            }
                        }
                    }
                }
            }
            for(i=1;i<3;i++) {
                if(!D_80152698[i] && D_80152698[i+1]) {
                    D_80152698[i]=D_80152698[i+1];D_80152698[i+1]=0;i=0;
                }
            }
        } else {
            nearby[2]=best;
            node=D_80152028;
            while(node) {
                record=*node;next=record->next;
                if(record->track==D_8014978C && record->mode==D_80152570 && node!=best) {
                    if(record->time<(*best)->time) {
                        if(!nearby[1] || (*nearby[1])->time<record->time) {
                            if(menu_item_value_get(node)) {nearby[0]=nearby[1];nearby[1]=node;}
                        } else if(!nearby[0] || (*nearby[0])->time<record->time) {
                            if(menu_item_value_get(node))nearby[0]=node;
                        }
                    } else {
                        if(!nearby[3] || record->time<(*nearby[3])->time) {
                            if(menu_item_value_get(node)) {nearby[4]=nearby[3];nearby[3]=node;}
                        } else if(!nearby[4] || record->time<(*nearby[4])->time) {
                            if(menu_item_value_get(node))nearby[4]=node;
                        }
                    }
                }
                node=next;
            }
            for(node=D_803BA010;node<D_803BA010+3;node++) {
                record=*node;
                if(record->active && record->track==D_8014978C && record->mode==D_80152570 && node!=best) {
                    if(record->time<(*best)->time) {
                        if(!nearby[1] || (*nearby[1])->time<record->time) {
                            if(menu_item_value_get(node)) {nearby[0]=nearby[1];nearby[1]=node;}
                        } else if(!nearby[0] || (*nearby[0])->time<record->time) {
                            if(menu_item_value_get(node))nearby[0]=node;
                        }
                    } else {
                        if(!nearby[3] || record->time<(*nearby[3])->time) {
                            if(menu_item_value_get(node)) {nearby[4]=nearby[3];nearby[3]=node;}
                        } else if(!nearby[4] || record->time<(*nearby[4])->time) {
                            if(menu_item_value_get(node))nearby[4]=node;
                        }
                    }
                }
            }
            count=1;
            for(i=0;i<5;i++)if(nearby[i] && count<4)D_80152698[count++]=nearby[i];
        }
    } else {
        node=D_80152028;
        while(node) {
            record=*node;next=record->next;
            if(record->track==D_8014978C && record->mode==D_80152570) {
                for(i=1;i<4;i++) {
                    if(!D_80152698[i] || record->time<(*D_80152698[i])->time) {
                        if(menu_item_value_get(node)) {
                            for(j=3;j>i;j--)D_80152698[j]=D_80152698[j-1];
                            D_80152698[i]=node;break;
                        }
                    }
                }
            }
            node=next;
        }
        for(node=D_803BA010;node<D_803BA010+3;node++) {
            record=*node;
            if(record->active && record->track==D_8014978C && record->mode==D_80152570) {
                for(i=1;i<4;i++) {
                    if(!D_80152698[i] || record->time<(*D_80152698[i])->time) {
                        if(menu_item_value_get(node)) {
                            for(j=3;j>i;j--)D_80152698[j]=D_80152698[j-1];
                            D_80152698[i]=node;break;
                        }
                    }
                }
            }
        }
    }
    if(!D_80156994) {
        if(D_80152698[2]) {menu_transition((void **)D_80152698[2]);D_80152698[2]=0;}
        if(D_80152698[3]) {menu_transition((void **)D_80152698[3]);D_80152698[3]=0;}
    }
}
void func_803A0498(void);
void func_8039D300(s32);
void func_803A0FD8(void)
{
    s32 player,car,i,mask,position;
    u8 selected;
    func_803A0498();
    audio_frame_sync(62,0,0,0,0);
    if(D_80156994) {
        audio_frame_sync(80,0,0,0,0);
        audio_frame_sync(61,0,0,0,0);
    } else if(D_8014978C>=6)audio_frame_sync(80,0,0,0,0);
    else audio_frame_sync(81,0,0,0,0);
    D_803BA02C=0;
    for(player=0;player<D_8014A108;player++) {
        D_803B28B8[player]=0;D_803BA048[player]=1;D_803BA038[player]=1;
        D_803BA028[player]=0;D_803BA1D0[player]=-1;
        func_8039D300(player);
    }
    for(player=0;player<D_8014A108;player++) {
        D_803BA020[player]=0;
        for(car=0;car<13;car++)D_803B9FD8[player][D_803BA020[player]++]=D_803B3010[car];
    }
    for(i=0;i<48;i++)D_803B9AA0[i]=func_800A7D6C();
    init_state_begin();
    D_80146180[0]=D_80146180[1]=D_80146180[2]=D_80146180[3]=0;
    D_80154188=90.0f;
    func_800A5A40();
    D_80151AD0=D_8014A108;
    if(D_80151AD0==3)D_80151AD0=4;
    if(D_8014A110==2) {
        D_803BA00C=3;D_80151AD0=4;
        if(!D_80156994) {D_803BA00C=1;D_80151AD0=2;}
    }
    car_render_full(D_80151AD0);
    if(D_80151AD0==1) {
        D_803B25A8=1.0f;D_803B25AC=0.04500000178813934f;D_803B25B0=40.0f;D_803B25B4=0.0f;
        D_803B25B8=-5.400000095367432f;D_803B25BC=-8.399999618530273f;D_803B25C0=15.0f;D_803B25C4=11.25f;
    } else {
        D_803B25A8=1.0f;D_803B25AC=0.054999999701976776f;D_803B25B0=40.0f;D_803B25B4=0.0f;
        D_803B25B8=-3.450000047683716f;D_803B25BC=-7.699999809265137f;D_803B25C0=15.0f;D_803B25C4=11.25f;
    }
    audio_interrupt_handler(1);sfx_position_3d(1);particle_lifetime_set();
    for(i=0;i<4;i++)D_803B9FC0[i]=ambient_sound_set(-20,0,-10,10,192,0,0,0);
    for(player=0;player<D_8014A108;player++) {
        math_utility(D_8011418C,D_803B9E40[player][0]);
        math_utility(D_8011418C,D_803B9E40[player][1]);
        D_803B25CC[player][0]=sign_extend_call(string_copy_format(D_803B85B0,0,D_80140BDC-1,1),D_803B9E40[player][0],-1,0x40000);
        D_803B25CC[player][1]=sign_extend_call(string_copy_format(D_803B85BC,0,D_80140BDC-1,1),D_803B9E40[player][1],-1,0);
        model_data_load(D_803B25CC[player][0],1,15);
        model_data_load(D_803B25CC[player][1],1,15);
        if(D_8014A110==2) {
            model_transform_setup(D_803B25CC[player][0],0,15);
            model_transform_setup(D_803B25CC[player][1],0,15);
        } else {
            mask=1<<player;
            model_transform_setup(D_803B25CC[player][0],0,mask);
            model_transform_setup(D_803B25CC[player][1],0,mask);
        }
        D_803B9E40[player][0][10]=2.0f;D_803B9E40[player][1][10]=2.0f;
        D_803B9E40[player][0][11]=15.0f;D_803B9E40[player][1][11]=15.0f;
        D_803B9E40[player][0][9]=0.0f;D_803B9E40[player][1][9]=0.0f;
        func_8039FF04(player);
        for(car=0;car<13;car++) {
            selected=car;
            D_80111049[player][car]=func_800CDA60(D_8014A118[player].owner,selected,0);
            D_8011108D[player][car]=func_800CDA60(D_8014A118[player].owner,selected,1);
            if(!func_800F7604(player,D_8011108D[player][car])) {
                do {
                    D_8011108D[player][car]--;
                    if(D_8011108D[player][car]<0) {D_8011108D[player][car]=0;break;}
                } while(!func_800F7604(player,D_8011108D[player][car]));
            }
            func_800C7578(D_8014A118[player].owner,selected,1,D_8011108D[player][car]);
            D_801111A9[player][car]=func_800CDA60(D_8014A118[player].owner,selected,2);
            if(!func_800F75D0(player,D_801111A9[player][car])) {
                do {
                    D_801111A9[player][car]--;
                    if(D_801111A9[player][car]<0) {D_801111A9[player][car]=0;break;}
                } while(!func_800F75D0(player,D_801111A9[player][car]));
            }
            func_800C7578(D_8014A118[player].owner,selected,2,D_801111A9[player][car]);
            D_8011123D[player][car]=func_800CDA60(D_8014A118[player].owner,selected,3);
            if(!func_800F75B0(player,D_8011123D[player][car])) {
                do {
                    D_8011123D[player][car]--;
                    if(D_8011123D[player][car]<0) {D_8011123D[player][car]=0;break;}
                } while(!func_800F75B0(player,D_8011123D[player][car]));
            }
            func_800C7578(D_8014A118[player].owner,selected,3,D_8011123D[player][car]);
            D_80111299[player][car]=func_800CDA60(D_8014A118[player].owner,selected,4);
            if(D_8014A110!=4 && D_80111299[player][car]>=2)D_80111299[player][car]=1;
            D_80111611[player][car]=func_800CDA60(D_8014A118[player].owner,selected,5);
            D_80111655[player][car]=func_800CDA60(D_8014A118[player].owner,selected,6);
            D_80111699[player][car]=func_800CDA60(D_8014A118[player].owner,selected,7);
            D_80111589[player][car]=func_800CDA60(D_8014A118[player].owner,selected,8);
            D_801115CD[player][car]=func_800CDA60(D_8014A118[player].owner,selected,9);
            D_801112DC[player+1][car]=func_800CDA60(D_8014A118[player].owner,selected,10)/100.0f+0.75f;
            D_801113E0[player+1][car]=func_800CDA60(D_8014A118[player].owner,selected,11)/100.0f+0.75f;
            if(!D_801407D0) {
                D_801112DC[player+1][car]=D_801112DC[0][car];
                D_801113E0[player+1][car]=D_801113E0[0][car];
            }
        }
        D_803B9FD0[player]=func_800CDE88(D_8014A118[player].owner);
        if(!func_800F7620(player,D_803B9FD0[player])) {
            do {
                if(D_803B9FD0[player]==0)D_803B9FD0[player]=12;
                else D_803B9FD0[player]--;
            } while(!func_800F7620(player,D_803B9FD0[player]));
        }
        D_803B9FD4[player]=0;
        while(D_803B9FD4[player]<D_803BA020[player] && D_803B9FD0[player]!=D_803B9FD8[player][D_803B9FD4[player]])D_803B9FD4[player]++;
        if(D_803B9FD4[player]>=D_803BA020[player]) {
            D_803B9FD4[player]=player+1;
            D_803B9FD0[player]=D_803B9FD8[player][D_803B9FD4[player]];
            func_800CCA04(D_8014A118[player].owner,D_803B9FD0[player]);
        }
        D_803BA018[player]=-1;
        func_8039E3BC(player);
        for(car=0,position=-D_803B9FD4[player];car<D_803BA020[player];car++,position++) {
            D_803B9BC8[player][car].position=position;
            if(D_803B9BC8[player][car].position<0)D_803B9BC8[player][car].position+=D_803BA020[player];
            D_803B9BC8[player][car].car=D_803B9FD8[player][car]+player*13;
            D_803B9BC8[player][car].angle=3.1415927410125732f;
        }
        if(D_8014A110==6)D_8014A118[player].paint=6;
        else {
            D_8014A118[player].paint=func_800CDDE8((void **)D_8014A118[player].owner);
            if(!func_800F75EC(player,D_8014A118[player].paint)) {
                do {
                    D_8014A118[player].paint-=2;
                    if((s32)D_8014A118[player].paint<0) {D_8014A118[player].paint+=2;break;}
                } while(!func_800F75EC(player,D_8014A118[player].paint));
            }
        }
        for(car=0;car<13;car++)D_80110E85[player][car]=D_8014A118[player].paint;
    }
    D_803B28AC=sound_control(0,0,D_803B2AB8,38);
    particle_velocity_set();effects_update_emitters();set_race_state();D_803B28A8=1;
}
void func_803A0498(void);
void func_803A0FD8(void);
void func_8039D300(s32);
void func_803A1EAC(void)
{
    s32 feedback=1,offset,waiting,other,j,x,change,car,all_same;
    s16 i,player,y,width,height;
    Record **record,**next;
    Player *state;
    s16 point[2];
    Vec3 position;
    if(!D_80156994)D_803BA010=D_8012E6F8;
    else {
        offset=D_80152570 ? 6 : 0;
        D_803BA010=D_8012E6F8+(offset+D_8014978C)*3;
    }
    if(D_801174BC!=1) {
        if(D_801174B4==D_801174BC)D_801174BC=1;
        else {func_803A0498();func_800B5570(0x40000);return;}
    }
    for(i=1;i<4;i++) {
        record=D_80152028;
        while(record) {
            next=(*record)->next;
            if((!(*record)->file || func_800A1A3C((*(*record)->file)->controller)) && record==D_80152698[i])break;
            record=next;
        }
        if(!record) {
            for(player=0;player<3;player++) {
                record=&D_803BA010[player];
                if(record==D_80152698[i])break;
                record=0;
            }
        }
        D_80152698[i]=record;
    }
    if(!D_803B28A8)func_803A0FD8();
    func_8039EF08();
    for(i=0;i<D_8014A108;i++) {
        if((*D_8014A118[i].owner)->file && func_800A361C((*(*D_8014A118[i].owner)->file)->controller)) {
            func_8039D05C();func_800B5570(32);D_803BA030=0;return;
        }
    }
    if(D_80157244) {func_8039D05C();func_800B5570(4);D_803BA030=0;return;}
    D_803BA030++;
    for(player=0;player<D_8014A108;player++) {
        if(!D_803BA02C && (D_8014A118[player].pressed&4)) {
            if(D_803BA028[player]==1)D_803BA028[player]=0;
            else {
                entity_flags_apply(38,0,1,0);func_8039D05C();func_800B5570(128);D_803BA030=0;return;
            }
        }
        if(D_803BA028[player]) {
            position=D_803B3030;
            if(D_80151AD0==1)object_create(13);else object_create(12);
            brake_light_update(player,position.v,&D_80150B70[player],0,point);
            x=point[0];y=point[1]-object_bytes_sum_global();waiting=0;
            for(j=0;j<D_8014A108;j++)if(j!=player && !D_803BA028[j]) {other=j;waiting++;}
            if(!waiting) {
                y+=object_bytes_sum_global();height=object_bytes_sum_global();width=object_manager_update(D_803B857C,-1);
            } else {
                width=object_manager_update(countdown_state.text[151],-1);height=object_bytes_sum_global()*2;
                if(waiting==1)width=(u32)object_manager_update((*D_8014A118[other].owner)->name,-1)<(u32)width ? width : object_manager_update((*D_8014A118[other].owner)->name,-1);
                else width=(u32)object_manager_update(countdown_state.text[31],-1)<(u32)width ? width : object_manager_update(countdown_state.text[31],-1);
            }
            y-=object_bytes_sum_global()/2;
            crowd_cheer_play(D_803B9FC0[player],x-width/2-4,y-4,x+width/2+4,y+height+4);
            func_8039D6A4(player);
            continue;
        }
        state=&D_8014A118[player];
        crowd_cheer_play(D_803B9FC0[player],-20,0,-10,10);
        if(state->pressed&3) {
            resource_type_select(state->pressed);
            D_803BA028[player]=1;D_80153E88[player].flags=160;
            car=D_803B9FD0[player];D_80153E88[player].car=car;
            if(D_8014A110==6 && D_801164BE==0) {
                D_80153E88[player].color[0]=D_80138664[D_8012E67C[player]];
                D_80153E88[player].color[1]=D_80138664[D_8012E67C[player]];
                D_80153E88[player].color[2]=D_80138664[D_8012E67C[player]];
            } else {
                D_80153E88[player].color[0]=D_80111611[player][car];
                D_80153E88[player].color[1]=D_80111655[player][car];
                D_80153E88[player].color[2]=D_80111699[player][car];
            }
            D_803BA030=0;
        }
        if(state->repeated&0x400) {
            entity_flags_apply(40,0,1,0);
            if(--D_803B28B8[player]<0)D_803B28B8[player]=D_803BA050[player]-1;
        } else if(state->repeated&0x800) {
            entity_flags_apply(39,0,1,0);
            if(++D_803B28B8[player]>=D_803BA050[player])D_803B28B8[player]=0;
        } else if(state->repeated&0x3080) {
            if(state->repeated&0x1000)change=-1;
            else change=(state->repeated&0x2000) ? 1 : 0;
            if(!D_803BA048[player] || (!change && !D_803BA04C[player])) {
                entity_flags_apply(42,0,1,0);
                goto changed_selection;
            }
            switch(D_803BA060[player][D_803B28B8[player]]) {
            case 0:
                if(change) {
                    do {
                        D_803B9FD4[player]+=change;
                        if(D_803B9FD4[player]<0)D_803B9FD4[player]=D_803BA020[player]-1;
                        else if(D_803B9FD4[player]>=D_803BA020[player])D_803B9FD4[player]=0;
                    } while(!func_800F7620(player,D_803B9FD8[player][D_803B9FD4[player]]));
                }
                D_803B9FD0[player]=D_803B9FD8[player][D_803B9FD4[player]];
                if(D_8014A110==6)state->paint=6;else state->paint=func_800CDDE8((void **)state->owner);
                for(i=0;i<13;i++)D_80110E85[player][i]=state->paint;
                func_8039E3BC(player);D_803B9BA0[player]=1;
                break;
            case 1:
                if(D_8014A110==6) {feedback=0;break;}
                if(change) {
                    do {
                        D_80110E85[player][D_803B9FD0[player]]+=change;
                        if(D_80110E85[player][D_803B9FD0[player]]<0)D_80110E85[player][D_803B9FD0[player]]=7;
                        else if(D_80110E85[player][D_803B9FD0[player]]>=8)D_80110E85[player][D_803B9FD0[player]]=0;
                    } while(!func_800F75EC(player,D_80110E85[player][D_803B9FD0[player]]) ||
                            (D_8014A110==6 && (D_80110E85[player][D_803B9FD0[player]]&1)));
                } else {
                    car=D_803B9FD0[player];D_80110E85[player][car]=D_80110E78[car];
                }
                state->paint=D_80110E85[player][D_803B9FD0[player]];
                func_800CDCAC(state->owner,state->paint);break;
            case 2:
                car=D_803B9FD0[player];
                if(change) {
                    D_80111049[player][car]+=change;
                    if(D_80111049[player][car]<0)D_80111049[player][car]=2;
                    else if(D_80111049[player][car]>=3)D_80111049[player][car]=0;
                } else {
                    car=D_803B9FD0[player];D_80111049[player][car]=D_8011103C[car];
                }
                func_800C7578(state->owner,car,0,D_80111049[player][car]);break;
            case 3:
                if(change) {
                    do {
                        D_8011108D[player][D_803B9FD0[player]]+=change;
                        if(D_8011108D[player][D_803B9FD0[player]]<0)D_8011108D[player][D_803B9FD0[player]]=8;
                        else if(D_8011108D[player][D_803B9FD0[player]]>=9)D_8011108D[player][D_803B9FD0[player]]=0;
                    } while(!func_800F7604(player,D_8011108D[player][D_803B9FD0[player]]));
                } else {
                    car=D_803B9FD0[player];D_8011108D[player][car]=D_80111080[car];
                }
                car=D_803B9FD0[player];
                func_800C7578(state->owner,car,1,D_8011108D[player][car]);break;
            case 4:
                if(change) {
                    do {
                        D_801111A9[player][D_803B9FD0[player]]+=change;
                        if(D_801111A9[player][D_803B9FD0[player]]<0)D_801111A9[player][D_803B9FD0[player]]=4;
                        else if(D_801111A9[player][D_803B9FD0[player]]>=5)D_801111A9[player][D_803B9FD0[player]]=0;
                    } while(!func_800F75D0(player,D_801111A9[player][D_803B9FD0[player]]));
                } else {
                    car=D_803B9FD0[player];D_801111A9[player][car]=D_8011119C[car];
                }
                car=D_803B9FD0[player];
                func_800C7578(state->owner,car,2,D_801111A9[player][car]);break;
            case 5:
                if(change) {
                    do {
                        D_8011123D[player][D_803B9FD0[player]]+=change;
                        if(D_8011123D[player][D_803B9FD0[player]]<0)D_8011123D[player][D_803B9FD0[player]]=5;
                        else if(D_8011123D[player][D_803B9FD0[player]]>=6)D_8011123D[player][D_803B9FD0[player]]=0;
                    } while(!func_800F75B0(player,D_8011123D[player][D_803B9FD0[player]]));
                } else {
                    car=D_803B9FD0[player];D_8011123D[player][car]=D_80111230[car];
                }
                car=D_803B9FD0[player];
                func_800C7578(state->owner,car,3,D_8011123D[player][car]);break;
            case 6:
                car=D_803B9FD0[player];
                if(change) {
                    D_80111299[player][car]+=change;
                    if(D_80111299[player][car]<0)D_80111299[player][car]=(D_8014A110==4 ? 2 : 1);
                    else if(D_80111299[player][car]>(D_8014A110==4 ? 2 : 1))D_80111299[player][car]=0;
                } else {
                    car=D_803B9FD0[player];D_80111299[player][car]=D_8011128C[car];
                }
                func_800C7578(state->owner,car,4,D_80111299[player][car]);break;
            case 7:
                car=D_803B9FD0[player];
                if(change) {
                    D_80111611[player][car]+=change;
                    if(D_80111611[player][car]<0)D_80111611[player][car]=31;
                    else if(D_80111611[player][car]>=32)D_80111611[player][car]=0;
                } else {
                    car=D_803B9FD0[player];D_80111611[player][car]=D_80111604[car];
                }
                func_800C7578(state->owner,car,5,D_80111611[player][car]);break;
            case 8:
                car=D_803B9FD0[player];
                if(change) {
                    D_80111655[player][car]+=change;
                    if(D_80111655[player][car]<0)D_80111655[player][car]=31;
                    else if(D_80111655[player][car]>=32)D_80111655[player][car]=0;
                } else {
                    car=D_803B9FD0[player];D_80111655[player][car]=D_80111648[car];
                }
                func_800C7578(state->owner,car,6,D_80111655[player][car]);break;
            case 9:
                car=D_803B9FD0[player];
                if(change) {
                    D_80111699[player][car]+=change;
                    if(D_80111699[player][car]<0)D_80111699[player][car]=31;
                    else if(D_80111699[player][car]>=32)D_80111699[player][car]=0;
                } else {
                    car=D_803B9FD0[player];D_80111699[player][car]=D_8011168C[car];
                }
                func_800C7578(state->owner,car,7,D_80111699[player][car]);break;
            case 10:
                if(change) {
                    do {
                        D_8012E67C[player]+=change;
                        if(D_8012E67C[player]<0)D_8012E67C[player]=3;
                        else if(D_8012E67C[player]>=4)D_8012E67C[player]=0;
                        all_same=1;
                        for(i=0;i<D_8014A108;i++)if(i!=player && D_8012E67C[i]!=D_8012E67C[player])all_same=0;
                    } while(all_same==1);
                } else D_8012E67C[player]=player;
                break;
            case 11:
                car=D_803B9FD0[player];
                if(change) {
                    D_80111589[player][car]+=change;
                    if(D_80111589[player][car]<0)D_80111589[player][car]=20;
                    else if(D_80111589[player][car]>=21)D_80111589[player][car]=0;
                } else D_80111589[player][car]=D_8011157C[car];
                func_800C7578(state->owner,car,8,D_80111589[player][car]);break;
            case 12:
                car=D_803B9FD0[player];feedback=0;
                if(change) {
                    D_801115CD[player][car]+=change;
                    if(D_801115CD[player][car]<0)D_801115CD[player][car]=9;
                    else if(D_801115CD[player][car]>=10)D_801115CD[player][car]=0;
                } else D_801115CD[player][car]=D_801115C0[car];
                scheduler_recv(D_803BA1D0[player]);
                D_803BA1D0[player]=entity_flags_apply(D_801115CD[player][D_803B9FD0[player]]+26,0,1,0);
                car=D_803B9FD0[player];func_800C7578(state->owner,car,9,D_801115CD[player][car]);break;
            case 13:
                car=D_803B9FD0[player];
                if(change) {
                    D_801112DC[player+1][car]+=change*0.05000000074505806f;
                    if(D_801112DC[player+1][car]<0.7400000095367432f)D_801112DC[player+1][car]=1.5f;
                    else if(D_801112DC[player+1][car]>1.600000023841858f)D_801112DC[player+1][car]=0.75f;
                } else D_801112DC[player+1][car]=D_801112DC[0][car];
                func_800C7578(state->owner,car,10,(s8)(D_801112DC[player+1][car]*100.0f-75.0f));break;
            case 14:
                car=D_803B9FD0[player];
                if(change) {
                    D_801113E0[player+1][car]+=change*0.05000000074505806f;
                    if(D_801113E0[player+1][car]<0.7400000095367432f)D_801113E0[player+1][car]=1.5f;
                    else if(D_801113E0[player+1][car]>1.5f)D_801113E0[player+1][car]=0.75f;
                } else D_801113E0[player+1][car]=D_801113E0[0][car];
                func_800C7578(state->owner,car,11,(s8)(D_801113E0[player+1][car]*100.0f-75.0f));break;
            case 15:
                car=D_803B9FD0[player];
                if(change) {
                    D_803B28B0[player]+=change;
                    if(D_803B28B0[player]<0)D_803B28B0[player]=10;
                    else if(D_803B28B0[player]>=11)D_803B28B0[player]=0;
                } else D_803B28B0[player]=0;
                sfx_stop(car,player,D_803B28B0[player]);break;
            case 16:D_80152698[1]=func_8039E200(D_80152698[1],change);break;
            case 17:D_80152698[2]=func_8039E200(D_80152698[2],change);break;
            case 18:D_80152698[3]=func_8039E200(D_80152698[3],change);break;
            }
            if(feedback)audio_doppler(state->repeated);
        }
        if(D_803B28B8[player]-1<D_803BA038[player])D_803BA038[player]=D_803B28B8[player]-1;
        if(D_803BA038[player]<D_803B28B8[player]-1)D_803BA038[player]=D_803B28B8[player]-1;
        if(D_803BA050[player]-3<D_803BA038[player])D_803BA038[player]=D_803BA050[player]-3;
        if(D_803BA038[player]<=0)D_803BA038[player]=1;
        D_803BA048[player]=1;
changed_selection:
        car=D_803B9FD0[player];
        if(D_803BA018[player]!=car) {
            if(D_803BA018[player]>=0) {func_800CCA04(state->owner,car);car=D_803B9FD0[player];}
            D_803BA018[player]=car;
        }
        if(D_8014A110==2) {
            s16 ghost_point[2];
            Vec3 ghost_position;
            for(j=1;j<=D_803BA00C;j++)if(D_803B9FC0[j]) {
                ghost_position.v[0]=D_803AF9A8[0][38].position[0];
                ghost_position.v[1]=-D_803AF9A8[0][38].position[1]*0.5f;
                ghost_position.v[2]=D_803AF9A8[0][38].position[2];
                brake_light_update(j,ghost_position.v,&D_80150B70[j],0,ghost_point);
                x=ghost_point[0];y=ghost_point[1];object_create(12);
                if(!D_80152698[j]) {
                    if(!D_803AF980)width=object_manager_update(countdown_state.text[225],-1);
                    else width=object_manager_update(countdown_state.text[224],-1);
                    height=object_bytes_sum_global();
                } else {
                    width=object_manager_update((*D_80152698[j])->name,-1);
                    width=(u32)object_manager_update(D_803B8590,-1)<(u32)width ? width : object_manager_update(D_803B85A0,-1);
                    height=object_bytes_sum_global()*2;
                }
                y-=object_bytes_sum_global()/2;
                crowd_cheer_play(D_803B9FC0[j],x-width/2-4,y-4,x+width/2+4,y+height+4);
            }
        }
        func_8039D6A4(player);func_8039D300(player);
    }
    D_803BA02C=1;
    for(i=0;i<D_8014A108;i++)if(!D_803BA028[i])D_803BA02C=0;
    if(D_803BA02C && D_803BA030>=10) {
        speed_mode0_wrapper(0.0f,3.0f);func_800B5570(0x40000);func_8039D05C();
        if(D_8014A110==3)net_session_update();
        if(D_8014A110==2) {
            record=D_80152028;
            while(record) {menu_transition((void **)record);record=(*record)->next;}
            for(i=1;i<D_803BA00C;i++)if(!D_80152698[i]) {D_80152698[i]=D_80152698[i+1];D_80152698[i+1]=0;}
            for(i=1;i<D_803BA00C;i++)if(!D_80152698[i]) {D_80152698[i]=D_80152698[i+1];D_80152698[i+1]=0;}
        }
        pause_resume(0);
    }
}

extern s8 D_803B3020[];
s32 func_8039D494(s32 item, s32 player)
{
    s32 available = 1;
    if ((item == 13 || item == 14) && !D_801407D0) available = 0;
    if (!D_80156994) {
        if (item == 11 && D_8014978C >= 0 && D_8014978C < 6) available = 0;
        if (item == 17 || item == 18) available = 0;
    }
    if (!D_803AF980 && (item == 16 || item == 17 || item == 18)) available = 0;
    if (item == 6 && (D_8014A110 == 2 || D_8014A110 == 6)) available = 0;
    if ((item == 7 || item == 8 || item == 9) && D_8014A110 == 6 && !D_801164BE) available = 0;
    if (item == 7 && !(D_803B3020[D_803B9FD0[player]] & 1)) available = 0;
    if (item == 8 && !(D_803B3020[D_803B9FD0[player]] & 2)) available = 0;
    if (item == 9 && !(D_803B3020[D_803B9FD0[player]] & 4)) available = 0;
    if (item == 10 && D_8014A110 != 6) available = 0;
    if ((item == 16 || item == 17 || item == 18) && D_8014A110 != 2) available = 0;
    if (item == 12 || item == 15) available = 0;
    return available;
}

void func_8039E3BC(s32 player)
{
    s32 item;
    D_803BA050[player]=0;
    for(item=0;item<19;item++) {
        if(func_8039D494(item,player))D_803BA060[player][D_803BA050[player]++]=item;
    }
}

extern Record **D_8015202C;
Record **func_8039E200(Record **node,s8 direction)
{
    Record **next;
    Record *record;
    if(!node) {
        if(direction>0)node=D_80152028;
        else node=D_803BA010+2;
    }
    while(node) {
        record=*node;
        if(direction>0)next=record->next;
        else if(node==D_80152028)next=0;
        else {
            for(next=D_80152028;next && (*next)->next!=node;next=(*next)->next) {}
        }
        if(record->track==D_8014978C && record->mode==D_80152570 &&
           node!=D_80152698[1] && node!=D_80152698[2] && node!=D_80152698[3] &&
           (!record->file || menu_item_value_get(node)) && (*node)->unknown24)break;
        if(!next) {
            if(node==D_803BA010) {
                if(direction>0)next=D_803BA010+1;
                else next=D_8015202C;
            } else if(node==D_803BA010+1) {
                if(direction>0)next=D_803BA010+2;
                else next=D_803BA010;
            } else if(node==D_803BA010+2) {
                if(direction<0)next=D_803BA010+1;
            } else if(direction>0)next=D_803BA010;
        }
        node=next;
    }
    return node;
}

typedef struct PanelPart {s16 handle,auxiliary;u8 unknown04[48];} PanelPart;
extern PanelPart D_803B2618[][11];
extern s32 D_803B25C8,D_803B9B90[];
extern void wheel_setup_initial(s32);
extern void sound_handles_array_clear(FiveParts *);
extern void sound_call_minimal(s16);
extern void sound_stop(Blit *);
extern void ambient_sounds_clear(void);
extern void func_800A5B3C(void);
void func_8039D05C(void)
{
    s32 player,i;
    FiveParts **box;
    Slot *slot;
    PanelPart *part;
    wheel_setup_initial(62);
    if(D_80156994) {
        wheel_setup_initial(80);
        wheel_setup_initial(61);
    } else if(D_8014978C>=6)wheel_setup_initial(80);
    else wheel_setup_initial(81);
    for(box=D_803B9FC0;box<D_803B9FC0+4;box++) {
        if(*box) {sound_handles_array_clear(*box);*box=0;}
    }
    for(player=0;player<D_8014A108;player++) {
        for(i=0;i<44;i++) {
            slot=&D_803AF9A8[(s8)player][i];
            if(slot->handle!=-1) {sound_call_minimal(slot->handle);slot->handle=-1;}
        }
        D_803B9B90[(s8)player]=0;
        if(D_803B25CC[player][0]!=-1) {sound_call_minimal(D_803B25CC[player][0]);D_803B25CC[player][0]=-1;}
        if(D_803B25CC[player][1]!=-1) {sound_call_minimal(D_803B25CC[player][1]);D_803B25CC[player][1]=-1;}
        if(player<=0) {
            for(i=0,part=D_803B2618[player];i<11;i++,part++) {
                if(part->handle!=-1) {sound_call_minimal(part->handle);part->handle=part->auxiliary=-1;}
            }
            if(D_803B25C8!=-1) {sound_call_minimal(D_803B25C8);D_803B25C8=-1;}
        }
    }
    if(D_803B28AC) {sound_stop(D_803B28AC);D_803B28AC=0;}
    ambient_sounds_clear();
    D_80154188=90.0f;D_80151AD0=1;
    func_800A5A40();func_800A5B3C();
    for(player=0;player<D_8014A108;player++)scheduler_recv(D_803BA1D0[player]);
    D_803B28A8=0;
}

typedef struct Color4 {u32 word;} Color4;
extern Color4 D_803B305C;
extern f32 D_803B3060;
extern char *D_803AF984[];
extern char D_803B8630[],D_803B8640[],D_803B8648[],D_803B864C[];
extern void *memcpy(void *,const void *,u32);
extern void func_8008E06C(s16,Color4 *);
extern void *func_800B24EC(char *,s16 *,s32,s8,s32);
extern void func_8008D870(s16,void *,s16);
extern void func_800B5898(f32,void *);
extern void func_800B5940(f32,void *);
extern void func_8008B32C(void *,void *,f32);
void func_8039FF04(s8 player)
{
    Color4 color=D_803B305C;
    s16 count;
    s32 i,mask;
    Slot *slot,*row;
    f32 scale;
    if(D_80151AD0==1)D_803B3060=1.0f;
    for(i=0,slot=D_803AF9A8[player];i<44;i++,slot++) {
        if(slot->handle==-1) {
            memcpy(slot->matrix,D_8011418C,36);
            mask=1<<player;
            slot->handle=sign_extend_call(string_copy_format(D_803AF984[slot->kind],0,(s8)(D_80140BDC-1),1),
                                          slot->matrix,-1,(slot->kind==0 || slot->kind==1 || slot->kind==2) ? 0x42000 : 0);
            func_8008E06C(slot->handle,&color);
            model_data_load(slot->handle,1,15);
            model_transform_setup(slot->handle,0,mask);
        }
    }
    row=D_803AF9A8[player];
    func_8008D870(row[38].handle,func_800B24EC(D_803B8630,&count,0,(s8)(D_80140BDC-1),1),-1);
    memcpy(row[38].matrix,D_8011418C,36);
    func_800B5898(-1.5707963705062866f,row[38].matrix);
    row[38].position[0]=0.0f;
    if(D_80151AD0>=2)row[38].position[1]=105.0f;
    else row[38].position[1]=115.0f;
    scale=1.0f/D_803B3060;
    row[38].position[0]*=scale;row[38].position[1]*=scale;row[38].position[2]=200.0f*scale;
    if(D_8014A110==2)model_transform_setup(row[38].handle,0,15);
    func_8008D870(row[39].handle,func_800B24EC(D_803B8640,&count,0,(s8)(D_80140BDC-1),1),-1);
    func_8008D870(row[40].handle,func_800B24EC(D_803B8648,&count,0,(s8)(D_80140BDC-1),1),-1);
    memcpy(row[40].matrix,D_8011418C,36);
    func_800B5898(-1.5707963705062866f,row[40].matrix);
    row[40].position[0]=75.0999984741211f;
    if(D_80151AD0>=2)row[40].position[1]=50.20000076293945f;
    else row[40].position[1]=55.20000076293945f;
    row[40].position[2]=100.0f;
    func_8008B32C(row[40].matrix,row[40].matrix,0.5f);
    memcpy(row[41].matrix,D_8011418C,36);
    func_800B5898(-1.5707963705062866f,row[41].matrix);
    func_800B5940(3.1415927410125732f,row[41].matrix);
    row[41].position[0]=87.0999984741211f;
    if(D_80151AD0>=2)row[41].position[1]=50.20000076293945f;
    else row[41].position[1]=55.20000076293945f;
    row[41].position[2]=100.0f;
    func_8008D870(row[42].handle,func_800B24EC(D_803B864C,&count,0,(s8)(D_80140BDC-1),1),-1);
    memcpy(row[42].matrix,D_8011418C,36);
    func_800B5898(-1.5707963705062866f,row[42].matrix);
    row[42].position[0]=-75.0999984741211f;
    if(D_80151AD0>=2)row[42].position[1]=50.20000076293945f;
    else row[42].position[1]=55.20000076293945f;
    scale=1.0f/D_803B3060;
    row[42].position[0]*=scale;row[42].position[1]*=scale;row[42].position[2]=100.0f*scale;
    func_8008B32C(row[42].matrix,row[42].matrix,0.5f);
    memcpy(row[43].matrix,D_8011418C,36);
    func_800B5898(-1.5707963705062866f,row[43].matrix);
    row[43].position[0]=-87.0999984741211f;
    if(D_80151AD0>=2)row[43].position[1]=50.20000076293945f;
    else row[43].position[1]=55.20000076293945f;
    scale=1.0f/D_803B3060;
    row[43].position[0]*=scale;row[43].position[1]*=scale;row[43].position[2]=100.0f*scale;
    D_803B9B90[player]=1;D_803B9BA0[player]=1;
    particle_velocity_set();
}

extern f32 fabsf(f32);
#pragma intrinsic(fabsf)
extern f32 D_8002EB94,D_803BA200[],D_803BA210[];
extern Color4 D_803B3064;
extern char D_803B8650[],D_803B8660[];
void func_8039D6A4(s8 player)
{
    f32 previous[3],anchor,y,delta;
    Color4 color=D_803B3064;
    s16 count;
    s8 settled=1;
    s32 item,mask;
    void *texture;
    Slot *row,*slot;
    if(!D_803B9B90[player])return;
    if((D_803BA050[player]-1)*D_803B25B0*D_803B25AC>D_803B25B8-D_803B25BC)
        anchor=D_803B25B8-(D_803B28B8[player]*(D_803B25B8-D_803B25BC))/(D_803BA050[player]-1);
    else anchor=D_803B25B8-D_803B28B8[player]*D_803B25B0*D_803B25AC;
    row=D_803AF9A8[player];
    memcpy(previous,row[39].position,12);
    row[39].angle+=6.2831854820251465f*D_8002EB94;
    if(row[39].angle>6.2831854820251465f)row[39].angle-=6.2831854820251465f;
    row[39].position[0]=D_803B25B4*D_803B25C4/D_803B25C0-170.0f*D_803B25AC;
    row[39].position[1]=anchor*D_803B25C4/D_803B25C0;
    row[39].position[2]=D_803B25C4;
    if(!D_803B9BA0[player]) {
        delta=fabsf(row[39].position[1]-previous[1]);
        if(D_803BA200[player]<delta*60.0f*D_803B25AC/10.0f) {
            D_803BA200[player]=delta*60.0f*D_803B25AC/10.0f;
            D_803BA200[player]=D_803BA200[player]<540.0f*D_803B25AC ? D_803BA200[player] : 540.0f*D_803B25AC;
            D_803BA200[player]=D_803BA200[player]>180.0f*D_803B25AC ? D_803BA200[player] : 180.0f*D_803B25AC;
        }
        if(previous[1]+D_803BA200[player]*D_8002EB94<row[39].position[1])
            row[39].position[1]=previous[1]+D_803BA200[player]*D_8002EB94;
        else if(row[39].position[1]<previous[1]-D_803BA200[player]*D_8002EB94)
            row[39].position[1]=previous[1]-D_803BA200[player]*D_8002EB94;
        else D_803BA200[player]=180.0f*D_803B25AC;
    }
    func_8008B32C(D_8011418C,row[39].matrix,D_803B25AC*0.4000000059604645f);
    func_800B5940(row[39].angle,row[39].matrix);
    func_800B5898(1.5707963705062866f,row[39].matrix);
    y=D_803B28B8[player]*D_803B25B0*D_803B25AC+anchor;
    for(item=0,slot=row;item<19;item++,slot++) {
        if(item==D_803BA060[player][D_803B28B8[player]]) {
            if(slot->angle<3.1415927410125732f)slot->angle+=12.566370964050293f*D_8002EB94;
            if(slot->angle>3.1415927410125732f)slot->angle=3.1415927410125732f;
            if(D_803B9BA0[player])slot->angle=3.1415927410125732f;
        } else {
            if(slot->angle>0.0f)slot->angle-=12.566370964050293f*D_8002EB94;
            if(slot->angle<0.0f)slot->angle=0.0f;
            if(D_803B9BA0[player])slot->angle=0.0f;
        }
        memcpy(previous,slot->position,12);
        slot->position[0]=-120.0f*D_803B25AC+D_803B25B4;
        slot->position[1]=y;
        slot->position[2]=D_803B25C0;
        if(!D_803B9BA0[player]) {
            delta=fabsf(slot->position[1]-previous[1]);
            if(D_803BA210[player]<delta*60.0f/10.0f) {
                D_803BA210[player]=delta*60.0f/10.0f;
                D_803BA210[player]=D_803BA210[player]<540.0f*D_803B25AC ? D_803BA210[player] : 540.0f*D_803B25AC;
                D_803BA210[player]=D_803BA210[player]>180.0f*D_803B25AC ? D_803BA210[player] : 180.0f*D_803B25AC;
            }
            if(previous[1]+D_803BA210[player]*D_8002EB94<slot->position[1]) {
                slot->position[1]=previous[1]+D_803BA210[player]*D_8002EB94;settled=0;
            } else if(slot->position[1]<previous[1]-D_803BA210[player]*D_8002EB94) {
                slot->position[1]=previous[1]-D_803BA210[player]*D_8002EB94;settled=0;
            }
        }
        slot[19].position[0]=120.0f*D_803B25AC+D_803B25B4;
        slot[19].position[1]=slot->position[1];slot[19].position[2]=slot->position[2];
        slot[19].angle=slot->angle;
        func_8008B32C(D_8011418C,slot->matrix,D_803B25AC);
        func_8008B32C(D_8011418C,slot[19].matrix,D_803B25AC);
        func_800B5898(slot->angle-1.5707963705062866f,slot->matrix);
        func_800B5898(slot[19].angle-1.5707963705062866f,slot[19].matrix);
        if(slot->angle>1.5707963705062866f)texture=func_800B24EC(D_803B8650,&count,0,(s8)(D_80140BDC-1),1);
        else texture=func_800B24EC(D_803B8660,&count,0,(s8)(D_80140BDC-1),1);
        func_8008D870(slot->handle,texture,-1);func_8008D870(slot[19].handle,texture,-1);
        if(slot->position[1]>D_803B25B8+D_803B25B0*D_803B25AC || slot->position[1]<D_803B25BC-D_803B25B0*D_803B25AC)slot->alpha=0;
        else if(slot->position[1]>D_803B25B8)slot->alpha=255.0f-(slot->position[1]-D_803B25B8)*255.0f/(D_803B25B0*D_803B25AC);
        else if(slot->position[1]<D_803B25BC)slot->alpha=255.0f-(D_803B25BC-slot->position[1])*255.0f/(D_803B25B0*D_803B25AC);
        else slot->alpha=255;
        if(!func_8039D494(item,player))slot->alpha=0;
        ((u8 *)&color)[3]=slot[19].alpha=slot->alpha;
        func_8008E06C(slot->handle,&color);func_8008E06C(slot[19].handle,&color);
        if(!func_8039D494(item,player)) {
            model_data_load(slot->handle,1,15);model_data_load(slot[19].handle,1,15);
        } else {
            if(!slot->alpha || D_803BA028[player]==1) {
                model_data_load(slot->handle,1,15);model_data_load(slot[19].handle,1,15);
            } else {
                mask=1<<player;
                model_transform_setup(slot->handle,0,mask);model_transform_setup(slot[19].handle,0,mask);
            }
            y-=D_803B25B0*D_803B25AC;
        }
    }
    if(D_803BA028[player]==1) {
        model_data_load(row[40].handle,1,15);model_data_load(row[41].handle,1,15);row[39].position[2]=2000.0f;
    } else {
        mask=1<<player;model_transform_setup(row[40].handle,0,mask);model_transform_setup(row[41].handle,0,mask);
    }
    if(D_803BA02C==1) {
        model_data_load(row[42].handle,1,15);model_data_load(row[43].handle,1,15);
    } else {
        mask=1<<player;model_transform_setup(row[42].handle,0,mask);model_transform_setup(row[43].handle,0,mask);
    }
    if(settled==1)D_803BA210[player]=0.0f;
    D_803B9BA0[player]=0;
}
