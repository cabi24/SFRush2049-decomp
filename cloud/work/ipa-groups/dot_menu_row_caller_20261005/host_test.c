/* Behavioral checks for the source, not a replacement for native-byte proof. */
#include <assert.h>
#include <string.h>
#include "group.c"
Color4 D_801146BC, D_801146C0, D_80118E28, D_80118E2C;
s32 D_80116D0C, D_80118E20, D_80118E24;
s16 D_80116D9C, D_8013FEC8;
OptionRow D_8011650C[14];
void *D_801164D0[4];
s8 D_80146108[19];
MenuAssets D_8017A4E0;
f32 D_80116CFC[3];
u8 D_80150B70[152], D_801461D0[24];
static int modes[40], mode_count, draws;
static struct { int x, y; void *object; } draw_log[40];
static int selects[4], select_count, locks, releases, complete, projections;
void dispatch_handler(s32 mode) { modes[mode_count++] = mode; }
void state_utility(s16 x, s16 y, void *object) {
    draw_log[draws].x=x; draw_log[draws].y=y; draw_log[draws++].object=object;
}
void render_helper(f32 value) { (void)value; }
void brake_light_update(s32 view, f32 *position, void *camera, f32 *scale, s16 *screen) {
    assert(view==0 && camera==D_80150B70 && scale==0);
    screen[0]=(s16)position[0]; screen[1]=(s16)position[1]; projections++;
}
s32 osRecvMesg(void *queue, void *message, s32 flag) {
    assert(queue==D_801461D0 && message==0 && flag==1); locks++; return 0;
}
s32 osJamMesg(void *queue, void *message, s32 flag) {
    assert(queue==D_801461D0 && message==0 && flag==0); releases++; return 0;
}
s32 slot_state_setup(s32 selector) { selects[select_count++]=selector; return 0; }
u32 object_manager_update(void *object, s16 limit) {
    assert(object==D_8017A4E0.labels[50] && limit==-1); return 10;
}
void func_800ED66C(f32 alpha) { assert(alpha==-1.0f || (alpha>=0.0f && alpha<=255.0f)); }
void particle_velocity_set(void) { complete++; }
static void reset(void) {
    mode_count=draws=select_count=locks=releases=complete=projections=0;
}
int main(void) {
    static int storage[128];
    static void *labels[64], *values[64];
    static ResourceHeader header;
    const int selector[14]={-1,10,0,1,2,3,4,5,6,7,8,9,18,11};
    const int label_index[14]={25,13,15,16,17,18,19,20,21,22,23,24,26,14};
    int i, j, mode, index, selected, angle, tests=0;
    for(i=0;i<64;i++) { labels[i]=&storage[i]; values[i]=&storage[64+i]; }
    for(i=0;i<19;i++) D_80146108[i]=(s8)i;
    D_8017A4E0.labels=labels; D_8017A4E0.values=values;
    D_8017A4E0.header=&header; header.string_offset=3;
    D_8013FEC8=2; D_801164D0[2]=&storage[127];
    D_801146BC.rgba=0x12345678; D_801146C0.rgba=0x9abcdef0;
    for(mode=0;mode<3;mode++) for(index=0;index<14;index++)
    for(selected=0;selected<14;selected++) for(angle=0;angle<3;angle++) {
        reset(); D_80116D0C=mode; D_80116D9C=selected;
        D_8011650C[index].angle=angle==0 ? 0.0f : angle==1 ? 1.57079637f : 3.0f;
        D_80118E28.rgba=0; D_80118E2C.rgba=0;
        func_8010A7A4(index, 200, -10, labels[0], values[0]);
        assert(mode_count==1 && draws==2);
        assert(modes[0]==(mode==1 ? (angle==0 ? 1 : 22) : (index==selected ? 22 : 1)));
        assert(draw_log[0].x==95 && draw_log[1].x==240);
        assert(draw_log[0].y==-10 && draw_log[1].y==-10);
        assert(draw_log[0].object==labels[0] && draw_log[1].object==values[0]);
        assert(D_80118E28.rgba==(mode==1 ? D_801146BC.rgba : 0));
        assert(D_80118E2C.rgba==(mode==1 ? D_801146C0.rgba : 0)); tests++;
    }
    for(j=0;j<5;j++) {
        reset(); D_80116D0C=0; D_80116D9C=5;
        D_80116CFC[0]=160; D_80116CFC[1]=20;
        for(i=0;i<14;i++) {
            D_8011650C[i].angle=j==0 ? 0.0f : j==1 ? 1.0f : j==2 ? 3.0f : j==3 ? 0.69813168f : 2.44346094f;
            D_8011650C[i].alpha=(j==2 && i%2) ? 0 : 255;
            D_8011650C[i].position[0]=(float)(100+i);
            D_8011650C[i].position[1]=(float)(50+i);
        }
        func_8010A8D0();
        assert(locks==2 && releases==2 && select_count==2 && selects[0]==13 && selects[1]==11);
        assert(complete==1 && D_80118E20==0 && D_80118E24==3);
        assert(draw_log[0].x==155 && draw_log[0].y==20 && draw_log[0].object==labels[50]);
        index=1;
        for(i=0;i<14;i++) {
            if(j==1 || (j==2 && i%2)) continue;
            assert(draw_log[index].x==45+i && draw_log[index+1].x==190+i);
            assert(draw_log[index].y==50+i && draw_log[index+1].y==50+i);
            assert(draw_log[index].object==labels[label_index[i]]);
            assert(draw_log[index+1].object==(i==0 ? D_801164D0[2] : values[selector[i]+3]));
            index+=2;
        }
        assert(draws==index && projections==1+(index-1)/2); tests++;
    }
    assert(tests==1769);
    return 0;
}
