/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image A player-select screen (MAT3 GameSource playerscr.c), one IPA unit rooted at the kept func_8039842C
 * (updatePlayerScreen, GC 0x80079f80). Claimed members (all strict in this group):
 *   func_80394564 updateSelectInfo, func_80394834 updatePakInfo, func_80394B00 setPlayerState, func_80397AD8
 *   initButtons (the w27p bodies, unchanged); func_80397FFC initScreen (GC 0x8007a49c); func_803968C0
 *   controlCreatePlayer (GC 0x8007b464); func_80395E8C controlDeletePlayer (GC 0x8007b9f8); func_80394DD8
 *   updateButtons(signed char) (GC 0x8007fc14). Context only: func_8039842C (122/451 words), func_80396FA0
 *   controlSelect (GC 0x8007a854, 262/718), func_803949CC uninitScreen, func_80395E24 playerClaimed,
 *   func_80396EE8 / func_80396F44 (installed exact bodies, copied verbatim).
 * Donor: MAT3 playerscr.c, direct typed ports; N64 differences are from the retail words: 14 menu items and 22
 * buttons (GC 15 / 23), no gCreateMenuNotify, B deletes a character in name entry, a pak-changed test in the
 * wait state, a "controls" arrow button (index 14) next to the selected item, 0.0f lower angle limit, and
 * gFrameSeconds / gNumModels are volatile.
 * Inlined statics (their 8-byte deleted stubs are non-static symbols, not on keep): func_80397FF4 =
 * calcSelectMenuMaxSize (inlined into initScreen), func_80396EE0 = claimPlayer (inlined 4x into controlSelect and 1x
 * into controlCreatePlayer). Their parameters are what the frame needs: defining them as helpers replaced the
 * `screens` local of the earlier research body and two unused locals.
 * Shaping devices (each needed for the exact frame; disclosed per the Wave 12/16 rule):
 *   func_80394DD8: two unused s32 locals before prevPos (frame 152 -> 160, locals at sp+104/110/119/132).
 *   func_8039842C: u0..u6 seven unused s32 locals (frame 160 -> 192; first spill home sp+116, s8 home sp+188).
 * Float zero in the type-9 button x is `0 * gButtonScale` (int zero; a float zero is folded by uopt), 255.f and
 * 255.0f are deliberately different spellings (one shared 255 constant is wrongly CSE'd), `q` in the root is a
 * signed-char variable assigned in every switch arm (a plain argument is elided by IPA).
 * All source names are reconstructions; none is asserted as original.
 * w36c (wave 36): func_80396FA0 = controlSelect@playerscr.c (MAT3 GC 0x8007a854) becomes a claimed member. Its body
 * is unchanged from w35d except one compiled-out statement `do {} while (0);` between the edges chain and the
 * autoFlags chain (SHAPING, disclosed; plausibly an empty release-build macro). The extra block boundary changes the
 * colouring order: the constant-13 web (whole-web save 0.5758 vs the lda D_803BADB8 web 0.44, both split) no longer
 * reaches a3 first, so the first-loop lda piece takes a3 as in retail (traced: forcing the 13 web to split gives the
 * same object). Own .rodata = its 14-entry jump table (retail 0x803B9484), entries hand-checked equal.
 * Context func_8039842C (kept root, not claimed) gets the same kind of compiled-out `do {} while (0);` before the
 * main player loop: its gNumPlayers preheader load is then coloured a1 as in retail (122 -> 2 words; the last 2 are
 * the spill home of the reset loop's walking pointer, sp+120 vs retail sp+124).
 */
typedef signed char s8;typedef unsigned char u8;
typedef short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
extern s16 D_8014A108;
extern s8 D_803BADBC[],D_803BADC0[],D_803BAD78[];
extern s32 D_803BAC20[],D_803BAC68[],D_803BACE0[];
extern s8 D_803BAD00[][14],D_803BAD40[][14];

void func_80394564(s32 player)
{
    s32 first = player;
    s32 last = player;
    s32 j;
    if (player == -1) {first = 0;last = D_8014A108 - 1;}
    for (player = first; player <= last; player++) {
        D_803BADC0[player] = D_803BAC20[D_803BADBC[player]];
        D_803BAD00[player][0] = 1;
        D_803BAD00[player][1] = 1;
        D_803BAD00[player][2] = 0;
        for (j = 0; j < 8; j++)
            D_803BAD00[player][j + 3] = j < D_803BADC0[player];
        D_803BAD00[player][11] = D_803BADC0[player] >= 0 && !D_803BAC68[D_803BADBC[player]];
        D_803BAD00[player][12] = D_803BADC0[player] > 0;
        D_803BAD00[player][13] = 0;
        if (D_803BACE0[player] == 4) {
            D_803BAD00[player][0] = 0;
            D_803BAD00[player][1] = 0;
            D_803BAD00[player][11] = 0;
            D_803BAD00[player][12] = 0;
        }
        D_803BAD40[player][0] = D_803BAD00[player][0];
        D_803BAD40[player][1] = D_803BACE0[player] == 4 || D_803BAD00[player][1];
        D_803BAD40[player][2] = D_803BADC0[player] < 1 || (D_803BAC68[D_803BADBC[player]] && D_803BACE0[player] != 4);
        for (j = 0; j < 8; j++) D_803BAD40[player][j + 3] = D_803BAD00[player][j + 3];
        D_803BAD40[player][11] = D_803BAD00[player][11];
        D_803BAD40[player][12] = D_803BAD00[player][12];
        D_803BAD40[player][13] = D_803BAD00[player][13];
        D_803BAD78[player] = 0;
        for (j = 0; j < 14; j++) if (D_803BAD40[player][j]) D_803BAD78[player]++;
    }
}

typedef struct Owner Owner;
typedef struct Item {u8 other[16];u8 player;} Item;
typedef struct Selection {Item *item;} Selection;
typedef struct OwnerBody {Owner *next,*previous;Selection *selection;u8 unknown0C[8];u8 name[1];} OwnerBody;
struct Owner {OwnerBody *body;};
typedef struct Player {u8 unaccessed0,id;u16 unaccessed2;u32 flags;u32 unaccessed8;u32 autoFlags;u8 unaccessed10[56];Owner *owner;} Player;
typedef struct Blit Blit;typedef struct FiveParts FiveParts;
typedef struct MultiBlit {char *name;s16 x,y,width,height,top,bottom,left,right;u32 z,alpha;s32 (*callback)(Blit *);u32 descriptor;} MultiBlit;
extern Player D_8014A118[];
extern u8 D_8012E67C[];
extern s32 D_803BAD80[],D_803B5CFC[][24];
extern s8 D_803BACF4,D_803B5ED0;
extern s16 D_80151AD0;
extern f32 D_803B5D0C,D_803B5D10,D_803B5D14,D_803B5D18,D_803B5D1C,D_803B5D20,D_803B5D24,D_803B5D28;
extern FiveParts *D_803BAC08[];
extern MultiBlit D_803B5D2C[];
extern Blit *D_803B5ED8;
extern s32 D_803B5ED4;
extern void func_800A3424(void **,s32);
extern void func_80394834(void);
extern void func_80394B00(s32,s32);
extern void *object_create(s32);
extern s16 object_bytes_sum_global(void);
extern void func_80397AD8(s8);
extern void func_800A6404(s32);
extern void particle_lifetime_set(void);
extern FiveParts *ambient_sound_set(s32,s32,s32,s32,s32,s32,s32,s32);
extern Blit *sound_control(s16,s16,const MultiBlit *,s16);
extern void entity_audio_update(void);

void func_80397FF4(void);


void func_80397FFC(void)
{
    s32 p;
    for (p = 0; p < D_8014A108; p++) {
        if (D_8014A118[p].owner) func_800A3424((void **)D_8014A118[p].owner->body->selection,0);
    }
    func_80394834();
    for (p = 0; p < D_8014A108; p++) {
        D_8012E67C[p] = p;
        D_8014A118[p].id = p;
        D_803BAD80[p] = 3;
        func_80394B00(p,0);
        func_80397FF4();
        func_80397AD8(p);
    }
    func_80394564(-1);
    D_80151AD0 = D_8014A108;
    if (D_80151AD0 == 3) D_80151AD0 = 4;
    if (D_80151AD0 == 1) {
        D_803B5D0C = 1.0f;
        D_803B5D10 = 1.0f;
        D_803B5D14 = 40.0f;
        D_803B5D18 = 0.0f;
        D_803B5D1C = 100.0f;
        D_803B5D20 = -120.0f;
        D_803B5D24 = 300.0f;
        D_803B5D28 = 225.0f;
    } else if (D_80151AD0 == 2) {
        D_803B5D0C = 1.0f;
        D_803B5D10 = 1.43f;
        D_803B5D14 = 40.0f;
        D_803B5D18 = 0.0f;
        D_803B5D1C = 69.0f;
        D_803B5D20 = -120.0f;
        D_803B5D24 = 300.0f;
        D_803B5D28 = 225.0f;
    } else {
        D_803B5D0C = 1.0f;
        D_803B5D10 = 1.1f;
        D_803B5D14 = 40.0f;
        D_803B5D18 = 0.0f;
        D_803B5D1C = 69.0f;
        D_803B5D20 = -120.0f;
        D_803B5D24 = 300.0f;
        D_803B5D28 = 225.0f;
    }
    func_800A6404(D_80151AD0);
    particle_lifetime_set();
    for (p = 0; p < D_8014A108; p++) {
        D_803BAC08[p] = ambient_sound_set(-20,0,-10,10,192,0,0,0);
    }
    D_803B5ED8 = sound_control(0,0,D_803B5D2C,1);
    D_803B5ED4 = 0;
    entity_audio_update();
    D_803B5ED0 = 1;
}

void func_80397FF4(void)
{
    s32 rectHeight = D_803B5CFC[D_8014A108][0];
    if (D_8014A108 >= 2) object_create(11);
    else object_create(13);
    rectHeight -= object_bytes_sum_global();
    if (D_8014A108 >= 2) object_create(12);
    else object_create(10);
    D_803BACF4 = rectHeight / object_bytes_sum_global();
}

extern Owner *D_8012E6E0,*D_803BAC80[];
extern Owner D_80146150[];
extern s8 D_803BADB8[],D_803BADC8[],D_803BADCC[],D_803BADD0[],D_803BADD4[];
extern u8 D_803BAC98[][13];
extern s16 D_803BACD0[],D_803BACD8[];
extern s32 D_803BA888[];
extern void *memset(void *,s32,u32);

void func_80394B00(s32 player,s32 mode)
{
    Owner *node,*other;
    if (mode == 0) {
        D_803BAC80[player] = 0;
        node = D_8012E6E0;
        while (node) {
            if (node == D_8014A118[player].owner) break;
            node = node->body->next;
        }
        D_8014A118[player].owner = 0;
        if (!node || !node->body->selection) {
            D_803BAC80[player] = &D_80146150[player];
            if (D_803BACE0[player] != 3 && D_803BACE0[player] != 4)
                D_803BADBC[player] = ((s32)(D_8014A118[player].id * sizeof(s32))) / (s32)sizeof(s32);
            if (D_803BAD80[player] >= 3 && D_803BAD80[player] < 11)
                D_803BAD80[player] = 0;
        } else {
            D_803BAC80[player] = node;
            D_803BADBC[player] = node->body->selection->item->player;
            if (D_803BAD80[player] >= 3 && D_803BAD80[player] < 11) {
                D_803BAD80[player] = 3;
                for (other = D_8012E6E0; other; other = other->body->next) {
                    if (other == node) break;
                    if (node->body->selection->item->player == other->body->selection->item->player)
                        if (D_803BAD80[player] < 10) D_803BAD80[player]++;
                }
            }
        }
        D_803BADB8[player] = 0;
    } else if (mode == 3) {
        memset(D_803BAC98[player],0,13);
        D_803BACD0[player] = 0;
        D_803BACD8[player] = 0;
        D_8014A118[player].owner = 0;
    } else if (mode == 4) {
        D_803BADC8[player] = 1;
        D_803BADCC[player] = 0;
        D_803BADD0[player] = 0;
        D_803BADD4[player] = 0;
        D_8014A118[player].owner = 0;
    }
    D_803BACE0[player] = mode;
    D_803BA888[player] = 1;
    func_80394564(player);
}

typedef struct PlayerPresence { s8 present; u8 unknown1[15]; } PlayerPresence;
extern PlayerPresence D_80156CF0[];
extern Owner *D_8012E6E0;
extern s32 D_803BA888[];
extern s8 func_800A1A3C(s32),func_800A35F8(s32),func_80094FC4(s32);
extern u32 func_800A35BC(s32);
extern s32 func_800A3508(s32),func_800A3518(s32);

void func_80394834(void)
{
    s32 i,j;
    Owner *node;
    u32 available;
    for(i=0;i<4;i++) {
        D_803BAC68[i]=0;
        if(!D_80156CF0[i].present)D_803BAC20[i]=-4;
        else if(!func_800A1A3C(i))D_803BAC20[i]=-1;
        else if(func_800A35F8(i))D_803BAC20[i]=-2;
        else if(func_80094FC4(i))D_803BAC20[i]=-3;
        else {
            if(D_803BAC20[i]<0) {
                for(j=0;j<D_8014A108;j++) {
                    if(D_803BADBC[j]==i)D_803BA888[j]=1;
                }
            }
            D_803BAC20[i]=0;
            for(node=D_8012E6E0;node;node=node->body->next) {
                if(node->body->selection->item->player==i)D_803BAC20[i]++;
            }
            if(D_803BAC20[i]==0)D_803BAC20[i]=0;
            available=func_800A35BC(i);
            D_803BAC68[i]=available<(u32)func_800A3508(0x810) || !func_800A3518(i);
        }
    }
}
typedef struct Slot64 {
    s32 unknown0,id;
    f32 angle,matrix[3][3],position[3];
    u8 alpha,unknown3D[3];
} Slot64;
extern Slot64 D_803B470C[][22];
extern s32 D_803BA868[];
extern f32 D_80154188;
extern void sound_call_minimal(s16),sound_handles_array_clear(FiveParts *);
extern void render_results_screen(void),sound_stop(Blit *),ambient_sounds_clear(void);
extern void func_800A5A40(void),func_800A5B3C(void);

void func_803949CC(void)
{
    s32 i,j;
    s8 player;
    Slot64 *slot;
    for(i=0;i<D_8014A108;i++) {
        player=i;
        for(j=0;j<22;j++) {
            slot=&D_803B470C[player][j];
            if(slot->id!=-1) {
                sound_call_minimal(slot->id);
                slot->id=-1;
            }
        }
        D_803BA868[player]=0;
        if(D_803BAC08[i]) {
            sound_handles_array_clear(D_803BAC08[i]);
            D_803BAC08[i]=0;
        }
    }
    render_results_screen();
    if(D_803B5ED8) {
        sound_stop(D_803B5ED8);
        D_803B5ED8=0;
    }
    ambient_sounds_clear();
    D_80154188=90.0f;
    D_80151AD0=1;
    func_800A5A40();
    func_800A5B3C();
    D_803B5ED0=0;
}

extern volatile u8 D_80140BDC;
extern char *D_803B46DC[];
extern char D_803B89D4[],D_803B89E4[],D_803B89EC[],D_803B89F0[];
extern f32 D_8011418C[3][3];
extern void *memcpy(void *,const void *,u32);
extern void *string_copy_format(char *,s32,s8,s32);
extern s32 sign_extend_call(void *,f32 (*)[3],s32,u32);
extern void func_8008E06C(s16,u8 *),model_data_load(s32,s32,s32),model_transform_setup(s32,s32,s32);
extern void *func_800B24EC(char *,s16 *,s8,s8,s32);
extern void func_8008D870(s16,void *,s32);
extern void func_800B5898(f32,f32 (*)[3]),func_800B5940(f32,f32 (*)[3]);
extern void func_8008B32C(f32 (*)[3],f32 (*)[3],f32);
extern void particle_velocity_set(void);
/* Complete native-led 97AD8 model-slot initialization: strict 327-word match
 * in this genuine context. The consumed scale local preserves the native
 * multiply-by-one operations that literal folding removed, including their
 * floating-point evaluation. Its native-derived declaration position leaves
 * color at sp+76 and the genuine s16 output at sp+70 in the unchanged 80-byte
 * frame. This is not an unused frame filler or a recovered original name.
 * All 40 literal bytes and the complete four-byte white color initializer
 * are authenticated against image A. Slot64 offsets/capacities are unchanged.
 * No fabricated formal or caller: actual input is one signed player byte
 * passed by the sole genuine 97FFC caller at incoming stack+3. Empty claims:
 * the nonmatching caller and cross-group ownership still block installation.
 */
/* Existing matched texture lookup declares this table-count byte volatile. */
void func_80397AD8(s8 player)
{
    u8 color[4]={255,255,255,255};
    f32 scale=1.0f;
    s16 count;
    s32 j;
    Slot64 *slot;
    void *resource;
    for(j=0;j<22;j++) {
        slot=&D_803B470C[player][j];
        if(slot->id==-1) {
            memcpy(slot->matrix,D_8011418C,36);
            resource=string_copy_format(D_803B46DC[slot->unknown0],0,D_80140BDC-1,1);
            slot->id=sign_extend_call(resource,slot->matrix,-1,
                slot->unknown0==0 || slot->unknown0==1 || slot->unknown0==2 ||
                slot->unknown0==7 || slot->unknown0==8 || slot->unknown0==9 ? 0x42000:0);
            func_8008E06C(slot->id,color);
            model_data_load(slot->id,1,15);
            model_transform_setup(slot->id,0,1<<player);
        }
    }
    resource=func_800B24EC(D_803B89D4,&count,0,D_80140BDC-1,1);
    func_8008D870(D_803B470C[player][16].id,resource,-1);
    memcpy(D_803B470C[player][16].matrix,D_8011418C,36);
    func_800B5898(-1.57079632679f,D_803B470C[player][16].matrix);
    D_803B470C[player][16].position[0]=0.0f;
    D_803B470C[player][16].position[1]=D_8014A108>=2?105.0f:115.0f;
    D_803B470C[player][16].position[0]*=scale;
    D_803B470C[player][16].position[1]*=scale;
    D_803B470C[player][16].position[2]=200.0f*scale;
    resource=func_800B24EC(D_803B89E4,&count,0,D_80140BDC-1,1);
    func_8008D870(D_803B470C[player][17].id,resource,-1);
    resource=func_800B24EC(D_803B89EC,&count,0,D_80140BDC-1,1);
    func_8008D870(D_803B470C[player][18].id,resource,-1);
    memcpy(D_803B470C[player][18].matrix,D_8011418C,36);
    func_800B5898(-1.57079632679f,D_803B470C[player][18].matrix);
    D_803B470C[player][18].position[0]=75.1f;
    D_803B470C[player][18].position[1]=D_8014A108>=2?50.2f:55.2f;
    D_803B470C[player][18].position[2]=100.0f;
    func_8008B32C(D_803B470C[player][18].matrix,D_803B470C[player][18].matrix,.5f);
    memcpy(D_803B470C[player][19].matrix,D_8011418C,36);
    func_800B5898(-1.57079632679f,D_803B470C[player][19].matrix);
    func_800B5940(3.14159265359f,D_803B470C[player][19].matrix);
    D_803B470C[player][19].position[0]=87.1f;
    D_803B470C[player][19].position[1]=D_8014A108>=2?50.2f:55.2f;
    D_803B470C[player][19].position[2]=100.0f;
    resource=func_800B24EC(D_803B89F0,&count,0,D_80140BDC-1,1);
    func_8008D870(D_803B470C[player][20].id,resource,-1);
    memcpy(D_803B470C[player][20].matrix,D_8011418C,36);
    func_800B5898(-1.57079632679f,D_803B470C[player][20].matrix);
    D_803B470C[player][20].position[0]=-75.1f;
    D_803B470C[player][20].position[1]=D_8014A108>=2?50.2f:55.2f;
    D_803B470C[player][20].position[0]*=scale;
    D_803B470C[player][20].position[1]*=scale;
    D_803B470C[player][20].position[2]=100.0f*scale;
    func_8008B32C(D_803B470C[player][20].matrix,D_803B470C[player][20].matrix,.5f);
    memcpy(D_803B470C[player][21].matrix,D_8011418C,36);
    func_800B5898(-1.57079632679f,D_803B470C[player][21].matrix);
    D_803B470C[player][21].position[0]=-87.1f;
    D_803B470C[player][21].position[1]=D_8014A108>=2?50.2f:55.2f;
    D_803B470C[player][21].position[0]*=scale;
    D_803B470C[player][21].position[1]*=scale;
    D_803B470C[player][21].position[2]=100.0f*scale;
    D_803BA868[player]=1;
    D_803BA888[player]=1;
    particle_velocity_set();
}

extern f32 fabsf(f32);
#pragma intrinsic (fabsf)
extern volatile f32 D_8002EB94;
extern volatile u8 D_80140BDC;
extern f32 D_8011418C[3][3];
extern f32 D_803B5D10,D_803B5D14,D_803B5D18,D_803B5D1C,D_803B5D20,D_803B5D24,D_803B5D28;
extern s32 D_803BA888[];
extern f32 D_803BA8A0[],D_803BA8F8[];
extern s32 D_803BA868[];
extern s8 D_803BAD78[],D_803BAD40[][14];

void func_80394DD8(s8 p)
{
    s32 buttonIdx;
    s32 i;
    s32 unusedA, unusedB; /* disclosed frame shaping, see header */
    f32 prevPos[3];
    s32 whichItem;
    f32 buttonY;
    f32 selectedPos;
    s8 clearScrollSpeed = 1;
    void *tex;
    s16 texIdx;
    u8 buttonColor[4] = {255,255,255,255};
    if (!D_803BA868[p]) return;
    if (D_803BACE0[p] == 1) {
        for (buttonIdx = 0; buttonIdx < 22; buttonIdx++) {
            if (buttonIdx == 16 || buttonIdx == 20 || buttonIdx == 21)
                model_transform_setup(D_803B470C[p][buttonIdx].id, 0, 1 << p);
            else model_data_load(D_803B470C[p][buttonIdx].id, 1, 15);
        }
        return;
    }
    if (D_803BACE0[p] == 3 || (D_803BACE0[p] == 4 && (D_803BADD4[p] == 1 || D_803BADCC[p] == 1))) {
        for (buttonIdx = 0; buttonIdx < 22; buttonIdx++) {
            if (buttonIdx == 16 || (buttonIdx == 20 || buttonIdx == 21) || buttonIdx == 18 || buttonIdx == 19)
                model_transform_setup(D_803B470C[p][buttonIdx].id, 0, 1 << p);
            else model_data_load(D_803B470C[p][buttonIdx].id, 1, 15);
        }
        return;
    }
    if (D_803BACE0[p] == 2 || (D_803BACE0[p] == 5 || D_803BACE0[p] == 6) || D_803BACE0[p] == 7) {
        for (buttonIdx = 0; buttonIdx < 22; buttonIdx++) model_data_load(D_803B470C[p][buttonIdx].id, 1, 15);
        return;
    }
    for (buttonIdx = 0; buttonIdx < 22; buttonIdx++) model_transform_setup(D_803B470C[p][buttonIdx].id, 0, 1 << p);
    whichItem = 0;
    if (D_803BACE0[p] == 0) {
        for (i = 0; i < D_803BAD80[p]; i++) if (D_803BAD40[p][i]) whichItem++;
    } else if (D_803BACE0[p] == 4) {
        for (i = 0; i < D_803BADC8[p]; i++) if (D_803BAD40[p][i]) whichItem++;
    }
    if (D_803B5D10 * ((D_803BAD78[p] - 1) * D_803B5D14) > D_803B5D1C - D_803B5D20)
        selectedPos = D_803B5D1C - whichItem * (D_803B5D1C - D_803B5D20) / (D_803BAD78[p] - 1);
    else selectedPos = D_803B5D1C - D_803B5D10 * (whichItem * D_803B5D14);
    memcpy(prevPos, D_803B470C[p][17].position, 12);
    D_803B470C[p][17].angle += 6.2831855f * D_8002EB94;
    if (D_803B470C[p][17].angle > 6.2831855f) D_803B470C[p][17].angle -= 6.2831855f;
    memcpy(D_803B470C[p][17].matrix, D_8011418C, 36);
    D_803B470C[p][17].position[0] = D_803B5D18 * D_803B5D28 / D_803B5D24 + -165.0f * D_803B5D10;
    D_803B470C[p][17].position[1] = selectedPos * D_803B5D28 / D_803B5D24;
    D_803B470C[p][17].position[2] = D_803B5D28;
    if (!D_803BA888[p]) {
        if (fabsf(D_803B470C[p][17].position[1] - prevPos[1]) * 60 * D_803B5D10 / 10 > D_803BA8A0[p]) {
            D_803BA8A0[p] = 60 * fabsf(D_803B470C[p][17].position[1] - prevPos[1]) * D_803B5D10 / 10;
            D_803BA8A0[p] = D_803BA8A0[p] < 540 * D_803B5D10 ? D_803BA8A0[p] : 540 * D_803B5D10;
            D_803BA8A0[p] = D_803BA8A0[p] > 180 * D_803B5D10 ? D_803BA8A0[p] : 180 * D_803B5D10;
        }
        if (D_803B470C[p][17].position[1] > prevPos[1] + D_8002EB94 * D_803BA8A0[p])
            D_803B470C[p][17].position[1] = prevPos[1] + D_8002EB94 * D_803BA8A0[p];
        else if (D_803B470C[p][17].position[1] < prevPos[1] - D_8002EB94 * D_803BA8A0[p])
            D_803B470C[p][17].position[1] = prevPos[1] - D_8002EB94 * D_803BA8A0[p];
        else D_803BA8A0[p] = 180 * D_803B5D10;
    }
    func_800B5940(D_803B470C[p][17].angle, D_803B470C[p][17].matrix);
    func_800B5898(1.5707964f, D_803B470C[p][17].matrix);
    func_8008B32C(D_803B470C[p][17].matrix, D_803B470C[p][17].matrix, D_803B5D10 * 0.4f);
    buttonY = selectedPos + D_803B5D10 * (whichItem * D_803B5D14);
    for (buttonIdx = 0; buttonIdx < 14; buttonIdx++) {
        if (((D_803BACE0[p] == 0 && buttonIdx == D_803BAD80[p]) ||
             (D_803BACE0[p] == 4 && buttonIdx == D_803BADC8[p])) && !D_803BADB8[p]) {
            if (D_803B470C[p][buttonIdx].angle < 3.1415927f) D_803B470C[p][buttonIdx].angle += 12.566371f * D_8002EB94;
            if (D_803B470C[p][buttonIdx].angle > 3.1415927f) D_803B470C[p][buttonIdx].angle = 3.1415927f;
            if (D_803BA888[p]) D_803B470C[p][buttonIdx].angle = 3.1415927f;
        } else {
            if (D_803B470C[p][buttonIdx].angle > 0) D_803B470C[p][buttonIdx].angle -= 12.566371f * D_8002EB94;
            if (D_803B470C[p][buttonIdx].angle < 0) D_803B470C[p][buttonIdx].angle = 0;
            if (D_803BA888[p]) D_803B470C[p][buttonIdx].angle = 0;
        }
        memcpy(prevPos, D_803B470C[p][buttonIdx].position, 12);
        if (D_803B470C[p][buttonIdx].unknown0 == 9) D_803B470C[p][buttonIdx].position[0] = D_803B5D18 + 0 * D_803B5D10;
        else if (D_803B470C[p][buttonIdx].unknown0 == 7) D_803B470C[p][buttonIdx].position[0] = D_803B5D18 + -80 * D_803B5D10;
        else D_803B470C[p][buttonIdx].position[0] = D_803B5D18 + -120 * D_803B5D10;
        D_803B470C[p][buttonIdx].position[1] = buttonY;
        D_803B470C[p][buttonIdx].position[2] = D_803B5D24;
        if (!D_803BA888[p]) {
            if (fabsf(D_803B470C[p][buttonIdx].position[1] - prevPos[1]) * 60 / 10 > D_803BA8F8[p]) {
                D_803BA8F8[p] = fabsf(D_803B470C[p][buttonIdx].position[1] - prevPos[1]) * 60 / 10;
                D_803BA8F8[p] = D_803BA8F8[p] < 540 * D_803B5D10 ? D_803BA8F8[p] : 540 * D_803B5D10;
                D_803BA8F8[p] = D_803BA8F8[p] > 180 * D_803B5D10 ? D_803BA8F8[p] : 180 * D_803B5D10;
            }
            if (D_803B470C[p][buttonIdx].position[1] > prevPos[1] + D_8002EB94 * D_803BA8F8[p]) {
                D_803B470C[p][buttonIdx].position[1] = prevPos[1] + D_8002EB94 * D_803BA8F8[p];
                clearScrollSpeed = 0;
            } else if (D_803B470C[p][buttonIdx].position[1] < prevPos[1] - D_8002EB94 * D_803BA8F8[p]) {
                D_803B470C[p][buttonIdx].position[1] = prevPos[1] - D_8002EB94 * D_803BA8F8[p];
                clearScrollSpeed = 0;
            }
        }
        func_8008B32C(D_8011418C, D_803B470C[p][buttonIdx].matrix, D_803B5D10);
        func_800B5898(D_803B470C[p][buttonIdx].angle - 1.5707964f, D_803B470C[p][buttonIdx].matrix);
        if (D_803B470C[p][buttonIdx].angle > 1.5707964f)
            tex = func_800B24EC("BUTTON_SELECT", &texIdx, 0, D_80140BDC - 1, 1);
        else tex = func_800B24EC("BUTTON", &texIdx, 0, D_80140BDC - 1, 1);
        func_8008D870(D_803B470C[p][buttonIdx].id, tex, -1);
        if (D_803B470C[p][buttonIdx].position[1] > D_803B5D1C + D_803B5D14 * D_803B5D10 ||
            D_803B470C[p][buttonIdx].position[1] < D_803B5D20 - D_803B5D14 * D_803B5D10) D_803B470C[p][buttonIdx].alpha = 0;
        else if (D_803B470C[p][buttonIdx].position[1] > D_803B5D1C)
            D_803B470C[p][buttonIdx].alpha = 255.f - 255.0f * (D_803B470C[p][buttonIdx].position[1] - D_803B5D1C) / (D_803B5D14 * D_803B5D10);
        else if (D_803B470C[p][buttonIdx].position[1] < D_803B5D20)
            D_803B470C[p][buttonIdx].alpha = 255.f - 255.0f * (D_803B5D20 - D_803B470C[p][buttonIdx].position[1]) / (D_803B5D14 * D_803B5D10);
        else D_803B470C[p][buttonIdx].alpha = 255;
        if (!D_803BAD40[p][buttonIdx]) D_803B470C[p][buttonIdx].alpha = 0;
        buttonColor[3] = D_803B470C[p][buttonIdx].alpha;
        func_8008E06C(D_803B470C[p][buttonIdx].id, buttonColor);
        if (!D_803BAD40[p][buttonIdx] || !D_803B470C[p][buttonIdx].alpha) model_data_load(D_803B470C[p][buttonIdx].id, 1, 15);
        else model_transform_setup(D_803B470C[p][buttonIdx].id, 0, 1 << p);
        if (buttonIdx == 1) {
            D_803B470C[p][15].position[0] = D_803B5D18 + 160 * D_803B5D10;
            D_803B470C[p][15].position[1] = D_803B470C[p][buttonIdx].position[1];
            D_803B470C[p][15].position[2] = D_803B470C[p][buttonIdx].position[2];
            D_803B470C[p][15].angle = D_803B470C[p][buttonIdx].angle;
            func_8008B32C(D_8011418C, D_803B470C[p][15].matrix, D_803B5D10);
            func_800B5898(D_803B470C[p][15].angle - 1.5707964f, D_803B470C[p][15].matrix);
            func_8008D870(D_803B470C[p][15].id, tex, -1);
            D_803B470C[p][15].alpha = D_803B470C[p][buttonIdx].alpha;
            func_8008E06C(D_803B470C[p][15].id, buttonColor);
            if (!D_803BAD40[p][buttonIdx] || !D_803B470C[p][buttonIdx].alpha) model_data_load(D_803B470C[p][15].id, 1, 15);
            else model_transform_setup(D_803B470C[p][15].id, 0, 1 << p);
        } else {
            if (D_803BACE0[p] == 0 && (D_803BAD80[p] == 0 || (D_803BAD80[p] >= 3 && D_803BAD80[p] < 11))) {
                if (buttonIdx == D_803BAD80[p]) {
                    D_803B470C[p][14].position[0] = 120 * D_803B5D10 + D_803B5D18;
                    D_803B470C[p][14].position[1] = D_803B470C[p][buttonIdx].position[1];
                    D_803B470C[p][14].position[2] = D_803B470C[p][buttonIdx].position[2];
                    if (D_803BADB8[p]) {
                        if (D_803B470C[p][14].angle < 3.1415927f) D_803B470C[p][14].angle += 12.566371f * D_8002EB94;
                        if (D_803B470C[p][14].angle > 3.1415927f) D_803B470C[p][14].angle = 3.1415927f;
                        if (D_803BA888[p]) D_803B470C[p][14].angle = 3.1415927f;
                    } else {
                        if (D_803B470C[p][14].angle > 0) D_803B470C[p][14].angle -= 12.566371f * D_8002EB94;
                        if (D_803B470C[p][14].angle < 0) D_803B470C[p][14].angle = 0;
                        if (D_803BA888[p]) D_803B470C[p][14].angle = 0;
                    }
                    func_8008B32C(D_8011418C, D_803B470C[p][14].matrix, D_803B5D10);
                    func_800B5898(D_803B470C[p][14].angle - 1.5707964f, D_803B470C[p][14].matrix);
                    if (D_803B470C[p][14].angle > 1.5707964f)
                        tex = func_800B24EC("BUTTON_SELECT", &texIdx, 0, D_80140BDC - 1, 1);
                    else tex = func_800B24EC("BUTTON", &texIdx, 0, D_80140BDC - 1, 1);
                    func_8008D870(D_803B470C[p][14].id, tex, -1);
                    D_803B470C[p][14].alpha = D_803B470C[p][buttonIdx].alpha;
                    func_8008E06C(D_803B470C[p][14].id, buttonColor);
                    if (!D_803BAD40[p][buttonIdx] || !D_803B470C[p][buttonIdx].alpha) {
                        D_803B470C[p][14].angle = 0;
                        model_data_load(D_803B470C[p][14].id, 1, 15);
                    } else model_transform_setup(D_803B470C[p][14].id, 0, 1 << p);
                }
            } else {
                D_803B470C[p][14].angle = 0;
                model_data_load(D_803B470C[p][14].id, 1, 15);
            }
        }
        if (D_803BAD40[p][buttonIdx] == 1) buttonY -= D_803B5D14 * D_803B5D10;
    }
    if (clearScrollSpeed == 1) D_803BA8F8[p] = 0;
    D_803BA888[p] = 0;
}

extern u32 D_801174BC,D_801174B4;
extern s8 D_8015418C[],D_80157244;
extern void player_mode_set(s32,s32),random_seed_init(s32,s32);
extern void net_state_validate(void),func_800B5570(s32);
extern s8 func_800A361C(s32);
extern void resource_type_select(u32),visual_objects_update(s32);
extern void object_render_cleanup(Owner *),func_800D91A0(void);
extern s32 func_800CCEFC(Owner *);
extern void func_800B4DA4(Owner *);
extern void func_800BE6A4(u8 *,u8 *);
extern void func_800CD058(Owner *);
extern struct PlayerFiles { u32 unknown0,unknown4; Owner *head,*tail; } D_8012E6D8;
extern s8 D_803BADC8[],D_803BADCC[],D_803BADD0[],D_803BADD4[];
extern u8 D_803BADD8[][13];
extern s32 D_803BA888[];
extern s8 D_803BAD00[][14];
/* playerClaimed: installed body of group runtime_a_profile_menu_20261006 (IPA context: owner in a0, skip in a2) */
s32 func_80395E24(s32 skip,Owner *owner)
{
    s8 i;
    s32 found=0;
    if(owner)for(i=0;i<D_8014A108;i++)if(i!=skip && owner==D_8014A118[i].owner)found=1;
    return found;
}
extern void audio_doppler(u32),audio_distance_atten(u32);
extern void func_800F084C(char *);
extern Owner *func_800CD104(s8,char *);
extern char *D_80117420;

/* Drop one navigation step to the next/previous profile on the same pak (inlined statics in the original; written out). */
void func_80395E8C(s32 p)
{
    s8 origSelect;
    s32 pak;
    s32 pIdx;
    Owner *player;
    pak = D_803BADBC[p];
    origSelect = D_803BADC8[p];
    if (D_803BAC20[pak] <= 0) {
        func_80394B00(p, 0);
        return;
    }
    while (!D_803BAD00[p][D_803BADC8[p]] || func_80395E24(p, D_803BAC80[p]) == 1) {
        D_803BADC8[p]++;
        if (D_803BADC8[p] > 13) D_803BADC8[p] = 0;
        if (D_803BADC8[p] == origSelect) {
            func_80394B00(p, 0);
            return;
        }
        if (D_803BADC8[p] >= 3 && D_803BADC8[p] < 11 && D_803BAD00[p][D_803BADC8[p]]) {
            if (D_803BAC80[p]) {
                do { D_803BAC80[p] = D_803BAC80[p]->body->next; }
                while (D_803BAC80[p] && D_803BAC80[p]->body->selection->item->player != D_803BADBC[p]);
            }
            if (!D_803BAC80[p]) {
                D_803BAC80[p] = D_8012E6D8.head;
                while (D_803BAC80[p] && D_803BAC80[p]->body->selection->item->player != D_803BADBC[p])
                    D_803BAC80[p] = D_803BAC80[p]->body->next;
            }
        } else D_803BAC80[p] = 0;
        D_803BA888[p] = 1;
    }
    if (D_8014A118[p].flags & 1) {
        resource_type_select(D_8014A118[p].flags);
        if (D_803BADD4[p] == 1) {
            D_803BADD4[p] = 0;
            func_80394B00(p, 0);
        } else if (D_803BADCC[p] == 1) {
            D_803BADCC[p] = 0;
            D_803BADD0[p] = 0;
        } else func_80394B00(p, 0);
    } else if (D_8014A118[p].flags & 2) {
        if (D_803BADD4[p] == 1) {
            resource_type_select(D_8014A118[p].flags);
            D_803BADD4[p] = 0;
            func_80394B00(p, 0);
        } else if (D_803BADCC[p] == 1) {
            if (D_803BADD0[p] == 1) {
                resource_type_select(D_8014A118[p].flags);
                func_800CD058(D_803BAC80[p]);
                for (player = D_8012E6D8.head; player; player = player->body->next)
                    if (D_803BAC80[p] == player) break;
                if (!player) {
                    for (pIdx = 0; pIdx < D_8014A108; pIdx++) {
                        if (pIdx != p && D_803BADBC[pIdx] == D_803BADBC[p] &&
                            D_803BAD80[pIdx] >= 3 && D_803BAD80[pIdx] < 11) {
                            D_803BAD80[pIdx] = 1;
                            D_803BAC80[pIdx] = 0;
                        }
                    }
                    D_803BAC80[p] = 0;
                    func_80394834();
                    func_80394564(-1);
                    D_803BADC8[p] = 0;
                    D_803BADD4[p] = 1;
                }
            } else resource_type_select(4);
            D_803BADCC[p] = 0;
            D_803BADD0[p] = 0;
        } else if (D_803BADC8[p] >= 3 && D_803BADC8[p] < 11) {
            resource_type_select(D_8014A118[p].flags);
            func_800BE6A4(D_803BADD8[p], D_803BAC80[p]->body->name);
            D_803BADCC[p] = 1;
            D_803BADD0[p] = 0;
        }
    } else if (D_8014A118[p].flags & 4) {
        resource_type_select(D_8014A118[p].flags);
        if (D_803BADD4[p] == 1) {
            D_803BADD4[p] = 0;
            func_80394B00(p, 0);
        } else if (D_803BADCC[p] == 1) {
            D_803BADCC[p] = 0;
            D_803BADD0[p] = 0;
        } else func_80394B00(p, 0);
    }
    if (D_8014A118[p].autoFlags & 0x2000) {
        if (D_803BADD4[p] == 1) return;
        if (D_803BADCC[p] == 1) {
            audio_doppler(D_8014A118[p].flags);
            D_803BADD0[p] = !D_803BADD0[p];
        } else if (D_803BADC8[p] == 1) {
            D_803BADBC[p]++;
            if (D_803BADBC[p] > 3) D_803BADBC[p] = 0;
            func_80394564(p);
            audio_doppler(D_8014A118[p].flags);
        }
    } else if (D_8014A118[p].autoFlags & 0x1000) {
        if (D_803BADD4[p] == 1) return;
        if (D_803BADCC[p] == 1) {
            audio_doppler(D_8014A118[p].flags);
            D_803BADD0[p] = !D_803BADD0[p];
        } else if (D_803BADC8[p] == 1) {
            D_803BADBC[p]--;
            if (D_803BADBC[p] < 0) D_803BADBC[p] = 3;
            func_80394564(p);
            audio_doppler(D_8014A118[p].flags);
        }
    } else if (D_8014A118[p].autoFlags & 0x400) {
        if (D_803BADD4[p] == 1) return;
        if (D_803BADCC[p] == 1) return;
        audio_distance_atten(D_8014A118[p].flags);
        do {
            if (D_803BADC8[p] == 3 && D_803BAD00[p][D_803BADC8[p]]) {
                if (D_803BAC80[p]) {
                    do { D_803BAC80[p] = D_803BAC80[p]->body->previous; }
                    while (D_803BAC80[p] && D_803BAC80[p]->body->selection->item->player != D_803BADBC[p]);
                }
                if (D_803BAC80[p]) break;
            }
            D_803BADC8[p]--;
            if (D_803BADC8[p] < 0) D_803BADC8[p] = 13;
            if (D_803BADC8[p] >= 3 && D_803BADC8[p] < 11 && D_803BAD00[p][D_803BADC8[p]]) {
                if (D_803BAC80[p]) {
                    do { D_803BAC80[p] = D_803BAC80[p]->body->previous; }
                    while (D_803BAC80[p] && D_803BAC80[p]->body->selection->item->player != D_803BADBC[p]);
                }
                if (!D_803BAC80[p]) {
                    D_803BAC80[p] = D_8012E6D8.tail;
                    while (D_803BAC80[p] && D_803BAC80[p]->body->selection->item->player != D_803BADBC[p])
                        D_803BAC80[p] = D_803BAC80[p]->body->previous;
                }
            } else D_803BAC80[p] = 0;
        } while (!D_803BAD00[p][D_803BADC8[p]] || func_80395E24(p, D_803BAC80[p]) == 1);
    } else if (D_8014A118[p].autoFlags & 0x800) {
        if (D_803BADD4[p] == 1) return;
        if (D_803BADCC[p] == 1) return;
        audio_distance_atten(D_8014A118[p].flags);
        do {
            if (D_803BADC8[p] == 10 && D_803BAD00[p][D_803BADC8[p]]) {
                if (D_803BAC80[p]) {
                    do { D_803BAC80[p] = D_803BAC80[p]->body->next; }
                    while (D_803BAC80[p] && D_803BAC80[p]->body->selection->item->player != D_803BADBC[p]);
                }
                if (D_803BAC80[p]) break;
            }
            D_803BADC8[p]++;
            if (D_803BADC8[p] > 13) D_803BADC8[p] = 0;
            if (D_803BADC8[p] >= 3 && D_803BADC8[p] < 11 && D_803BAD00[p][D_803BADC8[p]]) {
                if (D_803BAC80[p]) {
                    do { D_803BAC80[p] = D_803BAC80[p]->body->next; }
                    while (D_803BAC80[p] && D_803BAC80[p]->body->selection->item->player != D_803BADBC[p]);
                }
                if (!D_803BAC80[p]) {
                    D_803BAC80[p] = D_8012E6D8.head;
                    while (D_803BAC80[p] && D_803BAC80[p]->body->selection->item->player != D_803BADBC[p])
                        D_803BAC80[p] = D_803BAC80[p]->body->next;
                }
            } else D_803BAC80[p] = 0;
        } while (!D_803BAD00[p][D_803BADC8[p]] || func_80395E24(p, D_803BAC80[p]) == 1);
    }
}

/* playerBeingDeleted / playerBeingCreated: installed bodies (cloud/matches/ovl_a), IPA context */
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
void func_80396EE0(s32,Owner *);
void func_80396FA0(s32 p)
{
    while (!D_803BAD00[p][D_803BAD80[p]] ||
           func_80395E24(p, D_803BAC80[p]) == 1 ||
           (D_803BAD80[p] == 11 && func_80396F44(p) == 1) ||
           (D_803BAD80[p] == 12 && func_80396EE8(p) == 1)) {
        D_803BADB8[p] = 0;
        D_803BAD80[p]--;
        if (D_803BAD80[p] < 0) D_803BAD80[p] = 13;
        if (D_803BAD80[p] >= 3 && D_803BAD80[p] < 11 && D_803BAD00[p][D_803BAD80[p]]) {
            if (D_803BAC80[p]) {
                do {
                    D_803BAC80[p] = D_803BAC80[p]->body->previous;
                } while (D_803BAC80[p] && D_803BAC80[p]->body->selection->item->player != D_803BADBC[p]);
            }
            if (!D_803BAC80[p]) {
                D_803BAC80[p] = D_8012E6D8.tail;
                while (D_803BAC80[p] && D_803BAC80[p]->body->selection->item->player != D_803BADBC[p])
                    D_803BAC80[p] = D_803BAC80[p]->body->previous;
            }
        } else D_803BAC80[p] = 0;
    }
    if (D_8014A118[p].flags & 1) {
        resource_type_select(D_8014A118[p].flags);
        if (D_803BAD80[p] >= 3 && D_803BAD80[p] < 11)
            func_80396EE0(p, D_803BAC80[p]);
        else
            func_80396EE0(p, &D_80146150[p]);
        func_80394B00(p, 1);
    } else if (D_8014A118[p].flags & 2) {
        resource_type_select(D_8014A118[p].flags);
        if (D_803BAD80[p] >= 3 && D_803BAD80[p] < 11) {
            func_80396EE0(p, D_803BAC80[p]);
            if (D_803BADB8[p] == 1) {
                if (func_800CCEFC(D_8014A118[p].owner)) {
                    D_8015418C[p] = 1;
                    func_800A3424((void **)D_8014A118[p].owner->body->selection, 1);
                    func_80394B00(p, 2);
                }
            } else func_80394B00(p, 1);
        } else switch (D_803BAD80[p]) {
        case 0:
            func_80396EE0(p, &D_80146150[p]);
            if (D_803BADB8[p] == 1) {
                D_8015418C[p] = 1;
                func_800A3424((void **)D_8014A118[p].owner->body->selection, 1);
                func_80394B00(p, 2);
            } else func_80394B00(p, 1);
            break;
        case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10:
            break;
        case 11: func_80394B00(p, 3); break;
        case 12: D_803BADC8[p] = 0; func_80394B00(p, 4); break;
        case 13: func_80394B00(p, 6); break;
        }
    } else if (D_8014A118[p].flags & 4) {
        resource_type_select(D_8014A118[p].flags);
        D_803B5ED4 = 2;
    }
    do {} while (0); /* SHAPING: compiled-out block boundary, see header */
    if (D_8014A118[p].autoFlags & 0x2000) {
        if (D_803BAD80[p] == 0 || (D_803BAD80[p] >= 3 && D_803BAD80[p] < 11)) {
            D_803BADB8[p] = !D_803BADB8[p];
            audio_doppler(D_8014A118[p].flags);
        } else if (D_803BAD80[p] == 1) {
            D_803BADBC[p]++;
            if (D_803BADBC[p] >= 4) D_803BADBC[p] = 0;
            func_80394564(p);
            D_803BA888[p] = 1;
            audio_doppler(D_8014A118[p].flags);
        }
    } else if (D_8014A118[p].autoFlags & 0x1000) {
        if (D_803BAD80[p] == 0 || (D_803BAD80[p] >= 3 && D_803BAD80[p] < 11)) {
            D_803BADB8[p] = !D_803BADB8[p];
            audio_doppler(D_8014A118[p].flags);
        } else if (D_803BAD80[p] == 1) {
            D_803BADBC[p]--;
            if (D_803BADBC[p] < 0) D_803BADBC[p] = 3;
            func_80394564(p);
            D_803BA888[p] = 1;
            audio_doppler(D_8014A118[p].flags);
        }
    } else if (D_8014A118[p].autoFlags & 0x400) {
        audio_distance_atten(D_8014A118[p].flags);
        D_803BADB8[p] = 0;
        do {
            if (D_803BAD80[p] == 3 && D_803BAD00[p][D_803BAD80[p]]) {
                if (D_803BAC80[p]) {
                    do {
                        D_803BAC80[p] = D_803BAC80[p]->body->previous;
                    } while (D_803BAC80[p] && D_803BAC80[p]->body->selection->item->player != D_803BADBC[p]);
                }
                if (D_803BAC80[p]) break;
            }
            D_803BAD80[p]--;
            if (D_803BAD80[p] < 0) D_803BAD80[p] = 13;
            if (D_803BAD80[p] >= 3 && D_803BAD80[p] < 11 && D_803BAD00[p][D_803BAD80[p]]) {
                if (D_803BAC80[p]) {
                    do {
                        D_803BAC80[p] = D_803BAC80[p]->body->previous;
                    } while (D_803BAC80[p] && D_803BAC80[p]->body->selection->item->player != D_803BADBC[p]);
                }
                if (!D_803BAC80[p]) {
                    D_803BAC80[p] = D_8012E6D8.tail;
                    while (D_803BAC80[p] && D_803BAC80[p]->body->selection->item->player != D_803BADBC[p])
                        D_803BAC80[p] = D_803BAC80[p]->body->previous;
                }
            } else D_803BAC80[p] = 0;
        } while (!D_803BAD00[p][D_803BAD80[p]] || func_80395E24(p, D_803BAC80[p]) == 1 ||
                 (D_803BAD80[p] == 11 && func_80396F44(p) == 1) ||
                 (D_803BAD80[p] == 12 && func_80396EE8(p) == 1));
    } else if (D_8014A118[p].autoFlags & 0x800) {
        audio_distance_atten(D_8014A118[p].flags);
        D_803BADB8[p] = 0;
        do {
            if (D_803BAD80[p] == 10 && D_803BAD00[p][D_803BAD80[p]]) {
                if (D_803BAC80[p]) {
                    do {
                        D_803BAC80[p] = D_803BAC80[p]->body->next;
                    } while (D_803BAC80[p] && D_803BAC80[p]->body->selection->item->player != D_803BADBC[p]);
                }
                if (D_803BAC80[p]) break;
            }
            D_803BAD80[p]++;
            if (D_803BAD80[p] >= 14) D_803BAD80[p] = 0;
            if (D_803BAD80[p] >= 3 && D_803BAD80[p] < 11 && D_803BAD00[p][D_803BAD80[p]]) {
                if (D_803BAC80[p]) {
                    do {
                        D_803BAC80[p] = D_803BAC80[p]->body->next;
                    } while (D_803BAC80[p] && D_803BAC80[p]->body->selection->item->player != D_803BADBC[p]);
                }
                if (!D_803BAC80[p]) {
                    D_803BAC80[p] = D_8012E6D8.head;
                    while (D_803BAC80[p] && D_803BAC80[p]->body->selection->item->player != D_803BADBC[p])
                        D_803BAC80[p] = D_803BAC80[p]->body->next;
                }
            } else D_803BAC80[p] = 0;
        } while (!D_803BAD00[p][D_803BAD80[p]] || func_80395E24(p, D_803BAC80[p]) == 1 ||
                 (D_803BAD80[p] == 11 && func_80396F44(p) == 1) ||
                 (D_803BAD80[p] == 12 && func_80396EE8(p) == 1));
    }
}
void func_80396EE0(s32 playerIdx, Owner *player)
{
    D_8014A118[playerIdx].owner = player;
    func_80394564(-1);
}

void func_803968C0(s32 p)
{
    Owner *player;
    s32 pIdx;
    if (D_803BAC20[D_803BADBC[p]] < 0 || D_803BAC68[D_803BADBC[p]])
        func_80394B00(p, 0);
    if (D_8014A118[p].autoFlags & 0x400) {
        audio_distance_atten(D_8014A118[p].autoFlags);
        if (D_803BACD0[p] == -1) D_803BACD0[p] = 52;
        else if (D_803BACD0[p] == -2) D_803BACD0[p] = 49;
        else if (D_803BACD0[p] == -3) D_803BACD0[p] = 46;
        else if (D_803BACD0[p] < 3) D_803BACD0[p] = -3;
        else if (D_803BACD0[p] < 6) D_803BACD0[p] = -2;
        else if (D_803BACD0[p] < 9) D_803BACD0[p] = -1;
        else D_803BACD0[p] -= 9;
    } else if (D_8014A118[p].autoFlags & 0x800) {
        audio_distance_atten(D_8014A118[p].autoFlags);
        if (D_803BACD0[p] == -1) D_803BACD0[p] = 7;
        else if (D_803BACD0[p] == -2) D_803BACD0[p] = 4;
        else if (D_803BACD0[p] == -3) D_803BACD0[p] = 1;
        else if (D_803BACD0[p] < 45) D_803BACD0[p] += 9;
        else if (D_803BACD0[p] < 48) D_803BACD0[p] = -3;
        else if (D_803BACD0[p] < 51) D_803BACD0[p] = -2;
        else D_803BACD0[p] = -1;
    } else if (D_8014A118[p].autoFlags & 0x1000) {
        audio_doppler(D_8014A118[p].autoFlags);
        if (D_803BACD0[p] == -1) D_803BACD0[p] = -2;
        else if (D_803BACD0[p] == -2) D_803BACD0[p] = -3;
        else if (D_803BACD0[p] == -3) D_803BACD0[p] = -1;
        else if (D_803BACD0[p] % 9 == 0) D_803BACD0[p] += 8;
        else D_803BACD0[p]--;
    } else if (D_8014A118[p].autoFlags & 0x2000) {
        audio_doppler(D_8014A118[p].autoFlags);
        if (D_803BACD0[p] == -1) D_803BACD0[p] = -3;
        else if (D_803BACD0[p] == -2) D_803BACD0[p] = -1;
        else if (D_803BACD0[p] == -3) D_803BACD0[p] = -2;
        else if (D_803BACD0[p] % 9 == 8) D_803BACD0[p] -= 8;
        else D_803BACD0[p]++;
    }
    if (D_8014A118[p].flags & 3) {
        resource_type_select(D_8014A118[p].flags);
        if (D_8014A118[p].flags & 1) D_803BACD0[p] = -1;
        if (D_803BACD0[p] == -2) {
            if (D_803BACD8[p] > 0) {
                D_803BACD8[p]--;
                D_803BAC98[p][D_803BACD8[p]] = 0;
            }
        } else if (D_803BACD0[p] == -1) {
            while (D_803BAC98[p][D_803BACD8[p] - 1] == ' ') {
                D_803BACD8[p]--;
                D_803BAC98[p][D_803BACD8[p]] = 0;
            }
            func_800F084C(D_803BAC98[p]);
            if (D_803BAC98[p][0]) {
                player = func_800CD104(D_803BADBC[p], D_803BAC98[p]);
                if (player) {
                    func_80396EE0(p, player);
                    D_803BAD80[p] = 3;
                    D_803BAC80[p] = player;
                    for (pIdx = 0; pIdx < D_8014A108; pIdx++) {
                        if (pIdx != p && D_803BADBC[pIdx] == D_803BADBC[p] &&
                            D_803BAD80[pIdx] >= 3 && D_803BAD80[pIdx] < 11) {
                            D_803BAD80[pIdx] = 1;
                            D_803BAC80[pIdx] = 0;
                        }
                    }
                }
                func_80394834();
                func_80394564(-1);
            }
            func_80394B00(p, 0);
        } else if (D_803BACD8[p] < 12 && (D_803BACD0[p] != -3 || D_803BACD8[p] > 0)) {
            if (D_803BACD0[p] == -3)
                D_803BAC98[p][D_803BACD8[p]] = ' ';
            else
                D_803BAC98[p][D_803BACD8[p]] = D_80117420[D_803BACD0[p]];
            D_803BACD8[p]++;
            D_803BAC98[p][D_803BACD8[p]] = 0;
            if (D_803BACD8[p] == 12) D_803BACD0[p] = -1;
        }
    } else if (D_8014A118[p].flags & 4) {
        resource_type_select(D_8014A118[p].flags);
        if (D_803BACD8[p] > 0) {
            D_803BACD8[p]--;
            D_803BAC98[p][D_803BACD8[p]] = 0;
        } else func_80394B00(p, 0);
    }
}

void func_8039842C(void)
{
    s32 playerIdx;
    s32 p;
    s32 u0,u1,u2,u3,u4,u5,u6,u7;
    while (1) {
        if (D_801174BC != 1) {
            if (D_801174BC != D_801174B4) {
                s32 i;
                Owner *player;
                for (i = 0; i < D_8014A108; i++) {
                    if (D_8014A118[i].owner != &D_80146150[i]) {
                        for (player = D_8012E6E0; player; player = player->body->next)
                            if (D_8014A118[i].owner == player) break;
                        if (!player) {
                            D_801174BC = 1;
                            goto restart;
                        }
                    }
                    if (i > 1) player_mode_set(i, 1);
                    random_seed_init(i, 1);
                }
                for (; i < 4; i++)
                    if (i > 1) player_mode_set(i, 0);
                net_state_validate();
                func_800B5570(64);
                return;
            }
            D_801174BC = 1;
        }
        break;
restart:;
    }
    if (!D_803B5ED0) func_80397FFC();
    do {} while (0); /* SHAPING (context): compiled-out block boundary, see header */
    for (playerIdx = 0; playerIdx < D_8014A108; playerIdx++) {
        switch (D_803BACE0[playerIdx]) {
        case 0:
            func_80396FA0(playerIdx);
            break;
        case 1:
            if (!D_8014A118[playerIdx].owner ||
                (D_8014A118[playerIdx].owner->body->selection &&
                 func_800A361C(D_8014A118[playerIdx].owner->body->selection->item->player)) ||
                (D_8014A118[playerIdx].flags & 4)) {
                if (D_8014A118[playerIdx].flags & 4) resource_type_select(D_8014A118[playerIdx].flags);
                func_80394564(-1);
                func_80394B00(playerIdx, 0);
            }
            break;
        case 2:
            if (!D_8015418C[playerIdx]) {
                visual_objects_update(1);
                func_800A3424((void **)D_8014A118[playerIdx].owner->body->selection, 0);
                object_render_cleanup(D_8014A118[playerIdx].owner);
                func_80394564(-1);
                func_80394B00(playerIdx, 0);
                func_80394DD8(playerIdx);
            }
            break;
        case 3:
            func_803968C0(playerIdx);
            break;
        case 4:
            func_80395E8C(playerIdx);
            break;
        case 5:
            if (D_8014A118[playerIdx].flags & 7) {
                resource_type_select(D_8014A118[playerIdx].flags);
                func_80394B00(playerIdx, 0);
            }
            break;
        case 6:
            if (D_8014A118[playerIdx].flags & 7) {
                resource_type_select(D_8014A118[playerIdx].flags);
                func_80394B00(playerIdx, 0);
            }
            break;
        default:
            break;
        }
        func_80394DD8(playerIdx);
    }
    func_800D91A0();
    {
        s8 allReady = 1;
        for (playerIdx = 0; playerIdx < D_8014A108; playerIdx++)
            if (D_803BACE0[playerIdx] != 1) allReady = 0;
        if (allReady == 1) D_803B5ED4 = 1;
    }
    if (D_80157244) {
        func_803949CC();
        func_800B5570(4);
    } else if (D_803B5ED4) {
        switch (D_803B5ED4) {
        case 1: {
            Owner *player;
            for (player = D_8012E6E0; player; player = player->body->next) {
                for (p = 0; p < D_8014A108; p++)
                    if (player == D_8014A118[p].owner) break;
                if (p >= D_8014A108) object_render_cleanup(player);
            }
            for (p = 0; p < D_8014A108; p++) {
                if (!func_800CCEFC(D_8014A118[p].owner)) break;
                func_800B4DA4(D_8014A118[p].owner);
                func_800A3424((void **)D_8014A118[p].owner->body->selection, 1);
                if (p > 1) player_mode_set(p, 1);
                random_seed_init(p, 1);
            }
            if (p < D_8014A108) {
                s32 i;
                for (i = 0; i < D_8014A108; i++) {
                    D_803BAD80[i] = 3;
                    func_80394B00(i, 0);
                }
                break;
            }
            for (; p < 4; p++)
                if (p > 1) player_mode_set(p, 0);
            net_state_validate();
            func_803949CC();
            func_800B5570(64);
            break;
        }
        case 2: {
            s32 i;
            func_803949CC();
            func_800B5570(16);
            for (i = 0; i < D_8014A108; i++) D_8014A118[i].owner = 0;
            break;
        }
        }
        D_803B5ED4 = 0;
    }
    func_80394834();
    func_80394564(-1);
}
