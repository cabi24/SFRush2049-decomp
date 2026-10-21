/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef union Color4 {u8 c[4];u32 word;} Color4;
typedef struct Slot {s32 kind,handle;f32 angle,matrix[3][3],position[3];u8 alpha,unknown3D[3];} Slot;
extern Slot D_803AF9A8[][44];
extern s32 D_803B9B90[], D_803BA050[], D_803B28B8[], D_803BA060[][19], D_803B9BA0[];
extern f32 D_803B25AC,D_803B25B0,D_803B25B4,D_803B25B8,D_803B25BC,D_803B25C0,D_803B25C4;
extern f32 D_8011418C[12];
extern s8 D_803BA028[], D_803BA02C;
extern volatile u8 D_80140BDC;
extern void *memcpy(void *,const void *,u32);
extern void func_8008E06C(s16,Color4 *);
extern void *func_800B24EC(char *,u16 *,s32,s8,s32);
extern void func_8008D870(s16,void *,s16);
extern void func_800B5898(f32,void *);
extern void func_800B5940(f32,void *);
extern void func_8008B32C(void *,void *,f32);
extern void model_data_load(s32,s32,u32);
extern void model_transform_setup(s32,s32,u32);
extern s32 func_8039D494(s32,s32);
extern char *D_803AF984[];
extern char D_803B8630[],D_803B8640[],D_803B8648[],D_803B864C[];
extern s16 D_80151AD0; extern s32 D_8014A110;
extern s32 sign_extend_call(s32,void *,s32,u32);
extern s32 string_copy_format(char *,s8,s8,s8);
extern void particle_velocity_set(void);
extern s32 D_803B9B90[];
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


