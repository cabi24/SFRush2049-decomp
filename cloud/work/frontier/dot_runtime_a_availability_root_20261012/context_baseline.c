/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* NONMATCH image-A 97FFC availability setup research. No installation claims.
 * The complete real 9842C outer caller makes this an internal procedure; the
 * complete real 97AD8 model-slot callee recovers its native stack-passed input
 * and clobber set. Four already-exact bodies are preservation diagnostics.
 * A consumed s32 screen-count local preserves the post-store narrow read; it
 * restores the 72-byte frame and all spill homes without unused locals or
 * changing storage capacity. Separate loop statement lines restore scheduling.
 * Both root float literals are authenticated and checked in full. All source
 * spellings/names are reconstructions, not assertions of original source.
 * 97FFC still differs in 15 instruction words. Its body is not installable
 * separately from the private ABI context; 9842C and 97AD8 remain NONMATCH.
 * No proven arcade ancestor, forced compiler, fake caller/formal or pad array.
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
typedef struct OwnerBody {Owner *next,*previous;Selection *selection;} OwnerBody;
struct Owner {OwnerBody *body;};
typedef struct Player {u8 unaccessed0,id;u16 unaccessed2;u32 flags;u8 unaccessed8[64];Owner *owner;} Player;
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
extern void func_80397AD8(s8);
extern void func_800A6404(void);
extern void particle_lifetime_set(void);
extern FiveParts *ambient_sound_set(s32,s32,s32,s32,s32,s32,s32,s32);
extern Blit *sound_control(s16,s16,const MultiBlit *,s16);
extern void entity_audio_update(void);

