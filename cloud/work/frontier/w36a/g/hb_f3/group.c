/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w35a superseding revision: func_8039FF04 (GC initButtons@carscr.c) and func_8039D6A4 (GC updateButtons@carscr.c) are now
 * members, ported from reference/repos/mat3 carscr.c; see their headers below. Old members and their text are unchanged. */
/* Image-A selected car-stat product: D300, 404 newly strict-matched bytes.
 * The genuine cleanup/car-selection group retains all five accepted members.
 * D300 uses direct per-component indexed factors, letting stock IDO hoist the
 * complete row-address expressions while preserving the five selection reads.
 * Two do/while(0) statement regions deliberately establish CFE block boundaries:
 * the first pair assignment and third-factor update coalesce into one store;
 * the next region and final update retain three native stores per component.
 * This is disclosed source shaping under the live Wave 15 block-boundary rule,
 * not a claim that these original source macros or blocks have been recovered.
 * The initial pair's source operand order and compound third-factor update
 * reproduce the native floating operand order without reassociating its stages.
 * No volatile, unused local, capacity change, false signature, artificial caller,
 * assembly, forced register, or compiler modification is used.
 */
/* Overlay A cleanup (8039D05C), complete 676-byte body.
 * Source ancestry: PR #311, record-context group at d725fbe089ba81b36386b849f492035261af4883.
 * Authentic reconstructed callers/callees are preserved as nonclaiming context.
 * Arcade ancestry is unknown. This is a private-ABI no-argument helper that
 * clobbers s0-s8; its real caller func_803A1EAC saves/restores that set.
 * Shaping quirk: the code-free read of initialized player in the panel loop
 * adds one weight-100 use in IDO's allocation analysis (92/11 -> 192/11),
 * giving the retail register order. It has no memory access or side effects.
 * This is explicit source shaping, not a claim about original source spelling.
 */
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
typedef struct RecordedCar {u8 unknown00[8],car,unknown09[3];s8 component;u8 unknown0D[2],red,green;s8 blue,part,unknown13,stat0,stat1;} RecordedCar;
typedef struct Record Record;
struct Record {Record **next;FileInfo **file;s8 track;u8 mode,name[14];u32 id0,id1;f32 time;u32 unknown24;RecordedCar **active;};
extern Player D_8014A118[];
typedef struct RecordList {u8 indirect,doubly,unknown02[2];s32 count;Record **first, **last;} RecordList;
extern RecordList D_80152020;
extern Record **D_803BA010,**D_80152698[4];
extern s16 D_803BA01A[3];
extern s32 D_8014A110;
extern s8 D_801426EC,D_8014978C,D_80152570,D_80156994;
extern s32 func_8008AD04(u8 *,u8 *);
extern s32 menu_item_value_get(Record **);
extern void menu_transition(void **);
typedef struct Language {s32 unknown0;u8 **text;s32 unknown8;u16 *indices;u8 **indexed;} Language;
typedef struct Camera {f32 uv[3][3],position[3];u8 unknown30[104];} Camera;
typedef struct Slot {s32 kind,handle;f32 angle,matrix[3][3],position[3];u8 alpha,unknown3D[3];} Slot;
typedef struct CarSlot {f32 selection,angle;s8 position,car;u8 unknown0A[2];} CarSlot;
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
extern u8 D_80146180[4];
extern volatile u8 D_80140BDC;
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
/* Genuine low-image services, reconstructed below with natural source inputs. */
extern void func_8039FF04(s8);
extern void func_8039E3BC(s32);
typedef struct Vec3 {f32 v[3];} Vec3;
typedef struct PreparedCar {u8 unknown0,car;s8 color[3];u8 unknown5,flags,unknown7;} PreparedCar;
extern Record **D_8012E6F8;
extern u32 D_801174BC,D_801174B4;
extern s32 D_803BA030,D_803BA050[],D_803BA060[][19],D_803B9BA0[],D_803AF980;
extern s8 D_80157244,D_803BA04C[],D_8012E67C[],D_80138664[];
extern s16 D_801164BE,D_803B28B0[];
extern Slot D_803AF9A8[][44];
extern Vec3 D_803B3030;
extern PreparedCar D_80153E88[];
extern u8 D_803B857C[],D_803B8590[],D_803B85A0[];
extern s8 D_80110E78[][13],D_8011103C[][13],D_80111080[][13],D_8011119C[][13],D_80111230[][13],D_8011128C[][13];
extern s8 D_80111604[][13],D_80111648[][13],D_8011168C[][13],D_8011157C[][13],D_801115C0[][13];
extern s8 func_800A1A3C(s32);
extern s8 func_800A361C(s32);
extern void func_800B5570(s32);
extern void func_8039D05C(void);
extern void func_8039EF08(void);
extern void func_8039D6A4(s8);
extern Record **func_8039E200(Record **,s8);
extern s32 entity_flags_apply(u32,u32,u32,u8);
extern void audio_doppler(s32);
extern void resource_type_select(u32);
extern void scheduler_recv(s32);
extern void func_800CDCAC(Profile **,u8);
extern void sfx_stop(s16,s16,s32);
extern void speed_mode0_wrapper(f32,f32);
extern void net_session_update(void);
extern void pause_resume(s32);
extern void brake_light_update(s32,f32 *,Camera *,void *,s16 *);
extern void *object_create(s32);
extern s16 object_bytes_sum_global(void);
extern u32 object_manager_update(u8 *,s16);
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
    s16 component;

    for (component = 0; component < 4; component++) {
        do {
            D_803BA190[player][component] = D_803B2948[D_80111049[player][D_803B9FD0[player]]][component] * D_803B28C8[D_80110E85[player][D_803B9FD0[player]]][component];
            D_803BA190[player][component] *= D_803B2978[D_8011108D[player][D_803B9FD0[player]]][component];
        } while (0);
        do {
            D_803BA190[player][component] *= D_803B2A08[D_801111A9[player][D_803B9FD0[player]]][component];
        } while (0);
        D_803BA190[player][component] *= D_803B2A58[D_8011123D[player][D_803B9FD0[player]]][component];
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
        node=D_80152020.first;
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
            node=D_80152020.first;
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
            node=D_80152020.first;
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
        node=D_80152020.first;
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
/* Overlay A 0x803A0FD8, 3796 bytes: GC _InitScreen@carscr.c (MAT3 0x8008f904, 0xbc8 B), static.
 * Source: reference/repos/mat3 decomp/src/GameSource/src/carscr.c _InitScreen (typed port); the N64 build is an earlier
 * revision: 62/80/61/81 audio loads, ghost count by D_80156994, 12 per-car settings (8,9 bytes; 10,11 tire scales),
 * no cars[].alpha init, no SavePlayerCarTrans, 38 blits.  Private-ABI function of the IPA group (unsaved s0-s8/f20-f24).
 * Shaping (all verified on the builder, group dot_cleanup_colortrace_20261011):
 *  - the pane loop and the matrix loop use `p`/`i` as written (p out-ranks i for s4/s5);
 *  - D_80140BDC (gNumModels) is volatile u8 in the head;
 *  - `0.0` spelling for the pos[0] stores (zero constant gets $f0, 2.0 gets $f2);
 *  - `for (D_803BA020[p] = 0, i = 0; ...)` orders the store after the row-offset arithmetic;
 *  - `Player *pl` (assigned &D_8014A118[p], used for ->owner) is the third 4-byte local: it gives the retail 208-byte frame and puts
 *    p's home at 200(sp); it is real, not padding.
 *  - (w36a) `pl = &D_8014A118[p];` is also the first statement of the FIRST player loop, where it is dead and compiles to
 *    nothing.  It enters the &gPlayers[p] expression into uopt's expression table ahead of the later loops' webs, so
 *    f_spilltemps (homes in expression-bit order, first fit by block conflict) gives the strength-reduced pointer (s6) the
 *    first home, 188(sp), and pushes the gNumCarsActive pointer to 180(sp), as in retail.  Without it the pointer gets the
 *    fourth home (176) and gNumCarsActive the second (184): 7 words.  Disclosed shaping (a code-free dead assignment);
 *    the original probably assigned a player pointer at the top of each player loop;
 *  - (w36a) the final `for (i...)` loop body is on its own line: as1 then fills the unrolled loop's branch delay slot with
 *    the first byte store (displacement rewritten), not the `addiu` (4 words; source-line tie in the delay-slot filler).
 * Matches in this group (score.py group, strict).  Own .rodata verified by hand against decompressed image A (7/7 literal
 * sites, rocheck.py).
 */
void func_803A0FD8(void)
{
    Player *pl;
    s32 p;
    s32 i;

    func_803A0498();
    audio_frame_sync(62, 0, 0, 0, 0);
    if (D_80156994) {
        audio_frame_sync(80, 0, 0, 0, 0);
        audio_frame_sync(61, 0, 0, 0, 0);
    } else if (D_8014978C >= 6) {
        audio_frame_sync(80, 0, 0, 0, 0);
    } else {
        audio_frame_sync(81, 0, 0, 0, 0);
    }
    D_803BA02C = 0;
    for (p = 0; p < D_8014A108; p++) {
        pl = &D_8014A118[p];
        D_803B28B8[p] = 0;
        D_803BA048[p] = 1;
        D_803BA038[p] = 1;
        D_803BA028[p] = 0;
        D_803BA1D0[p] = -1;
        func_8039D300(p);
    }
    for (p = 0; p < D_8014A108; p++) {
        for (D_803BA020[p] = 0, i = 0; i < 13; i++) {
            if (i < 13) D_803B9FD8[p][D_803BA020[p]++] = D_803B3010[i];
        }
    }
    for (i = 0; i < 48; i++) D_803B9AA0[i] = func_800A7D6C();
    init_state_begin();
    D_80146180[0] = 0;
    D_80146180[1] = 0;
    D_80146180[2] = 0;
    D_80146180[3] = 0;
    D_80154188 = 90.0f;
    func_800A5A40();
    D_80151AD0 = D_8014A108;
    if (D_80151AD0 == 3) D_80151AD0 = 4;
    if (D_8014A110 == 2) {
        D_803BA00C = 3;
        D_80151AD0 = 4;
        if (!D_80156994) {
            D_803BA00C = 1;
            D_80151AD0 = 2;
        }
    }
    car_render_full(D_80151AD0);
    if (D_80151AD0 == 1) {
        D_803B25A8 = 1.0f;
        D_803B25AC = 0.045f;
        D_803B25B0 = 40.0f;
        D_803B25B4 = 0.0f;
        D_803B25B8 = -5.4f;
        D_803B25BC = -8.4f;
        D_803B25C0 = 15.0f;
        D_803B25C4 = 11.25f;
    } else {
        D_803B25A8 = 1.0f;
        D_803B25AC = 0.055f;
        D_803B25B0 = 40.0f;
        D_803B25B4 = 0.0f;
        D_803B25B8 = -3.45f;
        D_803B25BC = -7.7f;
        D_803B25C0 = 15.0f;
        D_803B25C4 = 11.25f;
    }
    audio_interrupt_handler(1);
    sfx_position_3d(1);
    particle_lifetime_set();
    for (p = 0; p < 4; p++) D_803B9FC0[p] = ambient_sound_set(-20, 0, -10, 10, 192, 0, 0, 0);
    for (p = 0; p < D_8014A108; p++) {
        pl = &D_8014A118[p];
        math_utility(D_8011418C, D_803B9E40[p][0]);
        math_utility(D_8011418C, D_803B9E40[p][1]);
        D_803B25CC[p][0] = sign_extend_call(string_copy_format(D_803B85B0, 0, D_80140BDC - 1, 1), D_803B9E40[p][0], -1, 0x40000);
        D_803B25CC[p][1] = sign_extend_call(string_copy_format(D_803B85BC, 0, D_80140BDC - 1, 1), D_803B9E40[p][1], -1, 0);
        model_data_load(D_803B25CC[p][0], 1, 15);
        model_data_load(D_803B25CC[p][1], 1, 15);
        if (D_8014A110 == 2) {
            model_transform_setup(D_803B25CC[p][0], 0, 15);
            model_transform_setup(D_803B25CC[p][1], 0, 15);
        } else {
            model_transform_setup(D_803B25CC[p][0], 0, 1 << p);
            model_transform_setup(D_803B25CC[p][1], 0, 1 << p);
        }
        D_803B9E40[p][0][9] = 0.0;
        D_803B9E40[p][0][10] = 2.0f;
        D_803B9E40[p][0][11] = 15.0f;
        D_803B9E40[p][1][9] = 0.0;
        D_803B9E40[p][1][10] = 2.0f;
        D_803B9E40[p][1][11] = 15.0f;
        func_8039FF04(p);
        for (i = 0; i < 13; i++) {
            D_8011103C[p + 1][i] = func_800CDA60(pl->owner, i, 0);
            D_80111080[p + 1][i] = func_800CDA60(pl->owner, i, 1);
            while (!func_800F7604(p, D_80111080[p + 1][i])) {
                D_80111080[p + 1][i]--;
                if (D_80111080[p + 1][i] < 0) { D_80111080[p + 1][i] = 0; break; }
            }
            func_800C7578(pl->owner, i, 1, D_80111080[p + 1][i]);
            D_8011119C[p + 1][i] = func_800CDA60(pl->owner, i, 2);
            while (!func_800F75D0(p, D_8011119C[p + 1][i])) {
                D_8011119C[p + 1][i]--;
                if (D_8011119C[p + 1][i] < 0) { D_8011119C[p + 1][i] = 0; break; }
            }
            func_800C7578(pl->owner, i, 2, D_8011119C[p + 1][i]);
            D_80111230[p + 1][i] = func_800CDA60(pl->owner, i, 3);
            while (!func_800F75B0(p, D_80111230[p + 1][i])) {
                D_80111230[p + 1][i]--;
                if (D_80111230[p + 1][i] < 0) { D_80111230[p + 1][i] = 0; break; }
            }
            func_800C7578(pl->owner, i, 3, D_80111230[p + 1][i]);
            D_8011128C[p + 1][i] = func_800CDA60(pl->owner, i, 4);
            if (D_8014A110 != 4 && D_8011128C[p + 1][i] > 1) D_8011128C[p + 1][i] = 1;
            D_80111604[p + 1][i] = func_800CDA60(pl->owner, i, 5);
            D_80111648[p + 1][i] = func_800CDA60(pl->owner, i, 6);
            D_8011168C[p + 1][i] = func_800CDA60(pl->owner, i, 7);
            D_8011157C[p + 1][i] = func_800CDA60(pl->owner, i, 8);
            D_801115C0[p + 1][i] = func_800CDA60(pl->owner, i, 9);
            D_801112DC[p + 1][i] = func_800CDA60(pl->owner, i, 10) / 100.0f + 0.75f;
            D_801113E0[p + 1][i] = func_800CDA60(pl->owner, i, 11) / 100.0f + 0.75f;
            if (!D_801407D0) {
                D_801112DC[p + 1][i] = D_801112DC[0][i];
                D_801113E0[p + 1][i] = D_801113E0[0][i];
            }
        }
        D_803B9FD0[p] = func_800CDE88(pl->owner);
        while (!func_800F7620(p, D_803B9FD0[p])) {
            if (!D_803B9FD0[p]) D_803B9FD0[p] = 12;
            else D_803B9FD0[p]--;
        }
        for (D_803B9FD4[p] = 0; D_803B9FD4[p] < D_803BA020[p]; D_803B9FD4[p]++) {
            if (D_803B9FD0[p] == D_803B9FD8[p][D_803B9FD4[p]]) break;
        }
        if (D_803B9FD4[p] >= D_803BA020[p]) {
            D_803B9FD4[p] = p + 1;
            D_803B9FD0[p] = D_803B9FD8[p][D_803B9FD4[p]];
            func_800CCA04(pl->owner, D_803B9FD0[p]);
        }
        D_803BA018[p] = -1;
        func_8039E3BC(p);
        for (i = 0; i < D_803BA020[p]; i++) {
            D_803B9BC8[p][i].position = i - D_803B9FD4[p];
            if (D_803B9BC8[p][i].position < 0) D_803B9BC8[p][i].position += D_803BA020[p];
            D_803B9BC8[p][i].car = D_803B9FD8[p][i] + p * 13;
            D_803B9BC8[p][i].angle = 3.1415927f;
        }
        if (D_8014A110 == 6) D_8014A118[p].paint = 6;
        else {
            D_8014A118[p].paint = func_800CDDE8((void **)pl->owner);
            while (!func_800F75EC(p, D_8014A118[p].paint)) {
                D_8014A118[p].paint -= 2;
                if ((s32)D_8014A118[p].paint < 0) { D_8014A118[p].paint += 2; break; }
            }
        }
        for (i = 0; i < 13; i++)
            D_80110E78[p + 1][i] = D_8014A118[p].paint;
    }
    D_803B28AC = sound_control(0, 0, D_803B2AB8, 38);
    particle_velocity_set();
    effects_update_emitters();
    set_race_state();
    D_803B28A8 = 1;
}
void func_803A0498(void);
void func_803A0FD8(void);
void func_8039D300(s32);
void func_803A1EAC(void)
{
    s16 i;
    s16 p;
    s32 sound = 1;
    Record **ghosth;
    Record *ghost;
    Record **nextGhost;

    if (!D_80156994) D_803BA010 = D_8012E6F8;
    else D_803BA010 = D_8012E6F8 + ((D_80152570 ? 6 : 0) + D_8014978C) * 3;
    if (D_801174BC != 1) {
        if (D_801174BC != D_801174B4) { func_803A0498(); func_800B5570(0x40000); return; }
        D_801174BC = 1;
    }
    for (i = 1; i < 4; i++) {
        for (ghosth = D_80152020.first; ghosth; ghosth = nextGhost) {
            ghost = *ghosth;
            nextGhost = ghost->next;
            if ((!ghost->file || func_800A1A3C((*ghost->file)->controller)) && ghosth == D_80152698[i]) goto found;
        }
found:
        if (!ghosth) {
            for (p = 0; p < 3; p++) {
                ghosth = D_803BA010 + p;
                if (ghosth == D_80152698[i]) break;
                ghosth = 0;
            }
        }
        D_80152698[i] = ghosth;
    }
    if (!D_803B28A8) func_803A0FD8();
    func_8039EF08();
    for (i = 0; i < D_8014A108; i++) {
        if ((*D_8014A118[i].owner)->file && func_800A361C((*(*D_8014A118[i].owner)->file)->controller)) {
            func_8039D05C(); func_800B5570(32); D_803BA030 = 0; return;
        }
    }
    if (D_80157244) { func_8039D05C(); func_800B5570(4); D_803BA030 = 0; return; }
    D_803BA030++;
    for (p = 0; p < D_8014A108; p++) {
        s32 notReadyCount;
        s32 notReadyIdx;
        s32 pIdx;
        s16 x, y, w, h;
        s16 screenPos[2];
        Vec3 msgPos;
        s32 ghostIdx;
        s16 gx, gy, gw, gh;
        f32 pos[3];
        s16 gscreenPos[2];
        if (!D_803BA02C && (D_8014A118[p].pressed & 4)) {
            if (D_803BA028[p] == 1) D_803BA028[p] = 0;
            else { entity_flags_apply(38, 0, 1, 0); func_8039D05C(); func_800B5570(128); D_803BA030 = 0; return; }
        }
        if (D_803BA028[p]) {
            msgPos = D_803B3030;
            if (D_80151AD0 == 1) object_create(13); else object_create(12);
            brake_light_update(p, msgPos.v, &D_80150B70[p], 0, screenPos);
            x = screenPos[0];
            y = screenPos[1] - object_bytes_sum_global();
            notReadyCount = 0;
            for (pIdx = 0; pIdx < D_8014A108; pIdx++) {
                if (pIdx != p && !D_803BA028[pIdx]) { notReadyIdx = pIdx; notReadyCount++; }
            }
            if (!notReadyCount) {
                y += object_bytes_sum_global();
                h = object_bytes_sum_global();
                w = object_manager_update(D_803B857C, -1);
            } else {
                w = object_manager_update(countdown_state.text[151], -1);
                h = object_bytes_sum_global() * 2;
                if (notReadyCount == 1) w = (w > object_manager_update((*D_8014A118[notReadyIdx].owner)->name, -1) ? w : object_manager_update((*D_8014A118[notReadyIdx].owner)->name, -1));
                else w = (w > object_manager_update(countdown_state.text[31], -1) ? w : object_manager_update(countdown_state.text[31], -1));
            }
            y -= object_bytes_sum_global() / 2;
            crowd_cheer_play(D_803B9FC0[p], x - w / 2 - 4, y - 4, x + w / 2 + 4, y + h + 4);
            func_8039D6A4(p);
            continue;
        }
        crowd_cheer_play(D_803B9FC0[p], -20, 0, -10, 10);
        if (D_8014A118[p].pressed & 3) {
            resource_type_select(D_8014A118[p].pressed);
            D_803BA028[p] = 1;
            D_80153E88[p].flags = 160;
            D_80153E88[p].car = D_803B9FD0[p];
            if (D_8014A110 == 6 && D_801164BE == 0) {
                D_80153E88[p].color[0] = D_80138664[D_8012E67C[p]];
                D_80153E88[p].color[1] = D_80138664[D_8012E67C[p]];
                D_80153E88[p].color[2] = D_80138664[D_8012E67C[p]];
            } else {
                D_80153E88[p].color[0] = D_80111604[p + 1][D_803B9FD0[p]];
                D_80153E88[p].color[1] = D_80111648[p + 1][D_803B9FD0[p]];
                D_80153E88[p].color[2] = D_8011168C[p + 1][D_803B9FD0[p]];
            }
            D_803BA030 = 0;
        }
        if (D_8014A118[p].repeated & 0x400) {
            entity_flags_apply(40, 0, 1, 0);
            D_803B28B8[p]--;
            if (D_803B28B8[p] < 0) D_803B28B8[p] = D_803BA050[p] - 1;
        } else if (D_8014A118[p].repeated & 0x800) {
            entity_flags_apply(39, 0, 1, 0);
            D_803B28B8[p]++;
            if (D_803B28B8[p] > D_803BA050[p] - 1) D_803B28B8[p] = 0;
        } else if (D_8014A118[p].repeated & 0x3080) {
            s32 d = (D_8014A118[p].repeated & 0x1000) ? -1 : ((D_8014A118[p].repeated & 0x2000) ? 1 : 0);
            if (!D_803BA048[p] || (!d && !D_803BA04C[p])) {
                entity_flags_apply(42, 0, 1, 0);
                goto after_options;
            }
            switch (D_803BA060[p][D_803B28B8[p]]) {
            case 0:
                if (d) {
                    do {
                        D_803B9FD4[p] += d;
                        if (D_803B9FD4[p] < 0) D_803B9FD4[p] = D_803BA020[p] - 1;
                        else if (D_803B9FD4[p] >= D_803BA020[p]) D_803B9FD4[p] = 0;
                    } while (!func_800F7620(p, D_803B9FD8[p][D_803B9FD4[p]]));
                }
                D_803B9FD0[p] = D_803B9FD8[p][D_803B9FD4[p]];
                if (D_8014A110 == 6) D_8014A118[p].paint = 6;
                else D_8014A118[p].paint = func_800CDDE8((void **)D_8014A118[p].owner);
                for (i = 0; i < 13; i++) D_80110E78[p + 1][i] = D_8014A118[p].paint;
                func_8039E3BC(p);
                D_803B9BA0[p] = 1;
                break;
            case 1:
                if (D_8014A110 != 6) {
                    if (d) {
                        do {
                            D_80110E78[p + 1][D_803B9FD0[p]] += d;
                            if (D_80110E78[p + 1][D_803B9FD0[p]] < 0) D_80110E78[p + 1][D_803B9FD0[p]] = 7;
                            else if (D_80110E78[p + 1][D_803B9FD0[p]] >= 8) D_80110E78[p + 1][D_803B9FD0[p]] = 0;
                        } while (!func_800F75EC(p, D_80110E78[p + 1][D_803B9FD0[p]]) || (D_8014A110 == 6 && (D_80110E78[p + 1][D_803B9FD0[p]] & 1)));
                    } else D_80110E78[p + 1][D_803B9FD0[p]] = D_80110E78[0][D_803B9FD0[p]];
                    D_8014A118[p].paint = D_80110E78[p + 1][D_803B9FD0[p]];
                    func_800CDCAC(D_8014A118[p].owner, D_8014A118[p].paint);
                } else sound = 0;
                break;
            case 2:
                if (d) {
                    D_8011103C[p + 1][D_803B9FD0[p]] += d;
                    if (D_8011103C[p + 1][D_803B9FD0[p]] < 0) D_8011103C[p + 1][D_803B9FD0[p]] = 2;
                    else if (D_8011103C[p + 1][D_803B9FD0[p]] > 2) D_8011103C[p + 1][D_803B9FD0[p]] = 0;
                } else D_8011103C[p + 1][D_803B9FD0[p]] = D_8011103C[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 0, D_8011103C[p + 1][D_803B9FD0[p]]);
                break;
            case 3:
                if (d) {
                    do {
                        D_80111080[p + 1][D_803B9FD0[p]] += d;
                        if (D_80111080[p + 1][D_803B9FD0[p]] < 0) D_80111080[p + 1][D_803B9FD0[p]] = 8;
                        else if (D_80111080[p + 1][D_803B9FD0[p]] > 8) D_80111080[p + 1][D_803B9FD0[p]] = 0;
                    } while (!func_800F7604(p, D_80111080[p + 1][D_803B9FD0[p]]));
                } else D_80111080[p + 1][D_803B9FD0[p]] = D_80111080[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 1, D_80111080[p + 1][D_803B9FD0[p]]);
                break;
            case 4:
                if (d) {
                    do {
                        D_8011119C[p + 1][D_803B9FD0[p]] += d;
                        if (D_8011119C[p + 1][D_803B9FD0[p]] < 0) D_8011119C[p + 1][D_803B9FD0[p]] = 4;
                        else if (D_8011119C[p + 1][D_803B9FD0[p]] > 4) D_8011119C[p + 1][D_803B9FD0[p]] = 0;
                    } while (!func_800F75D0(p, D_8011119C[p + 1][D_803B9FD0[p]]));
                } else D_8011119C[p + 1][D_803B9FD0[p]] = D_8011119C[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 2, D_8011119C[p + 1][D_803B9FD0[p]]);
                break;
            case 5:
                if (d) {
                    do {
                        D_80111230[p + 1][D_803B9FD0[p]] += d;
                        if (D_80111230[p + 1][D_803B9FD0[p]] < 0) D_80111230[p + 1][D_803B9FD0[p]] = 5;
                        else if (D_80111230[p + 1][D_803B9FD0[p]] > 5) D_80111230[p + 1][D_803B9FD0[p]] = 0;
                    } while (!func_800F75B0(p, D_80111230[p + 1][D_803B9FD0[p]]));
                } else D_80111230[p + 1][D_803B9FD0[p]] = D_80111230[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 3, D_80111230[p + 1][D_803B9FD0[p]]);
                break;
            case 6:
                if (d) {
                    D_8011128C[p + 1][D_803B9FD0[p]] += d;
                    if (D_8011128C[p + 1][D_803B9FD0[p]] < 0) D_8011128C[p + 1][D_803B9FD0[p]] = (D_8014A110 != 4 ? 1 : 2);
                    else if (D_8011128C[p + 1][D_803B9FD0[p]] > (D_8014A110 != 4 ? 1 : 2)) D_8011128C[p + 1][D_803B9FD0[p]] = 0;
                } else D_8011128C[p + 1][D_803B9FD0[p]] = D_8011128C[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 4, D_8011128C[p + 1][D_803B9FD0[p]]);
                break;
            case 7:
                if (d) {
                    D_80111604[p + 1][D_803B9FD0[p]] += d;
                    if (D_80111604[p + 1][D_803B9FD0[p]] < 0) D_80111604[p + 1][D_803B9FD0[p]] = 31;
                    else if (D_80111604[p + 1][D_803B9FD0[p]] >= 32) D_80111604[p + 1][D_803B9FD0[p]] = 0;
                } else D_80111604[p + 1][D_803B9FD0[p]] = D_80111604[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 5, D_80111604[p + 1][D_803B9FD0[p]]);
                break;
            case 8:
                if (d) {
                    D_80111648[p + 1][D_803B9FD0[p]] += d;
                    if (D_80111648[p + 1][D_803B9FD0[p]] < 0) D_80111648[p + 1][D_803B9FD0[p]] = 31;
                    else if (D_80111648[p + 1][D_803B9FD0[p]] >= 32) D_80111648[p + 1][D_803B9FD0[p]] = 0;
                } else D_80111648[p + 1][D_803B9FD0[p]] = D_80111648[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 6, D_80111648[p + 1][D_803B9FD0[p]]);
                break;
            case 9:
                if (d) {
                    D_8011168C[p + 1][D_803B9FD0[p]] += d;
                    if (D_8011168C[p + 1][D_803B9FD0[p]] < 0) D_8011168C[p + 1][D_803B9FD0[p]] = 31;
                    else if (D_8011168C[p + 1][D_803B9FD0[p]] >= 32) D_8011168C[p + 1][D_803B9FD0[p]] = 0;
                } else D_8011168C[p + 1][D_803B9FD0[p]] = D_8011168C[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 7, D_8011168C[p + 1][D_803B9FD0[p]]);
                break;
            case 10:
                if (d) {
                    s32 teamFull = 1;
                    do {
                        D_8012E67C[p] += d;
                        if (D_8012E67C[p] < 0) D_8012E67C[p] = 3;
                        else if (D_8012E67C[p] > 3) D_8012E67C[p] = 0;
                        for (i = 0; i < D_8014A108; i++) {
                            if (i != p && D_8012E67C[i] != D_8012E67C[p]) teamFull = 0;
                        }
                    } while (teamFull == 1);
                } else D_8012E67C[p] = p;
                break;
            case 11:
                if (d) {
                    D_8011157C[p + 1][D_803B9FD0[p]] += d;
                    if (D_8011157C[p + 1][D_803B9FD0[p]] < 0) D_8011157C[p + 1][D_803B9FD0[p]] = 20;
                    else if (D_8011157C[p + 1][D_803B9FD0[p]] > 20) D_8011157C[p + 1][D_803B9FD0[p]] = 0;
                } else D_8011157C[p + 1][D_803B9FD0[p]] = D_8011157C[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 8, D_8011157C[p + 1][D_803B9FD0[p]]);
                break;
            case 12:
                if (d) {
                    D_801115C0[p + 1][D_803B9FD0[p]] += d;
                    if (D_801115C0[p + 1][D_803B9FD0[p]] < 0) D_801115C0[p + 1][D_803B9FD0[p]] = 9;
                    else if (D_801115C0[p + 1][D_803B9FD0[p]] > 9) D_801115C0[p + 1][D_803B9FD0[p]] = 0;
                } else D_801115C0[p + 1][D_803B9FD0[p]] = D_801115C0[0][D_803B9FD0[p]];
                sound = 0;
                scheduler_recv(D_803BA1D0[p]);
                D_803BA1D0[p] = entity_flags_apply(D_801115C0[p + 1][D_803B9FD0[p]] + 26, 0, 1, 0);
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 9, D_801115C0[p + 1][D_803B9FD0[p]]);
                break;
            case 13:
                if (d) {
                    D_801112DC[p + 1][D_803B9FD0[p]] += d * 0.05f;
                    if (D_801112DC[p + 1][D_803B9FD0[p]] < 0.74f) D_801112DC[p + 1][D_803B9FD0[p]] = 1.5f;
                    else if (D_801112DC[p + 1][D_803B9FD0[p]] > 1.6f) D_801112DC[p + 1][D_803B9FD0[p]] = 0.75f;
                } else D_801112DC[p + 1][D_803B9FD0[p]] = D_801112DC[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 10, (s8)(100.0f * D_801112DC[p + 1][D_803B9FD0[p]] - 75.0f));
                break;
            case 14:
                if (d) {
                    D_801113E0[p + 1][D_803B9FD0[p]] += d * 0.05f;
                    if (D_801113E0[p + 1][D_803B9FD0[p]] < 0.74f) D_801113E0[p + 1][D_803B9FD0[p]] = 1.5f;
                    else if (D_801113E0[p + 1][D_803B9FD0[p]] > 1.5f) D_801113E0[p + 1][D_803B9FD0[p]] = 0.75f;
                } else D_801113E0[p + 1][D_803B9FD0[p]] = D_801113E0[0][D_803B9FD0[p]];
                func_800C7578(D_8014A118[p].owner, D_803B9FD0[p], 11, (s8)(100.0f * D_801113E0[p + 1][D_803B9FD0[p]] - 75.0f));
                break;
            case 15:
                if (d) {
                    D_803B28B0[p] += d;
                    if (D_803B28B0[p] < 0) D_803B28B0[p] = 10;
                    else if (D_803B28B0[p] > 10) D_803B28B0[p] = 0;
                } else D_803B28B0[p] = 0;
                sfx_stop(D_803B9FD0[p], p, D_803B28B0[p]);
                break;
            case 16: D_80152698[1] = func_8039E200(D_80152698[1], d); break;
            case 17: D_80152698[2] = func_8039E200(D_80152698[2], d); break;
            case 18: D_80152698[3] = func_8039E200(D_80152698[3], d); break;
            }
            if (sound) audio_doppler(D_8014A118[p].repeated);
        }
        if (D_803BA038[p] > D_803B28B8[p] - 1) D_803BA038[p] = D_803B28B8[p] - 1;
        if (D_803BA038[p] < D_803B28B8[p] - 1) D_803BA038[p] = D_803B28B8[p] - 1;
        if (D_803BA038[p] > D_803BA050[p] - 3) D_803BA038[p] = D_803BA050[p] - 3;
        if (D_803BA038[p] < 1) D_803BA038[p] = 1;
        D_803BA048[p] = 1;
after_options:
        if (D_803BA018[p] != D_803B9FD0[p]) {
            if (D_803BA018[p] >= 0) func_800CCA04(D_8014A118[p].owner, D_803B9FD0[p]);
            D_803BA018[p] = D_803B9FD0[p];
        }
        if (D_8014A110 == 2) {
            for (ghostIdx = 1; ghostIdx < D_803BA00C + 1; ghostIdx++) {
                if (D_803B9FC0[ghostIdx]) {
                    pos[0] = D_803AF9A8[0][38].position[0];
                    pos[1] = D_803AF9A8[0][38].position[1];
                    pos[2] = D_803AF9A8[0][38].position[2];
                    pos[1] = -pos[1] / 2.0f;
                    brake_light_update(ghostIdx, pos, &D_80150B70[ghostIdx], 0, gscreenPos);
                    gx = gscreenPos[0];
                    gy = gscreenPos[1];
                    object_create(12);
                    if (!D_80152698[ghostIdx]) {
                        if (!D_803AF980) gw = object_manager_update(countdown_state.text[225], -1);
                        else gw = object_manager_update(countdown_state.text[224], -1);
                        gh = object_bytes_sum_global();
                    } else {
                        gw = object_manager_update((*D_80152698[ghostIdx])->name, -1);
                        gw = (gw > object_manager_update(D_803B8590, -1) ? gw : object_manager_update(D_803B85A0, -1));
                        gh = object_bytes_sum_global() * 2;
                    }
                    gy -= object_bytes_sum_global() / 2;
                    crowd_cheer_play(D_803B9FC0[ghostIdx], gx - gw / 2 - 4, gy - 4, gx + gw / 2 + 4, gy + gh + 4);
                }
            }
        }
        func_8039D6A4(p);
        func_8039D300(p);
    }
    D_803BA02C = 1;
    for (i = 0; i < D_8014A108; i++) if (!D_803BA028[i]) D_803BA02C = 0;
    if (D_803BA02C && D_803BA030 >= 10) {
        speed_mode0_wrapper(0.0f, 3.0f);
        func_800B5570(0x40000);
        func_8039D05C();
        if (D_8014A110 == 3) net_session_update();
        if (D_8014A110 == 2) {
            {
                Record **gh;
                for (gh = D_80152020.first; gh; gh = (*gh)->next) menu_transition((void **)gh);
            }
            for (i = 1; i < D_803BA00C; i++) {
                if (!D_80152698[i]) { D_80152698[i] = D_80152698[i + 1]; D_80152698[i + 1] = 0; }
            }
            for (i = 1; i < D_803BA00C; i++) {
                if (!D_80152698[i]) { D_80152698[i] = D_80152698[i + 1]; D_80152698[i + 1] = 0; }
            }
        }
        pause_resume(0);
    }
}

