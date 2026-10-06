/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
    s32 i,j;
    if (player == -1) {first = 0;last = D_8014A108 - 1;}
    for (i = first; i <= last; i++) {
        D_803BADC0[i] = D_803BAC20[D_803BADBC[i]];
        D_803BAD00[i][0] = 1;
        D_803BAD00[i][1] = 1;
        D_803BAD00[i][2] = 0;
        for (j = 0; j < 8; j++)
            D_803BAD00[i][j + 3] = j < D_803BADC0[i];
        D_803BAD00[i][11] = D_803BADC0[i] >= 0 && !D_803BAC68[D_803BADBC[i]];
        D_803BAD00[i][12] = D_803BADC0[i] > 0;
        D_803BAD00[i][13] = 0;
        if (D_803BACE0[i] == 4) {
            D_803BAD00[i][0] = 0;
            D_803BAD00[i][1] = 0;
            D_803BAD00[i][11] = 0;
            D_803BAD00[i][12] = 0;
        }
        D_803BAD40[i][0] = D_803BAD00[i][0];
        D_803BAD40[i][1] = D_803BACE0[i] == 4 || D_803BAD00[i][1];
        D_803BAD40[i][2] = D_803BADC0[i] < 1 || (D_803BAC68[D_803BADBC[i]] && D_803BACE0[i] != 4);
        for (j = 0; j < 8; j++) D_803BAD40[i][j + 3] = D_803BAD00[i][j + 3];
        D_803BAD40[i][11] = D_803BAD00[i][11];
        D_803BAD40[i][12] = D_803BAD00[i][12];
        D_803BAD40[i][13] = D_803BAD00[i][13];
        D_803BAD78[i] = 0;
        for (j = 0; j < 14; j++) if (D_803BAD40[i][j]) D_803BAD78[i]++;
    }
}

typedef struct Owner Owner;
typedef struct Item {u8 other[16];u8 player;} Item;
typedef struct Selection {Item *item;} Selection;
typedef struct OwnerBody {Owner *next,*previous;Selection *selection;} OwnerBody;
struct Owner {OwnerBody *body;};
typedef struct Player {u8 unaccessed0,id;u8 unaccessed2[70];Owner *owner;} Player;
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
extern void func_800A3424(void **,s8);
extern void func_80394834(void);
extern void func_80394B00(s32,s32);
extern void *object_create(s32);
extern s16 object_bytes_sum_global(void);
extern void func_80397AD8(s32);
extern void func_800A6404(void);
extern void particle_lifetime_set(void);
extern FiveParts *ambient_sound_set(s32,s32,s32,s32,s32,s32,s32,s32);
extern Blit *sound_control(s16,s16,const MultiBlit *,s16);
extern void entity_audio_update(void);

void func_80397FFC(void)
{
    s32 i,height;
    for (i = 0; i < D_8014A108; i++) {
        if (D_8014A118[i].owner) func_800A3424((void **)D_8014A118[i].owner->body->selection,0);
    }
    func_80394834();
    for (i = 0; i < D_8014A108; i++) {
        D_8012E67C[i] = i;
        D_8014A118[i].id = i;
        D_803BAD80[i] = 3;
        func_80394B00(i,0);
        height = D_803B5CFC[D_8014A108][0];
        if (D_8014A108 >= 2) object_create(11);
        else object_create(13);
        height -= object_bytes_sum_global();
        if (D_8014A108 >= 2) object_create(12);
        else object_create(10);
        D_803BACF4 = height / object_bytes_sum_global();
        func_80397AD8(i);
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
    func_800A6404();
    particle_lifetime_set();
    for (i = 0; i < D_8014A108; i++) D_803BAC08[i] = ambient_sound_set(-20,0,-10,10,192,0,0,0);
    D_803B5ED8 = sound_control(0,0,D_803B5D2C,1);
    D_803B5ED4 = 0;
    entity_audio_update();
    D_803B5ED0 = 1;
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
        while (node && node != D_8014A118[player].owner) node = node->body->next;
        D_8014A118[player].owner = 0;
        if (!node || !node->body->selection) {
            D_803BAC80[player] = &D_80146150[player];
            if (D_803BACE0[player] != 3 && D_803BACE0[player] != 4)
                D_803BADBC[player] = D_8014A118[player].id;
            if (D_803BAD80[player] >= 3 && D_803BAD80[player] < 11)
                D_803BAD80[player] = 0;
        } else {
            D_803BAC80[player] = node;
            D_803BADBC[player] = node->body->selection->item->player;
            if (D_803BAD80[player] >= 3 && D_803BAD80[player] < 11) {
                D_803BAD80[player] = 3;
                for (other = D_8012E6E0; other && other != node; other = other->body->next) {
                    if (other->body->selection->item->player == node->body->selection->item->player)
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
