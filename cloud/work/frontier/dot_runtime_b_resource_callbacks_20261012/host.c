/* Logical-host fixtures of the actual source. These do not emulate N64, prove
 * native layout, or reinterpret the union as target-endian storage. The two
 * observed handle views are captured independently using host C's own union
 * representation. Arbitrary native scene/halfword alias equivalence is not
 * claimed. Pointer-width unsigned long preserves the source round-trip here. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "group.c"
State D_80399AE0;
f32 D_8002EB94;
u16 D_80142A82, D_80142A84, D_80142A8E;
s16 D_8014A108;
Player D_80152818[4];
static Object objects[4], spare;
static Slot slots[4];
static int random_index, rearming, variant, mutate;
static int random_calls, setter_calls, show_calls;
static float random_argument;
static s16 setter_index;
static u16 setter_value;
static s32 show_scene;
static int failures;

static void require(int condition) { if (!condition) failures++; }

f32 func_8008B2E4(f32 range)
{
    random_calls++;
    random_argument = range;
    return rearming ? range * 0.25f : (f32)random_index;
}

void func_80090770(s16 index, u16 value)
{
    int selected;
    setter_calls++;
    setter_index=index;
    setter_value=value;
    selected=variant==0 ? D_80399AE0.selected_b : variant==1 ? D_80399AE0.selected_delay : D_80399AE0.selected_a;
    if (mutate) D_80399AE0.slots[selected].object=&spare;
}

void model_transform_setup(s32 scene, s32 mode, u32 views)
{
    require(mode==0 && views==15);
    show_calls++;
    show_scene=scene;
}

static void setup(int n, int flags, int seed)
{
    int j;
    memset(&D_80399AE0,0xa5,sizeof(D_80399AE0));
    D_80399AE0.count=(s8)n;
    D_80399AE0.selected_a=D_80399AE0.selected_b=D_80399AE0.selected_delay=0;
    D_80399AE0.phase_a=D_80399AE0.phase_b=D_80399AE0.delay=13.0f;
    D_80399AE0.slots=slots;
    D_8002EB94=0.5f;
    D_80142A82=153;D_80142A84=154;D_80142A8E=159;
    memset(&spare,0xb7,sizeof(spare));spare.handle.scene=0x123456;
    for(j=0;j<4;j++) {
        memset(&objects[j],0x59,sizeof(Object));
        objects[j].flags=(u8)(0x40 | flags);
        objects[j].resource=(s16)(seed+j);
        objects[j].handle.scene=0x1010000+j*0x10001;
        memset(&slots[j],0x73,sizeof(Slot));slots[j].object=&objects[j];
        slots[j].active=(s16)(2+(seed+j)%3);
        slots[j].count=(s16)(1+(seed+j)%2);
    }
    slots[n-1].active=1;slots[n-1].count=1;
    random_calls=setter_calls=show_calls=0;
    random_argument=0;setter_index=0;setter_value=0;show_scene=0;
}

static int selection_matrix(void)
{
    static const float timers[5]={-2.0f,-1.0f,0.0f,0.5f,2.0f};
    int n,k,t,start,flag,m,seed,j,cases=0,kind,selected,expected_calls;
    State expected_state;
    Slot expected_slots[4];
    Object expected_objects[4];
    float *timer,*expected_timer,range,offset;
    s8 *selector,*expected_selector;
    s16 expected_setter;
    s32 expected_show;
    u16 texture;
    for(n=1;n<=4;n++)for(k=0;k<3;k++)for(t=0;t<5;t++)
    for(start=0;start<n;start++)for(flag=0;flag<2;flag++)for(m=0;m<2;m++) {
        seed=n+k+t+start+flag;setup(n,flag*2,seed);
        variant=k;mutate=m;random_index=start;
        kind=k==0?3:k==1?4:2;range=k==1?10.0f:60.0f;offset=k==1?5.0f:90.0f;
        texture=k==0?D_80142A8E:k==1?D_80142A82:D_80142A84;
        timer=k==0?&D_80399AE0.phase_b:k==1?&D_80399AE0.delay:&D_80399AE0.phase_a;
        selector=k==0?&D_80399AE0.selected_b:k==1?&D_80399AE0.selected_delay:&D_80399AE0.selected_a;
        *timer=timers[t];rearming=t==0;
        expected_state=D_80399AE0;
        memcpy(expected_slots,slots,sizeof(slots));memcpy(expected_objects,objects,sizeof(objects));
        expected_timer=k==0?&expected_state.phase_b:k==1?&expected_state.delay:&expected_state.phase_a;
        expected_selector=k==0?&expected_state.selected_b:k==1?&expected_state.selected_delay:&expected_state.selected_a;
        expected_calls=0;expected_setter=0;expected_show=0;selected=-1;
        if(t==0) {
            if(!flag) {
                *expected_timer=range*0.25f+offset;
                for(j=0;j<n;j++)if(expected_slots[j].active==kind)expected_slots[j].active=1;
                expected_slots[*selector].active=(s16)kind;expected_slots[*selector].count=1;
                expected_calls=1;
            }
        } else {
            *expected_timer-=0.5f;
            if(*expected_timer<0.0f) {
                selected=start;
                for(j=0;j<n;j++) {
                    int index=(start+j)%n;
                    if(expected_slots[index].count==1 && expected_slots[index].active!=kind) {selected=index;break;}
                }
                require(j<n);
                *expected_selector=(s8)selected;
                expected_slots[selected].active=1;expected_slots[selected].count=(s16)kind;
                expected_objects[selected].flags|=2;
                expected_objects[selected].resource=(s16)(k==0?359:k==1?353:354);
                expected_setter=objects[selected].handle.parts.index;
                expected_show=m?spare.handle.scene:objects[selected].handle.scene;
                if(m)expected_slots[selected].object=&spare;
                *expected_timer=-2.0f;expected_calls=1;
            }
        }
        if(k==0)func_80390B10();else if(k==1)func_80390D38();else func_80390B08();
        require(!memcmp(&expected_state,&D_80399AE0,sizeof(State)));
        require(!memcmp(expected_slots,slots,sizeof(slots)));
        require(!memcmp(expected_objects,objects,sizeof(objects)));
        require(random_calls==expected_calls);
        if(expected_calls)require(random_argument==(t==0?range:(float)n));
        require(setter_calls==(selected>=0) && show_calls==(selected>=0));
        if(selected>=0)require(setter_index==expected_setter && setter_value==texture && show_scene==expected_show);
        cases++;
    }
    return cases;
}

static int alpha_matrix(void)
{
    static const u8 table[8]={16,32,64,96,112,144,176,176};
    int n,phase,countdown,alpha,t,i,value,cases=0;
    Player expected[4];
    for(n=0;n<=4;n++)for(phase=0;phase<4;phase++)for(countdown=0;countdown<8;countdown++)
    for(alpha=0;alpha<256;alpha+=17)for(t=0;t<2;t++) {
        memset(D_80152818,0x69,sizeof(D_80152818));D_8014A108=(s16)n;D_8002EB94=0.5f;
        for(i=0;i<4;i++) {
            D_80152818[i].flags=(u32)(0x400 | (i&1));
            D_80152818[i].phase=(s8)phase;D_80152818[i].countdown=(s8)countdown;
            D_80152818[i].alpha=(u8)alpha;D_80152818[i].timer=t?1.0f:0.0f;
        }
        memcpy(expected,D_80152818,sizeof(expected));
        for(i=0;i<n;i++) {
            Player *p=&expected[i];
            if(p->flags&1) {
                if(phase==3)p->alpha=table[countdown];
                else {
                    p->countdown=0;
                    if(t) {
                        if(phase==1) {
                            value=(alpha-8)&255;p->alpha=(u8)value;
                            if(value<=16){p->alpha=16;p->phase=3;}
                        } else {value=alpha+8;p->alpha=(u8)(value>255?255:value);}
                        p->timer-=0.5f;
                    } else if(phase==1)p->phase=3;
                    else {p->phase=0;p->flags&=~1U;}
                }
            }
            if(p->countdown>0)p->countdown--;
        }
        func_80390B00();require(!memcmp(expected,D_80152818,sizeof(expected)));cases++;
    }
    return cases;
}

int main(void)
{
    int selections=selection_matrix(),alphas=alpha_matrix();
    if(failures){fprintf(stderr,"%d failures\n",failures);return 1;}
    printf("{\"selection_cases\":%d,\"alpha_cases\":%d,\"helper_pointer_mutation\":true,\"native_execution\":false,\"target_layout_claimed\":false}\n",selections,alphas);
    return 0;
}