extern s8 D_803B3020[];
s32 func_8039D494(s32 player, s32 item)
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
    D_803BA050[player]=0; for(item=0;item<19;item++) {
        if(func_8039D494(player,item))D_803BA060[player][D_803BA050[player]++]=item;
    }
}

Record **func_8039E200(Record **node,s8 direction)
{
    Record **next;
    Record *record;
    if(!node) {
        if(direction>0)node=D_80152020.first;
        else node=D_803BA010+2;
    }
    while(node) {
        record=*node;
        if(direction>0)next=record->next;
        else if(node==D_80152020.first)next=0;
        else {
            for(next=D_80152020.first;next && (*next)->next!=node;next=(*next)->next) {}
        }
        if(record->track==D_8014978C && record->mode==D_80152570 &&
           node!=D_80152698[1] && node!=D_80152698[2] && node!=D_80152698[3] &&
           (!(*node)->file || menu_item_value_get(node)) && (*node)->unknown24)break;
        if(!next) {
            if(node==D_803BA010) {
                if(direction>0)next=D_803BA010+1;
                else next=D_80152020.last;
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
    s32 player,i,count;
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
    for(player=0;player<(count=D_8014A108);player++) {
        for(i=0;i!=44;i++) {
            slot=&D_803AF9A8[(s8)player][i];
            if(slot->handle!=-1) {sound_call_minimal(slot->handle);slot->handle=-1;}
        }
        D_803B9B90[(s8)player]=0;
        if(D_803B25CC[player][0]!=-1) {sound_call_minimal(D_803B25CC[player][0]);D_803B25CC[player][0]=-1;}
        if(D_803B25CC[player][1]!=-1) {sound_call_minimal(D_803B25CC[player][1]);D_803B25CC[player][1]=-1;}
        if(player>0)continue;
        {
            for(i=0;i!=11;i++) {
                if (player) {} /* Code-free allocation-priority read; see header. */
                part=&D_803B2618[player][i];
                if(part->handle!=-1) {sound_call_minimal(part->handle);part->handle=-1;part->auxiliary=-1;}
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

typedef union Color4 {u8 c[4];u32 word;} Color4;
extern char *D_803AF984[];
extern char D_803B8630[],D_803B8640[],D_803B8648[],D_803B864C[];
extern void *memcpy(void *,const void *,u32);
extern void func_8008E06C(s16,Color4 *);
extern void *func_800B24EC(char *,u16 *,s32,s8,s32);
extern void func_8008D870(s16,void *,s16);
extern void func_800B5898(f32,void *);
extern void func_800B5940(f32,void *);
extern void func_8008B32C(void *,void *,f32);
/* func_8039FF04 = GC initButtons@carscr.c (MAT3 0x80093b10, 0x564 B): creates the button models of one player's
 * car-select menu and lays out the cursor/A/B arrow records. Direct typed port of the MAT3 body
 * (reference/repos/mat3/decomp/src/GameSource/src/carscr.c) with N64 offsets from the displacements:
 *   - D_803AF9A8 = buttonInfo[4][44] (64-byte records: kind +0, handle +4, angle +8, 3x3 matrix +0xC, position +0x30,
 *     alpha +0x3C). MAT3 has 40 records (17+17+6); the N64 has 44 (19+19+6), so GC records 34..39 are 38..43 here.
 *   - GC `4.0f +` offsets on records 36..39's y are absent on the N64; strings are the N64 button/cursor names.
 *   - zoom is GC's function-local `static float zoom = 1.0f` (retail 0x803B3060, next to the two colour initialisers
 *     0x803B305C / 0x803B3064); buttonColor = {255,255,255,255}.
 * Private-ABI member of the dot_cleanup_colortrace group: the s8 parameter arrives in the caller's stack home
 * (func_803A0FD8 stores it), the body clobbers s0-s8/f20-f30 without saving, so it only matches inside the group.
 * Levers (both are real source facts; no compiled-out device, no unused local):
 *   - D_80140BDC (gNumModels) is `volatile u8` in the head: every retail use is `lui; addiu; lbu 0(reg)`.
 *   - the early `zoom = 1.0` store is a double literal while `1.0f / zoom` stays float: the two spellings are
 *     different uopt constants, so the store keeps its own $f4 and the divide's 1.0f is not hoisted to $f30.
 * Own data (zoom, the colour initialiser, the float literals) is not verifiable for image A by score.py (no retail
 * data tree for ovl_a): code identical, own .data/.rodata unverified.
 */
void func_8039FF04(s8 p)
{
    Color4 buttonColor={{255,255,255,255}};
    void *tex;
    u16 texIdx;
    s32 buttonIdx;
    static f32 zoom=1.0f;
    if(D_80151AD0==1)zoom=1.0;
    for(buttonIdx=0;buttonIdx<44;buttonIdx++) {
        if(D_803AF9A8[p][buttonIdx].handle==-1) {
            memcpy(D_803AF9A8[p][buttonIdx].matrix,D_8011418C,36);
            D_803AF9A8[p][buttonIdx].handle=sign_extend_call(
                string_copy_format(D_803AF984[D_803AF9A8[p][buttonIdx].kind],0,D_80140BDC-1,1),
                D_803AF9A8[p][buttonIdx].matrix,-1,
                (D_803AF9A8[p][buttonIdx].kind==0 || D_803AF9A8[p][buttonIdx].kind==1 || D_803AF9A8[p][buttonIdx].kind==2) ? 0x42000 : 0);
            func_8008E06C((s16)D_803AF9A8[p][buttonIdx].handle,&buttonColor);
            model_data_load(D_803AF9A8[p][buttonIdx].handle,1,15);
            model_transform_setup(D_803AF9A8[p][buttonIdx].handle,0,1<<p);
        }
    }
    tex=func_800B24EC(D_803B8630,&texIdx,0,D_80140BDC-1,1);
    func_8008D870((s16)D_803AF9A8[p][38].handle,tex,-1);
    memcpy(D_803AF9A8[p][38].matrix,D_8011418C,36);
    func_800B5898(-1.5707964f,D_803AF9A8[p][38].matrix);
    D_803AF9A8[p][38].position[0]=0.0f;
    D_803AF9A8[p][38].position[1]=D_80151AD0>1 ? 105.0f : 115.0f;
    D_803AF9A8[p][38].position[2]=200.0f;
    D_803AF9A8[p][38].position[0]=(1.0f/zoom)*D_803AF9A8[p][38].position[0];
    D_803AF9A8[p][38].position[1]=(1.0f/zoom)*D_803AF9A8[p][38].position[1];
    D_803AF9A8[p][38].position[2]=(1.0f/zoom)*D_803AF9A8[p][38].position[2];
    if(D_8014A110==2)model_transform_setup(D_803AF9A8[p][38].handle,0,15);
    tex=func_800B24EC(D_803B8640,&texIdx,0,D_80140BDC-1,1);
    func_8008D870((s16)D_803AF9A8[p][39].handle,tex,-1);
    tex=func_800B24EC(D_803B8648,&texIdx,0,D_80140BDC-1,1);
    func_8008D870((s16)D_803AF9A8[p][40].handle,tex,-1);
    memcpy(D_803AF9A8[p][40].matrix,D_8011418C,36);
    func_800B5898(-1.5707964f,D_803AF9A8[p][40].matrix);
    D_803AF9A8[p][40].position[0]=75.1f;
    D_803AF9A8[p][40].position[1]=D_80151AD0>1 ? 50.2f : 55.2f;
    D_803AF9A8[p][40].position[2]=100.0f;
    func_8008B32C(D_803AF9A8[p][40].matrix,D_803AF9A8[p][40].matrix,0.5f);
    memcpy(D_803AF9A8[p][41].matrix,D_8011418C,36);
    func_800B5898(-1.5707964f,D_803AF9A8[p][41].matrix);
    func_800B5940(3.1415927f,D_803AF9A8[p][41].matrix);
    D_803AF9A8[p][41].position[0]=87.1f;
    D_803AF9A8[p][41].position[1]=D_80151AD0>1 ? 50.2f : 55.2f;
    D_803AF9A8[p][41].position[2]=100.0f;
    tex=func_800B24EC(D_803B864C,&texIdx,0,D_80140BDC-1,1);
    func_8008D870((s16)D_803AF9A8[p][42].handle,tex,-1);
    memcpy(D_803AF9A8[p][42].matrix,D_8011418C,36);
    func_800B5898(-1.5707964f,D_803AF9A8[p][42].matrix);
    D_803AF9A8[p][42].position[0]=-75.1f;
    D_803AF9A8[p][42].position[1]=D_80151AD0>1 ? 50.2f : 55.2f;
    D_803AF9A8[p][42].position[2]=100.0f;
    D_803AF9A8[p][42].position[0]=(1.0f/zoom)*D_803AF9A8[p][42].position[0];
    D_803AF9A8[p][42].position[1]=(1.0f/zoom)*D_803AF9A8[p][42].position[1];
    D_803AF9A8[p][42].position[2]=(1.0f/zoom)*D_803AF9A8[p][42].position[2];
    func_8008B32C(D_803AF9A8[p][42].matrix,D_803AF9A8[p][42].matrix,0.5f);
    memcpy(D_803AF9A8[p][43].matrix,D_8011418C,36);
    func_800B5898(-1.5707964f,D_803AF9A8[p][43].matrix);
    D_803AF9A8[p][43].position[0]=-87.1f;
    D_803AF9A8[p][43].position[1]=D_80151AD0>1 ? 50.2f : 55.2f;
    D_803AF9A8[p][43].position[2]=100.0f;
    D_803AF9A8[p][43].position[0]=(1.0f/zoom)*D_803AF9A8[p][43].position[0];
    D_803AF9A8[p][43].position[1]=(1.0f/zoom)*D_803AF9A8[p][43].position[1];
    D_803AF9A8[p][43].position[2]=(1.0f/zoom)*D_803AF9A8[p][43].position[2];
    D_803B9B90[p]=1;
    D_803B9BA0[p]=1;
    particle_velocity_set();
}


extern f32 fabsf(f32);
#pragma intrinsic(fabsf)
extern volatile f32 D_8002EB94;
extern char D_803B8650[],D_803B8660[];
/* func_8039D6A4 = GC updateButtons@carscr.c (MAT3 0x80094108, 0x8b8 B): per-frame scroll of the car-select option
 * buttons. Direct typed port of the MAT3 body with the N64 differences read from the retail displacements:
 *   - 19 option records per side (GC 17), right-hand copies at +19; cursor is record 39 (GC 35), A/B arrows 40..43.
 *   - unselected buttons settle to angle 0.0f (GC 0.08726646f); alpha ramps linearly over one button pitch
 *     (255.0f - d * 255 / (gButtonSpace * gButtonScale)) instead of GC's fixed 80.
 *   - validOption is func_8039D494 (member of this group); cursorSpeed / scrollSpeed are the function-local
 *     statics (retail .bss 0x803BA200 / 0x803BA210); gFrameSeconds is `volatile f32` (D_8002EB94).
 * Shaping that is real source, not a device:
 *   - comparisons with a fabsf() call are written `speed < fabsf(...) * 60 ...` (the call on the right): with the call
 *     on the left uopt reserves a 4-byte temporary in the local area (area 45 -> 41 here) and the colour/texIdx
 *     slots land 4 B too high. Unlike the optionscr sibling the clamps keep the GC `?:` form.
 *   - both alpha ramps spell the second 255 as the int `255` (and the first as 255.0f): that splits the 255.0f
 *     constant into two low-priority webs, so it is rematerialised as in retail instead of taking $f28 and
 *     starving the hoisted 1.5707964f ($f30, shared with the pre-loop WPitchUV call).
 *   - `mask = 1 << p` is the one named local beyond the MAT3 list: the retail local area has one more 4-byte local
 *     than the MAT3 declarations (texIdx at sp+114 and color at sp+108 instead of 118/112, homes unchanged at
 *     area 45). It is used (three times, as the shared model mask); an unused `s32 i` in the same slot gives the
 *     identical object.
 *   - `D_803AF9A8[idx+19].alpha = buttonColor.c[3] = D_803AF9A8[idx].alpha` (colour byte stored first).
 * Own .bss (the two statics), .data (colour initialiser) and .rodata floats are unverified for image A.
 */
void func_8039D6A4(s8 p)
{
    static f32 cursorSpeed[4];
    static f32 scrollSpeed[4];
    s32 buttonIdx;
    f32 prevPos[3];
    f32 buttonY;
    f32 selectedPos;
    s8 clearScrollSpeed=1;
    s32 mask;
    void *tex;
    u16 texIdx;
    Color4 buttonColor={{255,255,255,255}};
    if(!D_803B9B90[p])return;
    if((D_803BA050[p]-1)*D_803B25B0*D_803B25AC>D_803B25B8-D_803B25BC)
        selectedPos=D_803B25B8-D_803B28B8[p]*(D_803B25B8-D_803B25BC)/(D_803BA050[p]-1);
    else selectedPos=D_803B25B8-D_803B28B8[p]*D_803B25B0*D_803B25AC;
    memcpy(prevPos,D_803AF9A8[p][39].position,12);
    D_803AF9A8[p][39].angle+=6.2831855f*D_8002EB94;
    if(D_803AF9A8[p][39].angle>6.2831855f)D_803AF9A8[p][39].angle-=6.2831855f;
    D_803AF9A8[p][39].position[0]=D_803B25B4*D_803B25C4/D_803B25C0+-170.0f*D_803B25AC;
    D_803AF9A8[p][39].position[1]=selectedPos*D_803B25C4/D_803B25C0;
    D_803AF9A8[p][39].position[2]=D_803B25C4;
    if(!D_803B9BA0[p]) {
        if(cursorSpeed[p]<fabsf(D_803AF9A8[p][39].position[1]-prevPos[1])*60.0f*D_803B25AC/10.0f) {
            cursorSpeed[p]=fabsf(D_803AF9A8[p][39].position[1]-prevPos[1])*60.0f*D_803B25AC/10.0f;
            cursorSpeed[p]=cursorSpeed[p]<540.0f*D_803B25AC ? cursorSpeed[p] : 540.0f*D_803B25AC;
            cursorSpeed[p]=cursorSpeed[p]>180.0f*D_803B25AC ? cursorSpeed[p] : 180.0f*D_803B25AC;
        }
        if(D_803AF9A8[p][39].position[1]>prevPos[1]+D_8002EB94*cursorSpeed[p])
            D_803AF9A8[p][39].position[1]=prevPos[1]+D_8002EB94*cursorSpeed[p];
        else if(D_803AF9A8[p][39].position[1]<prevPos[1]-D_8002EB94*cursorSpeed[p])
            D_803AF9A8[p][39].position[1]=prevPos[1]-D_8002EB94*cursorSpeed[p];
        else cursorSpeed[p]=180.0f*D_803B25AC;
    }
    func_8008B32C(D_8011418C,D_803AF9A8[p][39].matrix,D_803B25AC*0.4f);
    func_800B5940(D_803AF9A8[p][39].angle,D_803AF9A8[p][39].matrix);
    func_800B5898(1.5707964f,D_803AF9A8[p][39].matrix);
    buttonY=selectedPos+D_803B28B8[p]*D_803B25B0*D_803B25AC;
    for(buttonIdx=0;buttonIdx<19;buttonIdx++) {
        if(buttonIdx==D_803BA060[p][D_803B28B8[p]]) {
            if(D_803AF9A8[p][buttonIdx].angle<3.1415927f)D_803AF9A8[p][buttonIdx].angle+=12.566371f*D_8002EB94;
            if(D_803AF9A8[p][buttonIdx].angle>3.1415927f)D_803AF9A8[p][buttonIdx].angle=3.1415927f;
            if(D_803B9BA0[p])D_803AF9A8[p][buttonIdx].angle=3.1415927f;
        } else {
            if(D_803AF9A8[p][buttonIdx].angle>0.0f)D_803AF9A8[p][buttonIdx].angle-=12.566371f*D_8002EB94;
            if(D_803AF9A8[p][buttonIdx].angle<0.0f)D_803AF9A8[p][buttonIdx].angle=0.0f;
            if(D_803B9BA0[p])D_803AF9A8[p][buttonIdx].angle=0.0f;
        }
        memcpy(prevPos,D_803AF9A8[p][buttonIdx].position,12);
        D_803AF9A8[p][buttonIdx].position[0]=-120.0f*D_803B25AC+D_803B25B4;
        D_803AF9A8[p][buttonIdx].position[1]=buttonY;
        D_803AF9A8[p][buttonIdx].position[2]=D_803B25C0;
        if(!D_803B9BA0[p]) {
            if(scrollSpeed[p]<fabsf(D_803AF9A8[p][buttonIdx].position[1]-prevPos[1])*60.0f/10.0f) {
                scrollSpeed[p]=fabsf(D_803AF9A8[p][buttonIdx].position[1]-prevPos[1])*60.0f/10.0f;
                scrollSpeed[p]=scrollSpeed[p]<540.0f*D_803B25AC ? scrollSpeed[p] : 540.0f*D_803B25AC;
                scrollSpeed[p]=scrollSpeed[p]>180.0f*D_803B25AC ? scrollSpeed[p] : 180.0f*D_803B25AC;
            }
            if(D_803AF9A8[p][buttonIdx].position[1]>prevPos[1]+D_8002EB94*scrollSpeed[p]) {
                D_803AF9A8[p][buttonIdx].position[1]=prevPos[1]+D_8002EB94*scrollSpeed[p];
                clearScrollSpeed=0;
            } else if(D_803AF9A8[p][buttonIdx].position[1]<prevPos[1]-D_8002EB94*scrollSpeed[p]) {
                D_803AF9A8[p][buttonIdx].position[1]=prevPos[1]-D_8002EB94*scrollSpeed[p];
                clearScrollSpeed=0;
            }
        }
        D_803AF9A8[p][buttonIdx+19].position[0]=120.0f*D_803B25AC+D_803B25B4;
        D_803AF9A8[p][buttonIdx+19].position[1]=D_803AF9A8[p][buttonIdx].position[1];
        D_803AF9A8[p][buttonIdx+19].position[2]=D_803AF9A8[p][buttonIdx].position[2];
        D_803AF9A8[p][buttonIdx+19].angle=D_803AF9A8[p][buttonIdx].angle;
        func_8008B32C(D_8011418C,D_803AF9A8[p][buttonIdx].matrix,D_803B25AC);
        func_8008B32C(D_8011418C,D_803AF9A8[p][buttonIdx+19].matrix,D_803B25AC);
        func_800B5898(D_803AF9A8[p][buttonIdx].angle-1.5707964f,D_803AF9A8[p][buttonIdx].matrix);
        func_800B5898(D_803AF9A8[p][buttonIdx+19].angle-1.5707964f,D_803AF9A8[p][buttonIdx+19].matrix);
        if(D_803AF9A8[p][buttonIdx].angle>1.5707964f)tex=func_800B24EC(D_803B8650,&texIdx,0,D_80140BDC-1,1);
        else tex=func_800B24EC(D_803B8660,&texIdx,0,D_80140BDC-1,1);
        func_8008D870((s16)D_803AF9A8[p][buttonIdx].handle,tex,-1);
        func_8008D870((s16)D_803AF9A8[p][buttonIdx+19].handle,tex,-1);
        if(D_803AF9A8[p][buttonIdx].position[1]>D_803B25B8+D_803B25B0*D_803B25AC || D_803AF9A8[p][buttonIdx].position[1]<D_803B25BC-D_803B25B0*D_803B25AC)D_803AF9A8[p][buttonIdx].alpha=0;
        else if(D_803AF9A8[p][buttonIdx].position[1]>D_803B25B8)D_803AF9A8[p][buttonIdx].alpha=255.0f-(D_803AF9A8[p][buttonIdx].position[1]-D_803B25B8)*255/(D_803B25B0*D_803B25AC);
        else if(D_803AF9A8[p][buttonIdx].position[1]<D_803B25BC)D_803AF9A8[p][buttonIdx].alpha=255.0f-(D_803B25BC-D_803AF9A8[p][buttonIdx].position[1])*255/(D_803B25B0*D_803B25AC);
        else D_803AF9A8[p][buttonIdx].alpha=255;
        if(!func_8039D494(p,buttonIdx))D_803AF9A8[p][buttonIdx].alpha=0;
        D_803AF9A8[p][buttonIdx+19].alpha=buttonColor.c[3]=D_803AF9A8[p][buttonIdx].alpha;
        func_8008E06C((s16)D_803AF9A8[p][buttonIdx].handle,&buttonColor);
        func_8008E06C((s16)D_803AF9A8[p][buttonIdx+19].handle,&buttonColor);
        if(!func_8039D494(p,buttonIdx)) {
            model_data_load(D_803AF9A8[p][buttonIdx].handle,1,15);
            model_data_load(D_803AF9A8[p][buttonIdx+19].handle,1,15);
        } else if(!D_803AF9A8[p][buttonIdx].alpha || D_803BA028[p]==1) {
            model_data_load(D_803AF9A8[p][buttonIdx].handle,1,15);
            model_data_load(D_803AF9A8[p][buttonIdx+19].handle,1,15);
            buttonY-=D_803B25B0*D_803B25AC;
        } else {
            mask=1<<p;
            model_transform_setup(D_803AF9A8[p][buttonIdx].handle,0,mask);
            model_transform_setup(D_803AF9A8[p][buttonIdx+19].handle,0,mask);
            buttonY-=D_803B25B0*D_803B25AC;
        }
    }
    if(D_803BA028[p]==1) {
        model_data_load(D_803AF9A8[p][40].handle,1,15);
        model_data_load(D_803AF9A8[p][41].handle,1,15);
        D_803AF9A8[p][39].position[2]=2000.0f;
    } else {
        mask=1<<p;
        model_transform_setup(D_803AF9A8[p][40].handle,0,mask);
        model_transform_setup(D_803AF9A8[p][41].handle,0,mask);
    }
    if(D_803BA02C==1) {
        model_data_load(D_803AF9A8[p][42].handle,1,15);
        model_data_load(D_803AF9A8[p][43].handle,1,15);
    } else {
        mask=1<<p;
        model_transform_setup(D_803AF9A8[p][42].handle,0,mask);
        model_transform_setup(D_803AF9A8[p][43].handle,0,mask);
    }
    if(clearScrollSpeed==1)scrollSpeed[p]=0.0f;
    D_803B9BA0[p]=0;
}

typedef struct CarHandles {s32 main;u8 unknown04[12];s32 parts[7];u8 unknown2C[20];} CarHandles;
extern CarHandles D_80139320[];
extern Color4 D_803B3054,D_803B3058;
void func_8039ED20(s32 enabled)
{
    if(enabled==1) {
        Color4 color=D_803B3054;
        if(D_80139320[D_803B9FD0[0]].main) {
            func_8008E06C(D_80139320[D_803B9FD0[0]].main,&color);
            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[0],&color);
            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[1],&color);
            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[3],&color);
            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[4],&color);
            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[5],&color);
            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[6],&color);
        }
    } else {
        Color4 color=D_803B3058;
        if(D_80139320[D_803B9FD0[0]].main) {
            func_8008E06C(D_80139320[D_803B9FD0[0]].main,&color);
            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[0],&color);
            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[1],&color);
            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[3],&color);
            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[4],&color);
            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[5],&color);
            func_8008E06C(D_80139320[D_803B9FD0[0]].parts[6],&color);
        }
    }
}

