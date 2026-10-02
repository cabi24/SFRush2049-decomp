/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct Effect88 {
    struct Effect88 *next;
    f32 matrix[3][3];
    f32 position[3];
    s32 entity;
    u8 unknown56[4];
    f32 lifetime,period;
    f32 velocity[3];
    u32 flags;
    s16 slot,animation;
} Effect88;
extern Effect88 *D_8013F1F0;
extern u8 D_8013F1E0[],D_80152818[],D_8014A250[],D_8012E700[];
extern u32 D_801392D8[];
extern u16 D_8014295A[],D_80142948[];
extern f32 D_8011744C[];
extern s32 D_8011735C;
extern s8 D_80156994,D_8014978C;
extern f32 D_8002EB94;
extern f32 D_80123FC0,D_80123FC4,D_80123FC8,D_80123FCC,D_80123FD0;
extern f32 D_80123FD4,D_80123FD8,D_80123FDC,D_80123FE0,D_80123FE4;
extern void model_data_load(s32,s32,s32);
extern void model_transform_setup(s32,s32,s32);
extern f32 func_8008E0B8(f32 *);
extern void vector_normalize_length(f32 *,f32 *);
extern void math_utility(f32 *,f32 *);
extern void entity_spawn_callback(s16,s32,s32);
extern void func_800AFA84(void *,Effect88 *);
f32 fabsf(f32);
#pragma intrinsic(fabsf)
void hud_render(void)
{
    Effect88 *node,*next;
    u8 *object,*car,*entity;
    f32 direction[3],matrix[3][3],position[3];
    f32 scale,step,drift;
    s16 kind,slot,angle,magnitude;
    s32 i,j;
    u32 seed;
    node=D_8013F1F0;
    if(node) {
    step=D_80123FC0;
    drift=D_80123FC4;
    while(node) {
        next=node->next;
        kind=(node->flags>>17)&15;
        slot=node->slot;
        if(node->flags&0x10) {
            object=D_80152818+slot*952;
            angle=FIELD(object,s16,248)>>2;
            magnitude=(s16)(s32)fabsf((f32)angle);
            if(FIELD(object,s8,861)<2)
                model_data_load(node->entity,1,1<<FIELD(object,s8,860));
            else
                model_transform_setup(node->entity,0,1<<FIELD(object,s8,860));
            if(FIELD(D_8014A250+slot*2056,u16,1564+kind*2)!=2 || magnitude<10) {
                if(kind==0) D_801392D8[slot]&=~0x100;
                else if(kind==1) D_801392D8[slot]&=~0x200;
                else if(kind==2) D_801392D8[slot]&=~0x400;
                else if(kind==3) D_801392D8[slot]&=~0x800;
                goto dispose;
            }
            direction[0]=FIELD(object,f32,20);
            direction[1]=FIELD(object,f32,24);
            direction[2]=FIELD(object,f32,28);
            func_8008E0B8(direction);
            vector_normalize_length(direction,&matrix[0][0]);
            math_utility(&matrix[0][0],&node->matrix[0][0]);
            if(magnitude<90) {
                if(angle<0) scale=((f32)magnitude/70.0f)*0.75f+0.25f;
                else scale=((f32)magnitude/90.0f)*D_80123FC8+drift;
                for(i=0;i<3;i++) for(j=0;j<3;j++) node->matrix[i][j]*=scale;
            }
            node->position[0]=FIELD(object,f32,116+kind*12);
            node->position[1]=FIELD(object,f32,120+kind*12);
            node->position[2]=FIELD(object,f32,124+kind*12);
            node->position[0]+=node->velocity[0];
            node->position[1]+=node->velocity[1];
            node->position[2]+=node->velocity[2];
            node->position[1]+=1.25f;
            node->period-=D_8002EB94;
            if(node->period<=0.0f) {
                FIELD(D_8012E700+FIELD(node,s16,54)*68,u16,20)=D_8014295A[node->animation];
                node->animation++;
                if(node->animation>=8) node->animation=0;
                node->period=D_80123FCC;
            }
        } else if(kind==5) {
            car=D_8014A250+slot*2056;
            if(FIELD(car,s8,1600)==0) goto dispose;
            entity=D_8012E700+node->entity*68;
            if(node->animation==-1) {
                if(FIELD(entity,f32,12)<node->lifetime) FIELD(entity,f32,12)+=D_80123FD0;
                else node->animation=0;
            } else {
                if(node->animation<8) FIELD(entity,f32,12)-=step;
                else FIELD(entity,f32,12)+=step;
                if(node->animation<14) node->animation++;
                else node->animation=0;
            }
            object=D_80152818+slot*952;
            position[0]=FIELD(object,f32,8);
            position[1]=FIELD(object,f32,12);
            position[2]=FIELD(object,f32,16);
            scale=D_8011744C[FIELD(car,u8,8)];
            position[0]+=node->velocity[0]*FIELD(object,f32,44);
            position[1]+=node->velocity[0]*FIELD(object,f32,48);
            position[2]+=node->velocity[0]*FIELD(object,f32,52);
            position[0]+=node->velocity[2]*FIELD(object,f32,68);
            position[1]+=node->velocity[2]*FIELD(object,f32,72);
            position[2]+=node->velocity[2]*FIELD(object,f32,76);
            position[0]+=scale*FIELD(object,f32,56);
            position[1]+=scale*FIELD(object,f32,60);
            position[2]+=scale*FIELD(object,f32,64);
            node->position[0]=position[0];
            node->position[1]=position[1];
            node->position[2]=position[2];
        } else {
            node->lifetime-=D_8002EB94;
            if(node->lifetime<=0.0f) goto dispose;
            if(node->lifetime<1.0f) {
                entity=D_8012E700+node->entity*68;
                if(FIELD(entity,u8,63)>=9) FIELD(entity,u8,63)-=9;
            }
            if(kind==4) {
                seed=(u32)D_8011735C*1103515245+12345;D_8011735C=seed;
                node->position[0]+=(f32)(((s32)seed>>16)&0x7fff)*D_80123FD4/32768.0f-drift;
                seed=(u32)D_8011735C*1103515245+12345;D_8011735C=seed;
                node->position[1]+=(f32)(((s32)seed>>16)&0x7fff)*0.25f/32768.0f;
                seed=(u32)D_8011735C*1103515245+12345;D_8011735C=seed;
                node->position[2]+=(f32)(((s32)seed>>16)&0x7fff)*drift/32768.0f-D_80123FD8;
                FIELD(D_8012E700+node->entity*68,f32,12)+=D_80123FDC;
            }
            node->period-=D_8002EB94;
            if(node->period<=0.0f) {
                if(D_80156994 || D_8014978C>=6) {
                    FIELD(D_8012E700+FIELD(node,s16,54)*68,u16,20)=D_80142948[node->animation];
                    if(node->flags&1) {
                        node->animation++;
                        if(node->animation>=8) node->animation=0;
                    } else {
                        node->animation--;
                        if(node->animation<0) node->animation=7;
                    }
                }
                node->period=D_80123FE0;
            }
            if(kind>=6) FIELD(D_8012E700+node->entity*68,f32,12)+=step;
            else FIELD(D_8012E700+node->entity*68,f32,12)+=D_80123FE4;
            node->position[0]+=node->velocity[0]*D_8002EB94;
            node->position[1]+=node->velocity[1]*D_8002EB94;
            node->position[2]+=node->velocity[2]*D_8002EB94;
        }
        node=next;
        continue;
    dispose:
        entity_spawn_callback(FIELD(node,s16,54),0,0);
        func_800AFA84(D_8013F1E0,node);
        node=next;
    }
    }
}
