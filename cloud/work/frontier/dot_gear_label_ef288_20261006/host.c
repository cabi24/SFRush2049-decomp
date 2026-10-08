/* Contract-hook host harness for the gear callback only. */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "candidate.c"
GearModel D_8014A250[4];
GearCar D_80152818[4];
GearPosition D_80115BE8[4][4];
GearAssets D_8017A4E0;
s16 D_80151AD0;
s8 D_80161394;
u8 D_801461D0[24];
static const char label0[] = "gear-A", label1[] = "gear-B";
static const char *labels[205];
static int *out;
static const int *input;
static int event_count, width_count, draw_count;
static void event(int type, int a, int b, int c) {
    int *p = out + 4 * event_count++;
    p[0]=type; p[1]=a; p[2]=b; p[3]=c;
}
void render_helper(float x) {
    event(1, x == 0.0f ? 0 : -1, 0, 0);
    if (x == 0.0f && input[5] == 1) D_80151AD0 = input[6];
}
s32 osRecvMesg(void *q, void *m, s32 flag) {
    if (q != D_801461D0 || m || flag != 1) return -901;
    event(2, 1, 0, 0); return 123;
}
s32 slot_state_setup(s32 s) { event(3, s, 0, 0); return -17; }
s32 osJamMesg(void *q, void *m, s32 flag) {
    if (q != D_801461D0 || m || flag) return -902;
    event(4, 0, 0, 0);
    if (input[5] == 2) D_80151AD0=input[6];
    return 456;
}
void dispatch_handler(s32 color) { event(5,color,0,0); }
u32 object_utility(const char *label, s32 limit) {
    u32 width = width_count++ & 1 ? (u32)input[8] : (u32)input[7];
    event(6,label == label0 ? 0 : 1,limit,(s32)width);
    if (input[5] == 3) labels[204] = label1;
    return width;
}
void state_utility(s16 x,s16 y,const char *text) {
    int content = text == label0 ? 1000 : text == label1 ? 1001 : (u8)text[0];
    event(7,x,y,content);
    draw_count++;
    if (draw_count == 1 && input[5] == 4) {
        D_8014A250[0].gear=7;
        D_80115BE8[D_80151AD0-1][0].x=input[9];
        D_80115BE8[D_80151AD0-1][0].y=input[10];
    }
    if (draw_count == 4 && input[5] == 5) {
        D_80151AD0=input[6];
    }
}
int run_case(const int *args,int *events) {
    int i,j,ret;
    input=args;out=events;event_count=width_count=draw_count=0;
    memset(D_8014A250,0,sizeof(D_8014A250));
    memset(D_80152818,0,sizeof(D_80152818));
    D_80151AD0=args[0];D_80161394=args[1];
    for(i=0;i<4;i++) {
        D_8014A250[i].hidden=(args[2]>>i)&1;
        D_8014A250[i].gear=args[11+i];
        D_8014A250[i].car_index=3-i;
        D_80152818[3-i].kind=(args[3]>>i)&1;
        for(j=0;j<4;j++) {
            D_80115BE8[i][j].x=(s32)((u32)args[4]+i*43U+j*17U);
            D_80115BE8[i][j].y=(s16)((u32)args[4]+i*23U+j*11U);
        }
    }
    labels[204]=label0;D_8017A4E0.labels=labels;
    ret=func_800EF288(0xFEDCBA98U);
    event(8,ret,D_80151AD0,0);
    return event_count;
}
typedef char model_size[sizeof(GearModel)==0x808 ? 1 : -1];
typedef char gear_offset[offsetof(GearModel,gear)==0x730 ? 1 : -1];
typedef char index_offset[offsetof(GearModel,car_index)==0x7C6 ? 1 : -1];
typedef char car_size[sizeof(GearCar)==0x3B8 ? 1 : -1];
typedef char kind_offset[offsetof(GearCar,kind)==0xEF ? 1 : -1];
typedef char position_size[sizeof(GearPosition)==8 ? 1 : -1];
#ifdef STANDALONE
int main(void) {
    int input[15]={4,1,0,0,32767,0,2,-1,123,300,-300,-1,0,3,127};
    int events[512];
    int i;
    for(i=0;i<4000;i++) { input[5]=i%6; input[4]=i*997-500000;run_case(input,events); }
    return 0;
}
#endif