/* CONTEXT STAND-IN (not a match claim): overlay A 0x8039E444, 2268 bytes, GC updateCarComponents@carscr.c (MAT3 0x800910f8).
 * Best-effort port of the MAT3 body adapted to the N64 data (compLight bytes, 10.0f light scale, CarHandles frame/tires, no button
 * colour).  Compiled in the group only so that IDO sees the real call convention of the call in func_8039EF08: parameter p in
 * $s6, every callee-saved register clobbered.  Compiles to 576 words (retail 567); it is not scored and has no claim.
 */
typedef struct CompRec { s16 idx, level; f32 uvs[9]; f32 pos[3]; } CompRec;
#define CMP(p,i) ((CompRec *)&D_803B2618[p][i])
extern u16 D_803B2600[11];
extern char *D_803B25EC[5];
extern u8 D_803B2858[2][40];
extern f32 D_803B303C[3];
extern char D_803B85CC[];
extern u8 D_8012E708[];
extern void func_800A61B0(f32 *,f32 *,void *);
extern f32 func_800BEA30(void);
extern s32 engine_rpm_calc(s32,void *,s32,s32,s32);
extern s32 sprintf(char *,const char *,...);
extern void car_mass_set(s32,s32);
extern void func_80090E9C(f32,void *);
extern void func_8008B32C(void *,void *,f32);
extern void func_800A79B8(s32,s32,void *);
typedef struct UVMat { f32 uvs[9]; f32 pos[3]; } UVMat;
extern UVMat *func_8008B3A0(s16);
void func_8039E444(s32 p)
{
    s32 i;
    f32 lightVec[3];
    CarHandles *car;
    f32 tmp[3];
    s16 currentLevel;
    s32 whichLight;
    f32 compScale;
    char componentName[16];
    s32 mask;
    lightVec[0] = D_803B303C[0];
    lightVec[1] = D_803B303C[1];
    lightVec[2] = D_803B303C[2];
    if (D_8014A108 > 1 || D_8014A110 == 2) return;
    if (D_803BA028[p] != 1) {
        if (D_803BA060[p][D_803B28B8[p]] == 1) goto show_components;
        if (D_803BA060[p][D_803B28B8[p]] == 2) goto show_components;
        if (D_803BA060[p][D_803B28B8[p]] == 3) goto show_components;
        if (D_803BA060[p][D_803B28B8[p]] == 4) goto show_components;
        if (D_803BA060[p][D_803B28B8[p]] == 5) goto show_components;
    }
    {
        for (i = 0; i < 11; i++) {
            if (CMP(p, i)->idx != -1) {
                model_data_load(CMP(p, i)->idx, 1, 15);
                car_mass_set(CMP(p, i)->idx, -1);
            }
        }
        return;
    }
show_components:
    car = &D_80139320[p * 13 + D_803B9FD0[p]];
    if (D_803B25C8 == -1) {
        D_803B25C8 = engine_rpm_calc(string_copy_format(D_803B85CC, 0, D_80140BDC - 1, 1), func_8008B3A0((s16)car->main), 15, -1, 0);
        model_data_load(D_803B25C8, 1, 15);
    } else {
        *(void **)(D_8012E708 + D_803B25C8 * 68) = func_8008B3A0((s16)car->main);
    }
    D_803B2858[1][0] = 255.0f * func_800BEA30();
    func_800A61B0(lightVec, tmp, func_8008B3A0((s16)car->main));
    D_803B2858[0][16] = D_803B2858[1][16] = (s8)(tmp[0] * 10.0f);
    D_803B2858[0][17] = D_803B2858[1][17] = (s8)(tmp[1] * 10.0f);
    D_803B2858[0][18] = D_803B2858[1][18] = (s8)(tmp[2] * 10.0f);
    mask = 1 << p;
    for (i = 0; i < 11; i++) {
        currentLevel = -1;
        compScale = 1.0f;
        whichLight = 0;
        switch (D_803B2600[i]) {
        case 0:
            if (D_803BA060[p][D_803B28B8[p]] == 3) { whichLight = 1; compScale = 0.1f * func_800BEA30() + 1.0f; }
            currentLevel = D_8011108D[p][D_803B9FD0[p]] % 5;
            break;
        case 1:
            if (D_803BA060[p][D_803B28B8[p]] == 5) { whichLight = 1; compScale = 0.1f * func_800BEA30() + 1.0f; }
            currentLevel = D_8011123D[p][D_803B9FD0[p]] % 3;
            break;
        case 2:
            if (D_803BA060[p][D_803B28B8[p]] == 2) { whichLight = 1; compScale = 0.1f * func_800BEA30() + 1.0f; }
            currentLevel = D_80111049[p][D_803B9FD0[p]] % 3;
            break;
        case 3:
            if (D_803BA060[p][D_803B28B8[p]] == 4) { whichLight = 1; compScale = 0.1f * func_800BEA30() + 1.0f; }
            currentLevel = D_801111A9[p][D_803B9FD0[p]] % 4;
            break;
        case 4:
            if (D_803BA060[p][D_803B28B8[p]] == 1) { whichLight = 1; compScale = 0.1f * func_800BEA30() + 1.0f; }
            currentLevel = D_80110E85[p][D_803B9FD0[p]] % 2;
            break;
        }
        if (currentLevel != CMP(p, i)->level) {
            sprintf(componentName, D_803B25EC[D_803B2600[i]], currentLevel + 1);
            if (CMP(p, i)->idx != -1) {
                sound_call_minimal(CMP(p, i)->idx);
                CMP(p, i)->idx = -1;
                CMP(p, i)->level = -1;
            }
            CMP(p, i)->idx = sign_extend_call(string_copy_format(componentName, 0, D_80140BDC - 1, 1), CMP(p, i)->uvs, -1, 0);
            model_data_load(CMP(p, i)->idx, 1, 15);
            if (D_80151AD0 == 1) {
                CMP(p, i)->pos[0] = 0.0f;
                CMP(p, i)->pos[1] = 1.0f;
                CMP(p, i)->pos[2] = 15.0f;
            } else {
                CMP(p, i)->pos[0] = 0.0f;
                CMP(p, i)->pos[1] = 2.0f;
                CMP(p, i)->pos[2] = 15.0f;
            }
            CMP(p, i)->level = currentLevel;
        }
        switch (i) {
        case 0: case 1: case 10:
            func_8008B32C(D_8011418C, CMP(p, i)->uvs, compScale);
            CMP(p, i)->pos[0] = 0.0f;
            CMP(p, i)->pos[1] = 0.0f;
            CMP(p, i)->pos[2] = 15.0f;
            break;
        case 2: case 6:
            func_8008B32C(func_8008B3A0((s16)car->parts[3])->uvs, CMP(p, i)->uvs, compScale);
            CMP(p, i)->pos[0] = func_8008B3A0((s16)car->parts[3])->pos[0];
            CMP(p, i)->pos[1] = func_8008B3A0((s16)car->parts[3])->pos[1];
            CMP(p, i)->pos[2] = func_8008B3A0((s16)car->parts[3])->pos[2];
            func_80090E9C(3.1415927f, CMP(p, i)->uvs);
            break;
        case 3: case 7:
            func_8008B32C(func_8008B3A0((s16)car->parts[4])->uvs, CMP(p, i)->uvs, compScale);
            CMP(p, i)->pos[0] = func_8008B3A0((s16)car->parts[4])->pos[0];
            CMP(p, i)->pos[1] = func_8008B3A0((s16)car->parts[4])->pos[1];
            CMP(p, i)->pos[2] = func_8008B3A0((s16)car->parts[4])->pos[2];
            break;
        case 5: case 9:
            func_8008B32C(func_8008B3A0((s16)car->parts[5])->uvs, CMP(p, i)->uvs, compScale);
            CMP(p, i)->pos[0] = func_8008B3A0((s16)car->parts[5])->pos[0];
            CMP(p, i)->pos[1] = func_8008B3A0((s16)car->parts[5])->pos[1];
            CMP(p, i)->pos[2] = func_8008B3A0((s16)car->parts[5])->pos[2];
            func_80090E9C(3.1415927f, CMP(p, i)->uvs);
            break;
        case 4: case 8:
            func_8008B32C(func_8008B3A0((s16)car->parts[6])->uvs, CMP(p, i)->uvs, compScale);
            CMP(p, i)->pos[0] = func_8008B3A0((s16)car->parts[6])->pos[0];
            CMP(p, i)->pos[1] = func_8008B3A0((s16)car->parts[6])->pos[1];
            CMP(p, i)->pos[2] = func_8008B3A0((s16)car->parts[6])->pos[2];
            break;
        }
        car_mass_set(CMP(p, i)->idx, (s16)D_803B25C8);
        model_transform_setup(CMP(p, i)->idx, 0, mask);
        func_800A79B8(CMP(p, i)->idx, 1, D_803B2858[whichLight]);
    }
}


