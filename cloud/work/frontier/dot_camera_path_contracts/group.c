/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Research only: full source drafts, not a closed or matching IPA unit.
 * Native contracts and explicit remaining dependencies: see README.md. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef double f64;
#define NULL ((void *)0)
#define M2C_FIELD(e,t,o) (*(t)((u8 *)(e)+(o)))
#define HALF(p,n) (*(s16 *)((u8 *)(p)+(n)))
typedef struct Position { s16 x,y,z; } Position;
typedef struct SearchPoint { s16 x,y,z; u8 speed,flags; } SearchPoint;
typedef struct Descriptor16 {u8 type; s8 next;u16 nextPos;s8 parent;u8 gap;u16 end;s8 section;u8 gap9;u16 count;Position *points;} Descriptor16;
typedef struct Header16 {u16 count;u16 gap2;Position *points;u8 paths;u8 gap9[3];Descriptor16 *descriptors;} Header16;
typedef struct Route8 {u16 count,gap;SearchPoint *points;} Route8;
typedef struct Record80 {u8 bytes[80];} Record80;
typedef struct CameraCar {
 u8 pre22C[0x22C]; f32 position[3];
 u8 pre660[0x660-0x238]; f32 cameraPosition[3]; f32 currentPosition[3];
 u8 pre6C4[0x6C4-0x678]; s16 cameraState;
 u8 pre7C4[0x7C4-0x6C6]; s16 pathPoint, player;
 u8 pre7E2[0x7E2-0x7C8]; s16 section, nextSection;
 u8 tail[0x808-0x7E6];
} CameraCar;
extern Header16 D_801407F0;
extern Route8 D_8012E5E8[];
extern Record80 D_80151CE8[];
extern CameraCar D_8014A250[];
extern s16 D_8014A108;
extern s32 D_8014A110;
extern s32 D_8011735C;
extern f32 D_80123F84;
void func_8038CA24(s32);
s32 func_800D3430(s32,s32,s32 *,s32 *,s32);
void math_utility(void *,void *);
void func_800C40E8(f32,f32,void *);
void func_800C4180(void *,f32 *);
void func_800C4C9C(void *,s16);
void func_800D11BC(void *);
void func_800D14F4(void *,f32 *);
void func_8009E820(f32 *,f32 *,f32 *);
s16 func_800D2C10(void *,s32);
u16 *camera_trigger_check(void *,f32 *,void *);
void camera_follow_path(s32,s32,s32,f32 *);
void race_countdown_display(void *,f32);
void lap_count_select(s32,s32,s32,s32,s32 *,s32 *);
s32 difficulty_select(s32,s32,s32,s32);
s32 viDeadlinePassed(void);
f32 sinf(f32), cosf(f32), fabsf(f32), sqrtf(f32);
f32 func_80098A54(f32 *);
/* This callee receives its float in f16 natively. Its source body must
 * join a future complete IPA closure; this prototype is semantic only. */
f32 func_8009C3F8(s32,f32);
#pragma intrinsic(fabsf,sqrtf)

/* Select a path point for the camera car. Native entry receives car/node/point
 * through s0/s2/s3; those are ordinary source parameters in this real call group.
 * Mode 6 maximizes the nearest other-car XZ distance, mode 5 uses the saved point,
 * other modes choose the nearest XYZ point and restrict it to section bounds.
 * Record padding below describes verified field offsets, not stack shaping. */
f32 func_8008B2E4(f32 range) {
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (f32)((D_8011735C >> 16) & 0x7FFF) * range / 32768.0f;
}

