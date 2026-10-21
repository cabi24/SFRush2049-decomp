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
        func_800B5898(D_803AF9A8[p][buttonIdx].angle-1.5707964,D_803AF9A8[p][buttonIdx].matrix);
        func_800B5898(D_803AF9A8[p][buttonIdx+19].angle-1.5707964,D_803AF9A8[p][buttonIdx+19].matrix);
        if(D_803AF9A8[p][buttonIdx].angle>1.5707964)tex=func_800B24EC(D_803B8650,&texIdx,0,D_80140BDC-1,1);
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