/* Overlay A 0x8039EF08, 4092 bytes: GC showCarSelect@carscr.c (MAT3 0x8008eddc, 0xb28 B), static.
 * Source: reference/repos/mat3 decomp/src/GameSource/src/carscr.c showCarSelect (typed port).  The MAT3 local order
 * (uv,i,p,carPos,alpha,framesLeft,reflectIdx,reflectUV,reflectPos,ghostIdx,ghostCar,show) reproduces the retail frame offsets
 * uv@212 i@208 p@204 carPos@192 reflectUV@144 reflectPos@132 exactly.  The N64 build is an earlier revision than GC: selection
 * fades (`+= 5.0f * gFrameSeconds`), the "face front" mode is gOpts==11 (GC 9), reflection alpha is alpha/1.5f, no alpha/4,
 * UpdateSelectCar takes 9 args (slot,p,paint,rims,horn,sheen,pos,uv,alpha), func_8039ED20(boolean) replaces showGridMap and
 * updateCarComponents is func_8039E444(p) (register parameter $s6, clobbers every callee-saved register).
 * Shaping, all verified on the builder (group dot_cleanup_colortrace_20261011):
 *  - the two `gThisCarIndex[p] == i` reads are volatile (cast): retail does not share the load between them, and a shared
 *    load becomes a coloured web that steals $v0 and moves p out of $s6 (nine code-free `if (p) {}` reads gave the same
 *    allocation before this was found; they are gone);
 *  - `if (!D_80152698[ghostIdx]) continue;`: the loop test `n + 1` is then kept in both paths, as in retail (the if-block form
 *    folds it to `n < ghostIdx`);
 *  - `0.f` for the angle>0 compare and `3.14159265f` for the RollUV argument give retail's separate constants/rodata entries;
 *  - two unused word locals (xx1, xx2) complete the 248-byte frame (frame without them 240; `show` is the third extra word).
 * Needs func_8039E444 in the group (stand-in context): without it the caller keeps its registers across the call.
 *  - (w36a) the `angle > 1.5707964f` test and its then-statement are on separate source lines: as1's cross-block pull fills the
 *    stall slots of the block after `angle -= 1.5707964f` with the compare's `lui at` and the then-arm's `&gFrameSeconds`
 *    (`lui t9; addiu t9`), and breaks the aftercycles tie between `lui at` and `addiu t9` by source line.  On one line the
 *    tie went to `addiu t9` (2 words off, w35b); on two lines `lui at` (the compare, earlier line) goes first, as in retail.
 * Matches in this group (score.py group, strict).  Own .rodata verified by hand against decompressed image A (33/33 literal
 * sites, w35b rocheck.py).
 */