void func_800D348C(CameraCar *car, s32 *outNode, s32 *outPoint) {
    s32 node, point, i, j, scanPoint, startPoint, endPoint, shifted;
    s32 selected, randomStart;
    f32 x,y,z,dx,dy,dz,distance,best,farthest;
    f32 otherX,otherZ,xzDistance;
    Position *p;
    CameraCar *other;
    node=-1;
    point=0;
    best=1.0e20f;
    if(D_8014A110==6) {
        func_8038CA24(car->player);
        farthest=0.0f;
        selected=0;
        randomStart=(s32)func_8008B2E4((f32)(u32)D_801407F0.count);
        for(i=0;i<D_801407F0.count;i++) {
            scanPoint=randomStart+i;
            best=1.0e20f;
            if(scanPoint>=D_801407F0.count) scanPoint-=D_801407F0.count;
            for(j=0;j<D_8014A108;j++) {
                if(j!=car->player) {
                    p=&D_801407F0.points[scanPoint];
                    other=&D_8014A250[j];
                    if(other->cameraState>=0) {otherX=other->cameraPosition[0];otherZ=other->cameraPosition[2];}
                    else {otherX=other->position[0];otherZ=other->position[2];}
                    dx=(f32)p->x-otherX; dz=(f32)p->z-otherZ;
                    xzDistance=dx*dx+dz*dz;
                    if(xzDistance<best) best=xzDistance;
                }
            }
            if(farthest<best) {selected=scanPoint;farthest=best;}
        }
        point=selected;
    } else if(D_8014A110==5) {
        point=car->pathPoint;
    } else {
        x=car->currentPosition[0];y=car->currentPosition[1];z=car->currentPosition[2];
        for(i=0,p=D_801407F0.points;i<D_801407F0.count;i++,p++) {
            dx=(f32)p->x-x; dy=(f32)p->y-y; dz=(f32)p->z-z;
            distance=dx*dx+dy*dy+dz*dz;
            if(distance<best) {best=distance;point=i;}
        }
        if(D_8014A110!=1 && D_8014A110!=4 && D_8014A110!=5) {
            if(best>1600.0f) {
                for(j=0;j<D_801407F0.paths;j++) {
                    for(i=0;i<D_801407F0.descriptors[j].count;i++) {
                        p=&D_801407F0.descriptors[j].points[i];
                        dx=(f32)p->x-x;dy=(f32)p->y-y;dz=(f32)p->z-z;
                        distance=dx*dx+dy*dy+dz*dz;
                        if(distance<best) {point=i;node=j;best=distance;}
                    }
                }
            }
            if(node>=0 && D_801407F0.descriptors[node].type==0)
                func_800D3430(node,point,&node,&point,1);
            if(node<0 || D_801407F0.descriptors[node].type!=2) {
                if(node>=0) {
                    shifted=node+5;
                    if(HALF(&D_80151CE8[car->section],0x38+node*2)>=0) {
                        endPoint=HALF(&D_80151CE8[car->nextSection],0x2E + shifted*2);
                        startPoint=HALF(&D_80151CE8[car->section],0x2E + shifted*2);
                        if(endPoint>=0) {
                            if(point<startPoint || point>endPoint) {
                                best=1.0e20f;
                                for(i=startPoint;i<=HALF(&D_80151CE8[car->nextSection],0x2E + shifted*2);i++) {
                                    p=&D_801407F0.descriptors[node].points[i];
                                    dx=(f32)p->x-x;dy=(f32)p->y-y;dz=(f32)p->z-z;
                                    distance=dx*dx+dy*dy+dz*dz;
                                    if(distance<best) {point=i;best=distance;}
                                }
                            }
                        } else if(point<startPoint) point=startPoint;
                    } else if(HALF(&D_80151CE8[car->nextSection],0x2E + shifted*2)<0 && car->section!=D_801407F0.descriptors[node].section) {
                        node=-1;point=-1;
                    }
                }
                if(node<0) {
                    if(car->nextSection<car->section) endPoint=D_801407F0.count;
                    else endPoint=HALF(&D_80151CE8[car->nextSection],0x2E);
                    startPoint=HALF(&D_80151CE8[car->section],0x2E);
                    if(point<startPoint || point>=endPoint) {
                        best=1.0e20f;
                        for(i=startPoint,p=&D_801407F0.points[startPoint];i<endPoint;i++,p++) {
                            dx=(f32)p->x-x;dy=(f32)p->y-y;dz=(f32)p->z-z;
                            distance=dx*dx+dy*dy+dz*dz;
                            if(distance<best) {best=distance;point=i;}
                        }
                    }
                }
            }
        }
    }
    *outNode=node;*outPoint=point;
}

void camera_follow_path(s32 pathMode,s32 point,s32 route,f32 *matrix) {
    f32 origin[3],delta[3],surface[3],angle;
    f32 heading,sine,cosine,vertical,horizontal;
    Descriptor16 *descriptor;
    Position *current,*target;
    SearchPoint *searchCurrent,*searchTarget;
    if(route>=0) {
        if(pathMode) {
            descriptor=&D_801407F0.descriptors[route];
            origin[0]=(f32)descriptor->points[point].x;
            origin[1]=(f32)descriptor->points[point].y;
            origin[2]=(f32)descriptor->points[point].z;
            if(point+1==descriptor->count) {
                if(descriptor->parent>=0) {
                    delta[0]=(f32)D_801407F0.descriptors[descriptor->parent].points[descriptor->end].x-origin[0];
                    delta[1]=(f32)D_801407F0.descriptors[descriptor->parent].points[descriptor->end].y-origin[1];
                    delta[2]=(f32)D_801407F0.descriptors[descriptor->parent].points[descriptor->end].z-origin[2];
                } else {
                    delta[0]=(f32)D_801407F0.points[descriptor->end].x-origin[0];
                    delta[1]=(f32)D_801407F0.points[descriptor->end].y-origin[1];
                    delta[2]=(f32)D_801407F0.points[descriptor->end].z-origin[2];
                }
            } else {
                delta[0]=(f32)descriptor->points[point+1].x-origin[0];
                delta[1]=(f32)descriptor->points[point+1].y-origin[1];
                delta[2]=(f32)descriptor->points[point+1].z-origin[2];
            }
        } else {
            searchCurrent=&((Route8 *)D_8012E5E8)[route].points[point];
            origin[0]=(f32)searchCurrent->x;
            origin[1]=(f32)searchCurrent->y;
            origin[2]=(f32)searchCurrent->z;
            if(point+1==D_8012E5E8[route].count)
                searchTarget=&((Route8 *)D_8012E5E8)[route].points[HALF(&D_80151CE8[HALF(D_80151CE8,2)],48+route*2)];
            else searchTarget=searchCurrent+1;
            delta[0]=(f32)(searchTarget->x-searchCurrent->x);
            delta[1]=(f32)(searchTarget->y-searchCurrent->y);
            delta[2]=(f32)(searchTarget->z-searchCurrent->z);
        }
    } else {
        current=&D_801407F0.points[point];
        origin[0]=(f32)current->x;
        origin[1]=(f32)current->y;
        origin[2]=(f32)current->z;
        if(point+1==D_801407F0.count)
            target=&D_801407F0.points[HALF(&D_80151CE8[HALF(D_80151CE8,2)],46)];
        else target=current+1;
        delta[0]=(f32)(target->x-current->x);
        delta[1]=(f32)(target->y-current->y);
        delta[2]=(f32)(target->z-current->z);
    }
    func_80098A54(delta);
    if(camera_trigger_check(origin,surface,matrix)) {
        heading=func_8009C3F8(0,delta[0]);
        if(delta[2]<0.0f) heading=D_80123F84-heading;
        func_800C4180(matrix,&angle);
        sine=sinf(heading+angle);
        cosine=cosf(heading+angle);
    } else {
        horizontal=sqrtf(delta[2]*delta[2]+delta[0]*delta[0]);
        vertical=delta[1];
        if(delta[2]<0.0f) vertical=-vertical;
        matrix[0]=1.0f;
        matrix[5]=-vertical;
        matrix[1]=0.0f;
        matrix[2]=0.0f;
        matrix[3]=0.0f;
        matrix[4]=horizontal;
        matrix[6]=0.0f;
        matrix[7]=vertical;
        matrix[8]=horizontal;
        sine=delta[0];
        cosine=delta[2];
    }
    func_800C40E8(-sine,cosine,matrix);
}
extern u8 D_8011418C[];
extern u8 D_80117360[];
extern s32 D_801174B4;
extern u8 D_801210E8[];
extern s32 D_8012E5EC;
extern s8 D_801407B0;
extern s8 D_80142760;
extern s32 D_8014A110;
extern u8 D_8014A96C[];
extern s16 D_80152734;
extern u8 D_80152818[];
extern u8 D_80153E8F[];
extern f32 D_80154390;

