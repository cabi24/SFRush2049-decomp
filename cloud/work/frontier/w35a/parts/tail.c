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

extern void func_80090F44(f32,void *);
extern void func_80090E9C(f32,void *);
extern void func_8009EA68(f32,void *);
extern void func_80390DCC(s16,s16,u8,u8,u8,s16,f32 *,f32 (*)[3],u8);
extern void func_8039E444(void);
extern f32 D_80111310[][13],D_80111414[][13];
void func_8039EF08(void)
{
    f32 matrix[3][3],position[3],shadow_matrix[3][3],shadow_position[3];
    s32 player,index,other,car,remaining,ghost,option;
    u32 alpha;
    CarSlot *slot;
    RecordedCar *saved;
    for(player=0;player<D_8014A108;player++) {
        for(index=0,slot=D_803B9BC8[player];index<D_803BA020[player];index++,slot++) {
            if(index==D_803B9FD4[player]) {
                slot->selection+=5.0f*D_8002EB94;
                if(slot->selection>1.0f)slot->selection=1.0f;
            } else {
                slot->selection-=5.0f*D_8002EB94;
                if(slot->selection<0.0f)slot->selection=0.0f;
            }
            if(D_803BA028[player]) {
                remaining=10-D_803BA030;
                if(remaining<2)slot->angle=3.665191411972046f;
                else {
                    slot->angle-=3.665191411972046f;
                    while(slot->angle>3.1415927410125732f)slot->angle-=6.2831854820251465f;
                    while(slot->angle<=-3.1415927410125732f)slot->angle+=6.2831854820251465f;
                    slot->angle-=slot->angle/(remaining-1);
                    slot->angle+=3.665191411972046f;
                }
            } else if(D_803BA060[player][D_803B28B8[player]]==11) {
                slot->angle-=1.5707963705062866f;
                while(slot->angle>3.1415927410125732f)slot->angle-=6.2831854820251465f;
                while(slot->angle<=-3.1415927410125732f)slot->angle+=6.2831854820251465f;
                if(slot->angle>1.5707963705062866f)
                    slot->angle+=1.0471975803375244f*D_8002EB94*4.0f < 3.1415927410125732f-slot->angle ? 1.0471975803375244f*D_8002EB94*4.0f : 3.1415927410125732f-slot->angle;
                else if(slot->angle< -1.5707963705062866f)
                    slot->angle-=1.0471975803375244f*D_8002EB94*4.0f < slot->angle+3.1415927410125732f ? 1.0471975803375244f*D_8002EB94*4.0f : slot->angle+3.1415927410125732f;
                else if(slot->angle>0.0f)
                    slot->angle-=1.0471975803375244f*D_8002EB94*4.0f < slot->angle ? 1.0471975803375244f*D_8002EB94*4.0f : slot->angle;
                else slot->angle+=1.0471975803375244f*D_8002EB94*4.0f < -slot->angle ? 1.0471975803375244f*D_8002EB94*4.0f : -slot->angle;
                slot->angle+=1.5707963705062866f;
            } else {
                slot->angle+=1.0471975803375244f*D_8002EB94;
                if(slot->angle>=6.2831854820251465f)slot->angle-=6.2831854820251465f;
            }
            func_8008B32C(D_8011418C,matrix,2.5f);
            func_80090F44(0.3490658402442932f,matrix);
            func_80090E9C(slot->angle,matrix);
            car=slot->car%13;
            if(car==10) {
                if(D_80151AD0==1) {position[0]=0.0f;position[1]=3.200000047683716f;position[2]=40.5f;}
                else {position[0]=0.0f;position[1]=5.900000095367432f;position[2]=40.5f;}
            } else {
                if(D_80151AD0==1) {position[0]=0.0f;position[1]=2.5999999046325684f;position[2]=40.5f;}
                else {position[0]=0.0f;position[1]=5.300000190734863f;position[2]=40.5f;}
            }
            alpha=(u8)(slot->selection*255.0f);
            if(D_8014A110==6 && !D_801164BE)
                func_80390DCC(slot->car,player,D_80138664[D_8012E67C[player]],D_80138664[D_8012E67C[player]],D_80138664[D_8012E67C[player]],D_803B28B0[player],position,matrix,alpha);
            else func_80390DCC(slot->car,player,D_80111611[player][car],D_80111655[player][car],D_80111699[player][car],D_803B28B0[player],position,matrix,alpha);
            if(D_8014A108<3 && D_8014A110!=2) {
                other=player==0 ? 2 : 3;
                func_8008B32C(matrix,shadow_matrix,1.2f);
                func_8009EA68(3.1415927410125732f,shadow_matrix);
                shadow_position[0]=position[0]*1.2000000476837158f;
                shadow_position[1]=position[1]*1.2000000476837158f;
                shadow_position[2]=position[2]*1.2000000476837158f;
                if(slot->car%13==10)shadow_position[1]-=4.0f;
                else shadow_position[1]-=2.5f;
                D_803B9BC8[other][index].car=slot->car+26;
                if(D_8014A110==6 && !D_801164BE)
                    func_80390DCC(D_803B9BC8[other][index].car,other,D_80138664[D_8012E67C[player]],D_80138664[D_8012E67C[player]],D_80138664[D_8012E67C[player]],D_803B28B0[player],shadow_position,shadow_matrix,alpha/1.5f);
                else {
                    car=slot->car%13;
                    func_80390DCC(D_803B9BC8[other][index].car,other,D_80111611[player][car],D_80111655[player][car],D_80111699[player][car],D_803B28B0[player],shadow_position,shadow_matrix,alpha/1.5f);
                }
            }
            if(index==D_803B9FD4[player]) {
                func_8008B32C(D_8011418C,D_803B9E40[player][0],1.3f);
                func_8008B32C(D_8011418C,D_803B9E40[player][1],1.3f);
                func_80090F44(0.3490658402442932f,D_803B9E40[player][0]);
                func_80090F44(0.3490658402442932f,D_803B9E40[player][1]);
                func_80090E9C(slot->angle,D_803B9E40[player][0]);
                if(D_80151AD0==1) {
                    D_803B9E40[player][0][9]=D_803B9E40[player][1][9]=0.0f;
                    D_803B9E40[player][0][10]=D_803B9E40[player][1][10]=1.7999999523162842f;
                    D_803B9E40[player][0][11]=D_803B9E40[player][1][11]=45.0f;
                } else {
                    D_803B9E40[player][0][9]=D_803B9E40[player][1][9]=0.0f;
                    D_803B9E40[player][0][10]=D_803B9E40[player][1][10]=4.800000190734863f;
                    D_803B9E40[player][0][11]=D_803B9E40[player][1][11]=45.0f;
                }
                if(D_8014A110==2) {
                    for(ghost=1;ghost<D_803BA00C+1;ghost++) {
                        if((!D_80152698[ghost] || D_803BA01A[ghost-1]!=(*(*D_80152698[ghost])->active)->car) && D_803BA01A[ghost-1]!=-1)
                            func_80390DCC(D_803B9BC8[ghost][D_803BA01A[ghost-1]].car,ghost,0,0,0,0,position,matrix,0);
                        if(D_80152698[ghost]) {
                            car=(*(*D_80152698[ghost])->active)->car;
                            D_803BA01A[ghost-1]=car;
                            D_803B9BC8[ghost][car].car=ghost*13+car;
                            D_801111A9[ghost][car]=(*(*D_80152698[ghost])->active)->component;
                            D_80111589[ghost][car]=(*(*D_80152698[ghost])->active)->part;
                            D_80111310[ghost][car]=(*(*D_80152698[ghost])->active)->stat0/100.0f+0.75f;
                            D_80111414[ghost][car]=(*(*D_80152698[ghost])->active)->stat1/100.0f+0.75f;
                            saved=*(*D_80152698[ghost])->active;
                            func_80390DCC(D_803B9BC8[ghost][car].car,ghost,saved->red,saved->green,saved->blue,0,position,matrix,255);
                        }
                    }
                }
            }
        }
        if(D_8014A108==1 && D_8014A110!=2) {
            option=D_803BA060[player][D_803B28B8[player]];
            func_8039ED20(!D_803BA028[0] && (option==1 || option==2 || option==3 || option==4 || option==5));
            func_8039E444();
        }
    }
}
