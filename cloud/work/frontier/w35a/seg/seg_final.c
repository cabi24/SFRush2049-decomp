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
 * Levers (all three are real, uncompiled-away source facts, none is a compiled-out device):
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
 *   - `mask = 1 << p` is the one extra named local (retail has a fourth unused-looking word between tex and texIdx
 *     in the local area: texIdx sp+114, color sp+108); it is used, three times, as the GC-style shared mask.
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