void stunt_combo_display(void *arg0) {
    s16 sp186;
    s32 sp164;
    s32 sp160;
    s32 sp15C;
    s32 sp158;
    s32 sp154;
    f32 sp12C;
    f32 sp128;
    f32 velocity[3];
    s32 sp114;
    f32 spF4;
    f32 spF0;
    f32 spEC;
    f32 spE8;
    f32 spDC[3];
    f32 spB0;
    f32 spAC;
    void *sp74;
    f32 *sp70;
    f32 *sp6C;
    void *sp68;
    f32 *sp64;
    f32 *temp_a1;
    f32 *temp_a2;
    f32 *temp_a2_2;
    f32 *temp_v0;
    f32 *temp_v0_2;
    f32 *temp_v1_2;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f10;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f14;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 var_f0;
    f32 var_f10;
    f32 var_f10_2;
    f32 var_f14;
    f32 var_f14_2;
    f32 var_f16;
    f32 var_f16_2;
    s16 temp_a3;
    s16 temp_a3_2;
    s16 temp_a3_3;
    s16 temp_a3_4;
    s16 temp_t2;
    s16 temp_v0_10;
    s16 temp_v0_4;
    s16 temp_v0_7;
    s16 temp_v0_8;
    s16 temp_v0_9;
    s16 var_a2;
    s16 var_v0_3;
    s16 var_v0_4;
    s32 var_v1_2;
    s32 temp_f6;
    s32 temp_t8;
    s32 temp_v1;
    s32 var_a1;
    s32 var_t1;
    s32 var_v0;
    s32 var_v1;
    u16 *temp_v0_5;
    s32 var_a0;
    u8 temp_t7;
    u8 temp_t9;
    void *temp_a0;
    void *temp_t1;
    void *temp_t3;
    void *temp_v0_11;
    void *temp_v0_3;
    void *temp_v0_6;
    void *var_a0_2;
    void *var_t3;
    void *var_t3_2;
    void *var_v0_2;

    sp186 = M2C_FIELD(arg0, s16 *, 0x7C6);
    if ((D_80142760 == 0) || (M2C_FIELD(arg0, f32 *, 0x6C0) == 0.0f) || (M2C_FIELD(arg0, s8 *, 0x6CC) != 0)) {
        M2C_FIELD(arg0, s8 *, 0x6CC) = 0;
        temp_t8 = *((u8 *) &D_80153E8F + (sp186 * 8)) == 6;
        sp114 = temp_t8;
        if (temp_t8 != 0) {
            temp_v0 = (f32 *)((M2C_FIELD(arg0, s16 *, 0x7C6) * 4) + (u8 *) &D_80117360);
            if (M2C_FIELD(arg0, f32 *, 0x714) < *temp_v0) {
                *temp_v0 = 0.0f;
            }
        }
        var_v1 = 0;
        if ((sp114 != 0) && (M2C_FIELD(arg0, s8 *, 0x6CD) == 0) && (D_801407B0 != 0)) {
            temp_v0_2 = (f32 *)((M2C_FIELD(arg0, s16 *, 0x7C6) * 4) + (u8 *) &D_80117360);
            temp_f0 = M2C_FIELD(arg0, f32 *, 0x714);
            if (((*temp_v0_2 + 2.0f) < temp_f0) && (D_80142760 == 0)) {
                var_v1 = 1;
                *temp_v0_2 = temp_f0;
            }
        }
        M2C_FIELD(arg0, s8 *, 0x6CD) = 0;
        if (M2C_FIELD(arg0, f32 *, 0x6C0) == 0.0f) {
            M2C_FIELD(arg0, f32 *, 0x6D4) = (f32) M2C_FIELD(arg0, f32 *, 0x66C);
            M2C_FIELD(arg0, f32 *, 0x6D8) = (f32) M2C_FIELD(arg0, f32 *, 0x670);
            M2C_FIELD(arg0, f32 *, 0x6DC) = (f32) M2C_FIELD(arg0, f32 *, 0x674);
            math_utility((u8 *) arg0 + 0x678, (u8 *) arg0 + 0x6E0);
            if ((D_8014A110 == 6) || (D_8014A110 == 4) || (D_8014A110 == 5) || ((sp114 == 0) && (M2C_FIELD(arg0, s16 *, 0x6D0) >= 6))) {
                M2C_FIELD(arg0, f32 *, 0x704) = 0.0f;
                M2C_FIELD(arg0, f32 *, 0x708) = 0.0f;
                M2C_FIELD(arg0, f32 *, 0x70C) = 0.0f;
            } else {
                temp_v0_3 = (u8 *) &D_801210E8 + (M2C_FIELD(arg0, s16 *, 0x6D0) * 0xC);
                M2C_FIELD(arg0, f32 *, 0x704) = (f32) M2C_FIELD(temp_v0_3, f32 *, 0);
                M2C_FIELD(arg0, f32 *, 0x708) = (f32) M2C_FIELD(temp_v0_3, f32 *, 4);
                M2C_FIELD(arg0, f32 *, 0x70C) = (f32) M2C_FIELD(temp_v0_3, f32 *, 8);
            }
            M2C_FIELD(arg0, s16 *, 0x6C4) = -1;
            func_800D11BC(arg0);
            if (D_801174B4 & 8) {
                temp_v0_4 = M2C_FIELD(((u8 *) &D_80152818 + (sp186 * 0x3B8)), s16 *, 0x33C);
                temp_t7 = M2C_FIELD((*((s32 *) ((u8 *) &D_8012E5EC + (temp_v0_4 * 8))) + (M2C_FIELD(((u8 *) &D_80151CE8 + (M2C_FIELD(arg0, s16 *, 0x7E2) * 0x50) + (temp_v0_4 * 2)), s16 *, 0x30) * 8)), u8 *, 6);
                var_f10 = (f32) temp_t7;
                if ((s32) temp_t7 < 0) {
                    var_f10 += 4294967296.0f;
                }
                temp_f6 = (s32) ((var_f10 * 1.4666667f) / 2.0f);
                M2C_FIELD(arg0, s32 *, 0x6BC) = temp_f6;
                temp_f0_2 = (f32) temp_f6;
                M2C_FIELD(arg0, f32 *, 0x9C) = temp_f0_2;
                M2C_FIELD(arg0, f32 *, 0x48) = temp_f0_2;
                M2C_FIELD(arg0, f32 *, 0xA8) = (f32) temp_f6;
                M2C_FIELD(arg0, f32 *, 0x478) = (f32) (M2C_FIELD(arg0, f32 *, 0x9C) / M2C_FIELD(arg0, f32 *, 0x430));
                M2C_FIELD(arg0, f32 *, 0xB4) = (f32) temp_f6;
                M2C_FIELD(arg0, f32 *, 0x4D4) = (f32) (M2C_FIELD(arg0, f32 *, 0xA8) / M2C_FIELD(arg0, f32 *, 0x48C));
                M2C_FIELD(arg0, f32 *, 0xC0) = (f32) temp_f6;
                M2C_FIELD(arg0, f32 *, 0x530) = (f32) (M2C_FIELD(arg0, f32 *, 0xB4) / M2C_FIELD(arg0, f32 *, 0x4E8));
                M2C_FIELD(arg0, f32 *, 0x58C) = (f32) (M2C_FIELD(arg0, f32 *, 0xC0) / M2C_FIELD(arg0, f32 *, 0x544));
            }
            *((u8 *) &D_8014A96C + (sp186 * 0x808)) = 1;
            return;
        }
        if (var_v1 != 0) {
            M2C_FIELD(arg0, s32 *, 0x6BC) = 0;
            temp_a0 = (u8 *) arg0 + 0x7A0;
            M2C_FIELD(arg0, f32 *, 0x660) = (f32) M2C_FIELD(arg0, f32 *, 0x22C);
            M2C_FIELD(arg0, f32 *, 0x664) = (f32) M2C_FIELD(arg0, f32 *, 0x230);
            M2C_FIELD(arg0, f32 *, 0x668) = (f32) M2C_FIELD(arg0, f32 *, 0x234);
            sp68 = temp_a0;
            func_800C4180(temp_a0, &spEC);
            temp_a2 = (f32 *)((u8 *) arg0 + 0x678);
            sp70 = temp_a2;
            spEC = -spEC;
            temp_v0_5 = camera_trigger_check((u8 *) arg0 + 0x660, spDC, temp_a2);
            if (temp_v0_5 != NULL) {
                if (*temp_v0_5 & 0x2000) {
                    var_v0 = 1;
                } else {
                    func_800C4180(sp70, &spE8);
                    spF4 = sinf(spEC + spE8);
                    var_f14 = cosf(spEC + spE8);
                    goto block_31;
                }
            } else {
                spF4 = sinf(spEC);
                spF0 = cosf(spEC);
                math_utility(&D_8011418C, sp70);
                var_f14 = spF0;
block_31:
                func_800C40E8(-spF4, var_f14, sp70);
                var_v0 = 0;
            }
            if (var_v0 == 0) {
                M2C_FIELD(arg0, s32 *, 0x7D4) = (s32) (M2C_FIELD(arg0, s32 *, 0x7D4) & ~0x10);
                M2C_FIELD(arg0, s8 *, 0x640) = 0;
                M2C_FIELD(arg0, f32 *, 0x664) = (f32) (M2C_FIELD(arg0, f32 *, 0x664) + 10.0f);
                var_t3 = (sp186 * 0x3B8) + (u8 *) &D_80152818;
                M2C_FIELD(arg0, f32 *, 0x70C) = 0.0f;
                M2C_FIELD(arg0, f32 *, 0x708) = 0.0f;
                M2C_FIELD(arg0, f32 *, 0x704) = 0.0f;
            } else {
                goto block_35;
            }
        } else {
            sp68 = (u8 *) arg0 + 0x7A0;
            sp70 = (f32 *)((u8 *) arg0 + 0x678);
block_35:
            temp_t3 = (u8 *) &D_80152818 + (sp186 * 0x3B8);
            if (M2C_FIELD(temp_t3, s8 *, 0x309) != 0) {
                sp12C = 1.1f;
            } else {
                sp12C = 0.8f;
            }
            M2C_FIELD(arg0, s8 *, 0x7EA) = 0;
            M2C_FIELD(temp_t3, s8 *, 0x308) = (s8) M2C_FIELD(arg0, s8 *, 0x7EA);
            M2C_FIELD(arg0, s8 *, 0x7EB) = 0;
            M2C_FIELD(temp_t3, s8 *, 0x309) = (s8) M2C_FIELD(arg0, s8 *, 0x7EB);
            M2C_FIELD(temp_t3, s8 *, 0x311) = -1;
            M2C_FIELD(temp_t3, s8 *, 0x310) = -1;
            M2C_FIELD(temp_t3, f32 *, 0x30C) = (f32) M2C_FIELD(arg0, f32 *, 0x714);
            M2C_FIELD(arg0, s8 *, 0x7DF) = 1;
            if (sp114 != 0) {
                sp74 = temp_t3;
                func_800D348C(arg0, &sp164, &sp15C);
                var_t3_2 = sp74;
                if ((sp164 >= 0) && (temp_v0_6 = &D_801407F0.descriptors[sp164], (M2C_FIELD(temp_v0_6, u8 *, 0) == 2))) {
                    M2C_FIELD(arg0, f32 *, 0x660) = (f32) D_801407F0.descriptors[sp164].points[sp15C].x;
                    M2C_FIELD(arg0, f32 *, 0x664) = (f32) ((f32) M2C_FIELD((M2C_FIELD((M2C_FIELD(&D_801407F0, s32 *, 0xC) + (sp164 * 0x10)), s32 *, 0xC) + (sp15C * 6)), s16 *, 2) + 3.0f);
                    M2C_FIELD(arg0, s32 *, 0x6BC) = 0;
                    M2C_FIELD(arg0, f32 *, 0x668) = (f32) M2C_FIELD((M2C_FIELD((M2C_FIELD(&D_801407F0, s32 *, 0xC) + (sp164 * 0x10)), s32 *, 0xC) + (sp15C * 6)), s16 *, 4);
                    camera_follow_path(1, sp15C, sp164, sp70);
                    var_t3 = var_t3_2;
                    M2C_FIELD(arg0, s8 *, 0x640) = 0;
                    M2C_FIELD(arg0, f32 *, 0x70C) = 0.0f;
                    M2C_FIELD(arg0, s32 *, 0x7D4) = (s32) (M2C_FIELD(arg0, s32 *, 0x7D4) & ~0x10);
                    M2C_FIELD(arg0, f32 *, 0x708) = 0.0f;
                    M2C_FIELD(arg0, f32 *, 0x704) = 0.0f;
                } else {
                    goto block_52;
                }
            } else {
                var_f16 = 1.0e20f;
                var_a2 = 0;
                temp_t2 = M2C_FIELD(((u8 *) &D_80152818 + (M2C_FIELD(arg0, s16 *, 0x7C6) * 0x3B8)), s16 *, 0x33C);
                spB0 = M2C_FIELD(arg0, f32 *, 0x670);
                temp_t1 = (temp_t2 * 8) + (u8 *) &D_8012E5E8;
                spAC = M2C_FIELD(arg0, f32 *, 0x674);
                temp_v0_7 = M2C_FIELD(arg0, s16 *, 0x7E2);
                temp_a3 = M2C_FIELD(arg0, s16 *, 0x7E4);
                var_a1 = temp_t2 * 2;
                if (temp_a3 < temp_v0_7) {
                    var_a0 = M2C_FIELD(temp_t1, u16 *, 0);
                    var_a1 = temp_t2 * 2;
                } else {
                    var_a0 = M2C_FIELD(((u8 *) &D_80151CE8 + (temp_a3 * 0x50) + var_a1), s16 *, 0x30);
                }
                var_v1_2 = M2C_FIELD(((u8 *) &D_80151CE8 + (temp_v0_7 * 0x50) + var_a1), s16 *, 0x30);
                if (var_v1_2 < (s32) var_a0) {
                    var_v0_2 = (u8 *) M2C_FIELD(temp_t1, void **, 4) + (var_v1_2 * 8);
                    do {
                        temp_f0_3 = (f32) M2C_FIELD(var_v0_2, s16 *, 0) - M2C_FIELD(arg0, f32 *, 0x66C);
                        temp_f2 = (f32) M2C_FIELD(var_v0_2, s16 *, 2) - spB0;
                        temp_f12 = (f32) M2C_FIELD(var_v0_2, s16 *, 4) - spAC;
                        temp_f14 = (temp_f0_3 * temp_f0_3) + (temp_f2 * temp_f2) + (temp_f12 * temp_f12);
                        if (temp_f14 < var_f16) {
                            var_f16 = temp_f14;
                            var_a2 = var_v1_2;
                        }
                        var_v1_2 += 1;
                        var_v0_2 = (u8 *) var_v0_2 + 8;
                    } while (var_v1_2 < (s32) var_a0);
                }
                sp164 = (s32) temp_t2;
                sp15C = (s32) var_a2;
                sp74 = temp_t3;
                func_800C4C9C(arg0, var_a2);
                var_t3_2 = temp_t3;
block_52:
                M2C_FIELD(arg0, f32 *, 0x6C8) = (f32) M2C_FIELD(arg0, f32 *, 0x714);
                if ((D_8014A110 == 5) || (D_8014A110 == 0) || (D_8014A110 == 1) || (D_8014A110 == 3) || (D_8014A110 == 2)) {
                    D_80154390 = 0.01f;
                } else {
                    D_80154390 = 1.5f;
                }
                temp_f2_2 = (M2C_FIELD(arg0, f32 *, 0x6C8) - M2C_FIELD(arg0, f32 *, 0x6C0)) + D_80154390;
                if ((D_8014A110 == 1) || (D_8014A110 == 6) || (D_8014A110 == 4) || (D_8014A110 == 5) || (D_801407B0 != 0)) {
                    M2C_FIELD(arg0, s32 *, 0x6BC) = 0;
                } else if (M2C_FIELD(arg0, s32 *, 0x6BC) < 0) {
                    M2C_FIELD(arg0, s32 *, 0x6BC) = 1;
                } else if (sp114 != 0) {
                    if (sp164 >= 0) {
                        var_a0_2 = &D_801407F0.descriptors[sp164].points[sp15C];
                    } else {
                        var_a0_2 = &D_801407F0.points[sp15C];
                    }
                        sp74 = var_t3_2;
                    sp128 = temp_f2_2;
                    M2C_FIELD(arg0, s32 *, 0x6BC) = (s32) ((f32) M2C_FIELD((D_8012E5EC + (func_800D2C10(var_a0_2, 0) * 8)), u8 *, 6) * 1.4666667f);
                } else {
                    temp_t9 = M2C_FIELD((*((s32 *) ((u8 *) &D_8012E5EC + (sp164 * 8))) + (sp15C * 8)), u8 *, 6);
                    var_f10_2 = (f32) temp_t9;
                    if ((s32) temp_t9 < 0) {
                        var_f10_2 += 4294967296.0f;
                    }
                    M2C_FIELD(arg0, s32 *, 0x6BC) = (s32) (var_f10_2 * 1.4666667f);
                }
                var_f0 = (f32) M2C_FIELD(arg0, s32 *, 0x6BC);
                if (146.66667f < var_f0) {
                    M2C_FIELD(arg0, s32 *, 0x6BC) = 0x92;
                    var_f0 = (f32) 0x92;
                }
                temp_a3_2 = M2C_FIELD(arg0, s16 *, 0x7E4);
                var_t1 = (s32) ((var_f0 * sp12C * temp_f2_2) / 20.0f);
                if ((M2C_FIELD(&D_80151CE8, s16 *, 4) == temp_a3_2) && (D_80152734 == (M2C_FIELD(arg0, s8 *, 0x7E8) + 1)) && (D_8014A110 != 1) && (D_8014A110 != 4) && (D_8014A110 != 6) && (D_8014A110 != 5)) {
                        sp154 = var_t1;
                    sp74 = var_t3_2;
                    temp_v1 = difficulty_select(sp114, sp164, sp15C, (s32) temp_a3_2) / 2;
                    if (temp_v1 < var_t1) {
                        var_t1 = temp_v1;
                    }
                }
                sp74 = var_t3_2;
                lap_count_select(sp114, sp164, sp15C, var_t1, &sp160, &sp158);
                if (sp160 >= 0) {
                    if (sp114 != 0) {
                        M2C_FIELD(arg0, f32 *, 0x660) = (f32) D_801407F0.descriptors[sp160].points[sp158].x;
                        M2C_FIELD(arg0, f32 *, 0x664) = (f32) ((f32) M2C_FIELD((M2C_FIELD((M2C_FIELD(&D_801407F0, s32 *, 0xC) + (sp160 * 0x10)), s32 *, 0xC) + (sp158 * 6)), s16 *, 2) + 3.0f);
                        M2C_FIELD(arg0, f32 *, 0x668) = (f32) M2C_FIELD((M2C_FIELD((M2C_FIELD(&D_801407F0, s32 *, 0xC) + (sp160 * 0x10)), s32 *, 0xC) + (sp158 * 6)), s16 *, 4);
                        if ((D_8014A110 != 1) && (D_8014A110 != 6) && (D_8014A110 != 4)) {
                            temp_v0_8 = M2C_FIELD(((u8 *) &D_80151CE8 + (M2C_FIELD(arg0, s16 *, 0x7E4) * 0x50) + (sp160 * 2)), s16 *, 0x38);
                            if ((temp_v0_8 >= 0) && (sp158 >= temp_v0_8) && (viDeadlinePassed() == 0)) {
                                race_countdown_display(arg0, M2C_FIELD(arg0, f32 *, 0x714));
                            }
                        }
                    } else {
                        temp_v0_9 = M2C_FIELD(arg0, s16 *, 0x7E2);
                        temp_a3_3 = M2C_FIELD(arg0, s16 *, 0x7E4);
                        M2C_FIELD(arg0, f32 *, 0x660) = (f32) D_8012E5E8[sp160].points[sp158].x;
                        M2C_FIELD(arg0, f32 *, 0x664) = (f32) ((f32) M2C_FIELD((M2C_FIELD(((u8 *) &D_8012E5E8 + (sp160 * 8)), s32 *, 4) + (sp158 * 8)), s16 *, 2) + 3.0f);
                        M2C_FIELD(arg0, f32 *, 0x668) = (f32) M2C_FIELD((M2C_FIELD(((u8 *) &D_8012E5E8 + (sp160 * 8)), s32 *, 4) + (sp158 * 8)), s16 *, 4);
                        if ((((temp_a3_3 < temp_v0_9) && (sp158 < M2C_FIELD(((u8 *) &D_80151CE8 + (temp_v0_9 * 0x50) + (sp160 * 2)), s16 *, 0x30))) || ((temp_v0_9 < temp_a3_3) && (sp158 >= M2C_FIELD(((u8 *) &D_80151CE8 + (temp_a3_3 * 0x50) + (sp160 * 2)), s16 *, 0x30)))) && (viDeadlinePassed() == 0)) {
                            race_countdown_display(arg0, M2C_FIELD(arg0, f32 *, 0x714));
                        }
                    }
                } else {
                    M2C_FIELD(arg0, f32 *, 0x660) = (f32) D_801407F0.points[sp158].x;
                    M2C_FIELD(arg0, f32 *, 0x664) = (f32) ((f32) M2C_FIELD((M2C_FIELD(&D_801407F0, s32 *, 4) + (sp158 * 6)), s16 *, 2) + 3.0f);
                    M2C_FIELD(arg0, f32 *, 0x668) = (f32) M2C_FIELD((M2C_FIELD(&D_801407F0, s32 *, 4) + (sp158 * 6)), s16 *, 4);
                    if ((D_8014A110 != 1) && (D_8014A110 != 6) && (D_8014A110 != 4) && (((temp_v0_10 = M2C_FIELD(arg0, s16 *, 0x7E2), temp_a3_4 = M2C_FIELD(arg0, s16 *, 0x7E4), ((temp_a3_4 < temp_v0_10) != 0)) && (sp158 < M2C_FIELD(((u8 *) &D_80151CE8 + (temp_v0_10 * 0x50)), s16 *, 0x2E))) || ((temp_v0_10 < temp_a3_4) && (sp158 >= M2C_FIELD(((u8 *) &D_80151CE8 + (temp_a3_4 * 0x50)), s16 *, 0x2E)))) && (viDeadlinePassed() == 0)) {
                        race_countdown_display(arg0, M2C_FIELD(arg0, f32 *, 0x714));
                    }
                }
                if ((D_8014A110 != 1) && (D_8014A110 != 6) && (D_8014A110 != 4) && (D_8014A110 != 5)) {
                    M2C_FIELD(arg0, s32 *, 0x6BC) = 0x3A;
                }
                camera_follow_path(sp114, sp158, sp160, sp70);
                var_t3 = sp74;
                if (sp114 != 0) {
                    if (D_8014A110 == 2) {
                        M2C_FIELD(arg0, f32 *, 0x704) = (f32) M2C_FIELD(&D_801210E8, f32 *, 0x48);
                        M2C_FIELD(arg0, f32 *, 0x708) = (f32) M2C_FIELD(&D_801210E8, f32 *, 0x4C);
                        M2C_FIELD(arg0, f32 *, 0x70C) = (f32) M2C_FIELD(&D_801210E8, f32 *, 0x50);
                    } else {
                        temp_v0_11 = (u8 *) &D_801210E8 + (M2C_FIELD(arg0, s16 *, 0x7C6) * 0xC);
                        M2C_FIELD(arg0, f32 *, 0x704) = (f32) M2C_FIELD(temp_v0_11, f32 *, 0x48);
                        M2C_FIELD(arg0, f32 *, 0x708) = (f32) M2C_FIELD(temp_v0_11, f32 *, 0x4C);
                        M2C_FIELD(arg0, f32 *, 0x70C) = (f32) M2C_FIELD(temp_v0_11, f32 *, 0x50);
                    }
                    if ((D_8014A110 != 6) && (D_8014A110 != 4) && (D_8014A110 != 5) && (D_801407B0 == 0)) {
                        func_8009E820((f32 *)((u8 *) arg0 + 0x704), velocity, sp70);
                        var_t3 = sp74;
                        M2C_FIELD(arg0, f32 *, 0x660) = (f32) (velocity[0] + M2C_FIELD(arg0, f32 *, 0x660));
                        M2C_FIELD(arg0, f32 *, 0x664) = (f32) (velocity[1] + M2C_FIELD(arg0, f32 *, 0x664));
                        M2C_FIELD(arg0, f32 *, 0x668) = (f32) (velocity[2] + M2C_FIELD(arg0, f32 *, 0x668));
                    } else {
                        M2C_FIELD(arg0, f32 *, 0x704) = 0.0f;
                        M2C_FIELD(arg0, f32 *, 0x708) = 0.0f;
                        M2C_FIELD(arg0, f32 *, 0x70C) = 0.0f;
                    }
                }
                M2C_FIELD(arg0, f32 *, 0x704) = 0.0f;
                M2C_FIELD(arg0, f32 *, 0x708) = 0.0f;
                M2C_FIELD(arg0, f32 *, 0x70C) = 0.0f;
            }
        }
        temp_a2_2 = (f32 *)((u8 *) arg0 + 0x69C);
        sp6C = temp_a2_2;
        sp74 = var_t3;
        func_800D14F4((u8 *) arg0 + 0x2EC, temp_a2_2);
        temp_a1 = (f32 *)((u8 *) arg0 + 0x6AC);
        sp64 = temp_a1;
        func_800D14F4(sp70, temp_a1);
        var_f14_2 = 0.0f;
        var_f16_2 = 0.0f;
        var_v0_3 = 0;
        do {
            temp_f2_3 = temp_a1[var_v0_3];
            temp_f12_2 = temp_a2_2[var_v0_3];
            temp_f0_4 = temp_f12_2 - temp_f2_3;
            var_v0_3 += 1;
            var_f16_2 += fabsf(temp_f0_4);
            var_f14_2 += fabsf(temp_f2_3 + temp_f12_2);
        } while (var_v0_3 < 4);
        var_v0_4 = 0;
        if (var_f14_2 < var_f16_2) {
            do {
                temp_v1_2 = &temp_a1[var_v0_4];
                temp_f10 = *temp_v1_2;
                var_v0_4 += 1;
                *temp_v1_2 = -temp_f10;
            } while (var_v0_4 < 4);
        }
        M2C_FIELD(arg0, s16 *, 0x6C4) = 0;
        sp74 = var_t3;
        math_utility(sp68, (u8 *) arg0 + 0x6E0);
        M2C_FIELD(arg0, f32 *, 0x6D4) = (f32) M2C_FIELD(arg0, f32 *, 0x794);
        M2C_FIELD(arg0, f32 *, 0x6D8) = (f32) M2C_FIELD(arg0, f32 *, 0x798);
        M2C_FIELD(arg0, f32 *, 0x6DC) = (f32) M2C_FIELD(arg0, f32 *, 0x79C);
        velocity[0] = M2C_FIELD(arg0, f32 *, 0x220);
        velocity[1] = M2C_FIELD(arg0, f32 *, 0x224);
        velocity[2] = M2C_FIELD(arg0, f32 *, 0x228);
        func_800D11BC(arg0);
        M2C_FIELD(arg0, f32 *, 0x5CC) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0xC4) = 0.0f;
        M2C_FIELD(arg0, s16 *, 0x62C) = 0;
        M2C_FIELD(var_t3, s16 *, 0x344) = 0;
        M2C_FIELD(arg0, s16 *, 0x62E) = 0;
        M2C_FIELD(arg0, f32 *, 0xD0) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0x5D0) = 0.0f;
        M2C_FIELD(var_t3, s16 *, 0x346) = 0;
        M2C_FIELD(arg0, s16 *, 0x630) = 0;
        M2C_FIELD(arg0, f32 *, 0xDC) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0x5D4) = 0.0f;
        M2C_FIELD(var_t3, s16 *, 0x348) = 0;
        M2C_FIELD(arg0, s16 *, 0x632) = 0;
        M2C_FIELD(arg0, f32 *, 0xE8) = 0.0f;
        M2C_FIELD(arg0, f32 *, 0x5D8) = 0.0f;
        M2C_FIELD(var_t3, s16 *, 0x34A) = 0;
        M2C_FIELD(arg0, s8 *, 0x642) = 0;
        M2C_FIELD(arg0, s8 *, 0x641) = 0;
        M2C_FIELD(arg0, s8 *, 0x643) = (s8) M2C_FIELD(arg0, s8 *, 0x642);
        M2C_FIELD(arg0, f32 *, 0x220) = velocity[0];
        M2C_FIELD(arg0, f32 *, 0x224) = velocity[1];
        M2C_FIELD(arg0, f32 *, 0x228) = velocity[2];
        M2C_FIELD(var_t3, s8 *, 0xED) = 1;
    }
}
