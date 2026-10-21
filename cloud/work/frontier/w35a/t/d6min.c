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
extern f32 fabsf(f32);
#pragma intrinsic(fabsf)
extern volatile f32 D_8002EB94;
extern char D_803B8650[],D_803B8660[];
void func_8039D6A4(s8 p)
{
    static f32 cursorSpeed[4];
    static f32 scrollSpeed[4];
    s32 buttonIdx;
    f32 prevPos[3];
    f32 buttonY;
    f32 selectedPos;
    s8 clearScrollSpeed=1;
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
        if(fabsf(D_803AF9A8[p][39].position[1]-prevPos[1])*60.0f*D_803B25AC/10.0f>cursorSpeed[p]) {
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
            if(fabsf(D_803AF9A8[p][buttonIdx].position[1]-prevPos[1])*60.0f/10.0f>scrollSpeed[p]) {
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
        else if(D_803AF9A8[p][buttonIdx].position[1]>D_803B25B8)D_803AF9A8[p][buttonIdx].alpha=255.0f-(D_803AF9A8[p][buttonIdx].position[1]-D_803B25B8)*255.0f/(D_803B25B0*D_803B25AC);
        else if(D_803AF9A8[p][buttonIdx].position[1]<D_803B25BC)D_803AF9A8[p][buttonIdx].alpha=255.0f-(D_803B25BC-D_803AF9A8[p][buttonIdx].position[1])*255.0f/(D_803B25B0*D_803B25AC);
        else D_803AF9A8[p][buttonIdx].alpha=255;
        if(!func_8039D494(p,buttonIdx))D_803AF9A8[p][buttonIdx].alpha=0;
        buttonColor.c[3]=D_803AF9A8[p][buttonIdx+19].alpha=D_803AF9A8[p][buttonIdx].alpha;
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
            model_transform_setup(D_803AF9A8[p][buttonIdx].handle,0,1<<p);
            model_transform_setup(D_803AF9A8[p][buttonIdx+19].handle,0,1<<p);
            buttonY-=D_803B25B0*D_803B25AC;
        }
    }
    if(D_803BA028[p]==1) {
        model_data_load(D_803AF9A8[p][40].handle,1,15);
        model_data_load(D_803AF9A8[p][41].handle,1,15);
        D_803AF9A8[p][39].position[2]=2000.0f;
    } else {
        model_transform_setup(D_803AF9A8[p][40].handle,0,1<<p);
        model_transform_setup(D_803AF9A8[p][41].handle,0,1<<p);
    }
    if(D_803BA02C==1) {
        model_data_load(D_803AF9A8[p][42].handle,1,15);
        model_data_load(D_803AF9A8[p][43].handle,1,15);
    } else {
        model_transform_setup(D_803AF9A8[p][42].handle,0,1<<p);
        model_transform_setup(D_803AF9A8[p][43].handle,0,1<<p);
    }
    if(clearScrollSpeed==1)scrollSpeed[p]=0.0f;
    D_803B9BA0[p]=0;
}
