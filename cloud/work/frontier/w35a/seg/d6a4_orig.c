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
        if(!func_8039D494(player,item))slot->alpha=0;
        ((u8 *)&color)[3]=slot[19].alpha=slot->alpha;
        func_8008E06C(slot->handle,&color);func_8008E06C(slot[19].handle,&color);
        if(!func_8039D494(player,item)) {
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