#define CAR_MIN(a,b) ((a) < (b) ? (a) : (b))
extern void func_80390DCC(s16,s16,u8,u8,u8,s16,f32 *,f32 (*)[3],u32);
extern void func_80090F44(f32,void *);
extern void func_80090E9C(f32,void *);
extern void func_8009EA68(f32,void *);
extern void func_8008B32C(void *,void *,f32);
extern s8 D_80138664[];
void func_8039EF08(void)
{
    f32 uv[3][3];
    s32 i;
    s32 p;
    f32 carPos[3];
    u8 alpha;
    s32 framesLeft;
    s8 reflectIdx;
    f32 reflectUV[3][3];
    f32 reflectPos[3];
    s32 ghostIdx;
    s32 ghostCar;
    s32 show;
    s32 xx1;
    s32 xx2;
    for (p = 0; p < D_8014A108; p++) {
        for (i = 0; i < D_803BA020[p]; i++) {
            if (((volatile s8 *)D_803B9FD4)[p] == i) {
                D_803B9BC8[p][i].selection += 5.0f * D_8002EB94;
                if (D_803B9BC8[p][i].selection > 1.0f) D_803B9BC8[p][i].selection = 1.0f;
            } else {
                D_803B9BC8[p][i].selection -= 5.0f * D_8002EB94;
                if (D_803B9BC8[p][i].selection < 0.0f) D_803B9BC8[p][i].selection = 0.0f;
            }
            if (D_803BA028[p]) {
                framesLeft = 10 - D_803BA030;
                if (framesLeft <= 1) D_803B9BC8[p][i].angle = 3.6651914f;
                else {
                    D_803B9BC8[p][i].angle -= 3.6651914f;
                    while (D_803B9BC8[p][i].angle > 3.1415927f) D_803B9BC8[p][i].angle -= 6.2831855f;
                    while (D_803B9BC8[p][i].angle <= -3.1415927f) D_803B9BC8[p][i].angle += 6.2831855f;
                    D_803B9BC8[p][i].angle -= D_803B9BC8[p][i].angle / (framesLeft - 1);
                    D_803B9BC8[p][i].angle += 3.6651914f;
                }
            } else if (D_803BA060[p][D_803B28B8[p]] == 11) {
                D_803B9BC8[p][i].angle -= 1.5707964f;
                while (D_803B9BC8[p][i].angle > 3.1415927f) D_803B9BC8[p][i].angle -= 6.2831855f;
                while (D_803B9BC8[p][i].angle <= -3.1415927f) D_803B9BC8[p][i].angle += 6.2831855f;
                if (D_803B9BC8[p][i].angle > 1.5707964f)
                    D_803B9BC8[p][i].angle += CAR_MIN(1.0471976f * D_8002EB94 * 4.0f, 3.1415927f - D_803B9BC8[p][i].angle);
                else if (D_803B9BC8[p][i].angle < -1.5707964f) D_803B9BC8[p][i].angle -= CAR_MIN(1.0471976f * D_8002EB94 * 4.0f, 3.1415927f + D_803B9BC8[p][i].angle);
                else if (D_803B9BC8[p][i].angle > 0.f) D_803B9BC8[p][i].angle -= CAR_MIN(1.0471976f * D_8002EB94 * 4.0f, D_803B9BC8[p][i].angle);
                else D_803B9BC8[p][i].angle += CAR_MIN(1.0471976f * D_8002EB94 * 4.0f, -D_803B9BC8[p][i].angle);
                D_803B9BC8[p][i].angle += 1.5707964f;
            } else {
                D_803B9BC8[p][i].angle += 1.0471976f * D_8002EB94;
                if (D_803B9BC8[p][i].angle >= 6.2831855f) D_803B9BC8[p][i].angle -= 6.2831855f;
            }
            func_8008B32C(D_8011418C, uv, 2.5f);
            func_80090F44(0.34906584f, uv);
            func_80090E9C(D_803B9BC8[p][i].angle, uv);
            if (D_803B9BC8[p][i].car % 13 == 10) {
                if (D_80151AD0 == 1) { carPos[0] = 0.0f; carPos[1] = 3.2f; carPos[2] = 40.5f; }
                else { carPos[0] = 0.0f; carPos[1] = 5.9f; carPos[2] = 40.5f; }
            } else {
                if (D_80151AD0 == 1) { carPos[0] = 0.0f; carPos[1] = 2.6f; carPos[2] = 40.5f; }
                else { carPos[0] = 0.0f; carPos[1] = 5.3f; carPos[2] = 40.5f; }
            }
            alpha = D_803B9BC8[p][i].selection * 255.0f;
            if (D_8014A110 == 6 && D_801164BE == 0)
                func_80390DCC(D_803B9BC8[p][i].car, p, D_80138664[D_8012E67C[p]], D_80138664[D_8012E67C[p]], D_80138664[D_8012E67C[p]], D_803B28B0[p], carPos, uv, alpha);
            else func_80390DCC(D_803B9BC8[p][i].car, p, D_80111604[p + 1][D_803B9BC8[p][i].car % 13], D_80111648[p + 1][D_803B9BC8[p][i].car % 13], D_8011168C[p + 1][D_803B9BC8[p][i].car % 13], D_803B28B0[p], carPos, uv, alpha);
            if (D_8014A108 < 3 && D_8014A110 != 2) {
                reflectIdx = p == 0 ? 2 : 3;
                func_8008B32C(uv, reflectUV, 1.2f);
                func_8009EA68(3.14159265f, reflectUV);
                reflectPos[0] = 1.2f * carPos[0];
                reflectPos[1] = 1.2f * carPos[1];
                reflectPos[2] = 1.2f * carPos[2];
                if (D_803B9BC8[p][i].car % 13 == 10) reflectPos[1] -= 4.0f;
                else reflectPos[1] -= 2.5f;
                D_803B9BC8[reflectIdx][i].car = D_803B9BC8[p][i].car + 26;
                if (D_8014A110 == 6 && D_801164BE == 0)
                    func_80390DCC(D_803B9BC8[reflectIdx][i].car, reflectIdx, D_80138664[D_8012E67C[p]], D_80138664[D_8012E67C[p]], D_80138664[D_8012E67C[p]], D_803B28B0[p], reflectPos, reflectUV, alpha / 1.5f);
                else func_80390DCC(D_803B9BC8[reflectIdx][i].car, reflectIdx, D_80111604[p + 1][D_803B9BC8[p][i].car % 13], D_80111648[p + 1][D_803B9BC8[p][i].car % 13], D_8011168C[p + 1][D_803B9BC8[p][i].car % 13], D_803B28B0[p], reflectPos, reflectUV, alpha / 1.5f);
            }
            if (((volatile s8 *)D_803B9FD4)[p] == i) {
                func_8008B32C(D_8011418C, D_803B9E40[p][0], 1.3f);
                func_8008B32C(D_8011418C, D_803B9E40[p][1], 1.3f);
                func_80090F44(0.34906584f, D_803B9E40[p][0]);
                func_80090F44(0.34906584f, D_803B9E40[p][1]);
                func_80090E9C(D_803B9BC8[p][i].angle, D_803B9E40[p][0]);
                if (D_80151AD0 == 1) {
                    D_803B9E40[p][0][9] = 0.0f;
                    D_803B9E40[p][0][10] = 1.8f;
                    D_803B9E40[p][0][11] = 45.0f;
                    D_803B9E40[p][1][9] = 0.0f;
                    D_803B9E40[p][1][10] = 1.8f;
                    D_803B9E40[p][1][11] = 45.0f;
                } else {
                    D_803B9E40[p][0][9] = 0.0f;
                    D_803B9E40[p][0][10] = 4.8f;
                    D_803B9E40[p][0][11] = 45.0f;
                    D_803B9E40[p][1][9] = 0.0f;
                    D_803B9E40[p][1][10] = 4.8f;
                    D_803B9E40[p][1][11] = 45.0f;
                }
                if (D_8014A110 == 2) {
                    for (ghostIdx = 1; ghostIdx < D_803BA00C + 1; ghostIdx++) {
                        if (!D_80152698[ghostIdx] || D_803BA018[ghostIdx] != (*(*D_80152698[ghostIdx])->active)->car) {
                            if (D_803BA018[ghostIdx] != -1) func_80390DCC(D_803B9BC8[ghostIdx][D_803BA018[ghostIdx]].car, ghostIdx, 0, 0, 0, 0, carPos, uv, 0);
                        }
                        if (!D_80152698[ghostIdx]) continue;
                        ghostCar = (*(*D_80152698[ghostIdx])->active)->car;
                        D_803BA018[ghostIdx] = ghostCar;
                        D_803B9BC8[ghostIdx][ghostCar].car = ghostCar + ghostIdx * 13;
                        D_8011119C[ghostIdx + 1][ghostCar] = (*(*D_80152698[ghostIdx])->active)->component;
                        D_8011157C[ghostIdx + 1][ghostCar] = (*(*D_80152698[ghostIdx])->active)->part;
                        D_801112DC[ghostIdx + 1][ghostCar] = (*(*D_80152698[ghostIdx])->active)->stat0 / 100.0f + 0.75f;
                        D_801113E0[ghostIdx + 1][ghostCar] = (*(*D_80152698[ghostIdx])->active)->stat1 / 100.0f + 0.75f;
                        func_80390DCC(D_803B9BC8[ghostIdx][ghostCar].car, ghostIdx, (*(*D_80152698[ghostIdx])->active)->red, (*(*D_80152698[ghostIdx])->active)->green, (*(*D_80152698[ghostIdx])->active)->blue, 0, carPos, uv, 255);
                    }
                }
            }
        }
        if (D_8014A108 == 1 && D_8014A110 != 2) {
            show = !D_803BA028[0] && (D_803BA060[p][D_803B28B8[p]] == 1 || D_803BA060[p][D_803B28B8[p]] == 2 || D_803BA060[p][D_803B28B8[p]] == 3 || D_803BA060[p][D_803B28B8[p]] == 4 || D_803BA060[p][D_803B28B8[p]] == 5);
            func_8039ED20(show);
            func_8039E444(p);
        }
    }
}