void func_80397FFC(void)
{
    s32 i,height,screens;
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
    screens = D_80151AD0;
    if (screens == 3) D_80151AD0 = 4;
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
    for (i = 0; i < D_8014A108; i++) {
        D_803BAC08[i] = ambient_sound_set(-20,0,-10,10,192,0,0,0);
    }
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
    s32 unknown3C;
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

extern u32 D_801174BC,D_801174B4;
extern s8 D_8015418C[],D_80157244;
extern void player_mode_set(s32,s32),random_seed_init(s32,s32);
extern void net_state_validate(void),func_800B5570(s32);
extern s8 func_800A361C(s32);
extern void resource_type_select(u32),visual_objects_update(s32);
extern void object_render_cleanup(Owner *),func_800D91A0(void);
extern s32 func_800CCEFC(Owner *);
extern void func_800B4DA4(Owner *);
extern void func_80396FA0(s32),func_80394DD8(s8),func_803968C0(s32),func_80395E8C(s32);

void func_8039842C(void)
{
    s32 i,j,all;
    Owner *node;
restart:
    if(D_801174BC!=1) {
        if(D_801174BC!=D_801174B4) {
            for(i=0;i<D_8014A108;i++) {
                if(D_8014A118[i].owner!=&D_80146150[i]) {
                    for(node=D_8012E6E0;node && node!=D_8014A118[i].owner;node=node->body->next) {}
                    if(!node) {
                        D_801174BC=1;
                        goto restart;
                    }
                }
                if(i>=2)player_mode_set(i,1);
                random_seed_init(i,1);
            }
            for(;i<4;i++)if(i>=2)player_mode_set(i,0);
            net_state_validate();
            func_800B5570(64);
            return;
        }
        D_801174BC=1;
    }
    if(!D_803B5ED0)func_80397FFC();
    for(i=0;i<D_8014A108;i++) {
        switch(D_803BACE0[i]) {
        case 0:
            func_80396FA0(i);
            break;
        case 1:
            if(!D_8014A118[i].owner ||
               (D_8014A118[i].owner->body->selection && func_800A361C(D_8014A118[i].owner->body->selection->item->player)) ||
               (D_8014A118[i].flags&4)) {
                if(D_8014A118[i].flags&4)resource_type_select(D_8014A118[i].flags);
                func_80394564(-1);
                func_80394B00(i,0);
            }
            break;
        case 2:
            if(!D_8015418C[i]) {
                visual_objects_update(1);
                func_800A3424((void **)D_8014A118[i].owner->body->selection,0);
                object_render_cleanup(D_8014A118[i].owner);
                func_80394564(-1);
                func_80394B00(i,0);
                func_80394DD8(i);
            }
            break;
        case 3:
            func_803968C0(i);
            break;
        case 4:
            func_80395E8C(i);
            break;
        case 5:
            if(D_8014A118[i].flags&7) {
                resource_type_select(D_8014A118[i].flags);
                func_80394B00(i,0);
            }
            break;
        case 6:
            if(D_8014A118[i].flags&7) {
                resource_type_select(D_8014A118[i].flags);
                func_80394B00(i,0);
            }
            break;
        }
        func_80394DD8(i);
    }
    func_800D91A0();
    all=1;
    for(i=0;i<D_8014A108;i++)if(D_803BACE0[i]!=1)all=0;
    if(all==1)D_803B5ED4=1;
    if(D_80157244) {
        func_803949CC();
        func_800B5570(4);
    } else if(D_803B5ED4) {
        switch(D_803B5ED4) {
        case 1:
            for(node=D_8012E6E0;node;node=node->body->next) {
                for(j=0;j<D_8014A108;j++)if(node==D_8014A118[j].owner)break;
                if(j==D_8014A108)object_render_cleanup(node);
            }
            for(j=0;j<D_8014A108;j++) {
                if(!func_800CCEFC(D_8014A118[j].owner))break;
                func_800B4DA4(D_8014A118[j].owner);
                func_800A3424((void **)D_8014A118[j].owner->body->selection,1);
                if(j>=2)player_mode_set(j,1);
                random_seed_init(j,1);
            }
            if(j<D_8014A108) {
                for(i=0;i<D_8014A108;i++) {
                    D_803BAD80[i]=3;
                    func_80394B00(i,0);
                }
            } else {
                for(;j<4;j++)if(j>=2)player_mode_set(j,0);
                net_state_validate();
                func_803949CC();
                func_800B5570(64);
            }
            break;
        case 2:
            func_803949CC();
            func_800B5570(16);
            for(i=0;i<D_8014A108;i++)D_8014A118[i].owner=0;
            break;
        }
        D_803B5ED4=0;
    }
    func_80394834();
    func_80394564(-1);
}
/* Complete native-led 97AD8 model-slot initialization, nonmatching context.
 * Scalar constants and four-byte white color initializer are authenticated
 * against image A; accessed Slot64 fields retain native offsets/capacities.
 * No fabricated formal or caller: actual input is one signed player byte.
 */
/* Existing matched texture lookup declares this table-count byte volatile. */
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
void func_80397AD8(s8 player)
{
    u8 color[4]={255,255,255,255};
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
    D_803B470C[player][16].position[0]*=1.0f;
    D_803B470C[player][16].position[1]*=1.0f;
    D_803B470C[player][16].position[2]=200.0f*1.0f;
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
    D_803B470C[player][20].position[0]*=1.0f;
    D_803B470C[player][20].position[1]*=1.0f;
    D_803B470C[player][20].position[2]=100.0f*1.0f;
    func_8008B32C(D_803B470C[player][20].matrix,D_803B470C[player][20].matrix,.5f);
    memcpy(D_803B470C[player][21].matrix,D_8011418C,36);
    func_800B5898(-1.57079632679f,D_803B470C[player][21].matrix);
    D_803B470C[player][21].position[0]=-87.1f;
    D_803B470C[player][21].position[1]=D_8014A108>=2?50.2f:55.2f;
    D_803B470C[player][21].position[0]*=1.0f;
    D_803B470C[player][21].position[1]*=1.0f;
    D_803B470C[player][21].position[2]=100.0f*1.0f;
    D_803BA868[player]=1;
    D_803BA888[player]=1;
    particle_velocity_set();
}
