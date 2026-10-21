/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* func_800D816C = updateButtons@controlscr.c (w36k/w36n: proven), not name entry; source not
 * original-source recovery. Genuine D91A0 caller and D8154 predicate retained.
 * Stock IDO 5.3: strict MATCH, 699 words; frame232 and owned literals agree.
 * Group/private ABI only: caller D91A0 remains nonmatching; no install claim.
 *
 * Disclosed shaping: four unused scalar homes (span, distance, current_y,
 * frame_residual3), 16 bytes total; deleting all four gives frame216 and
 * 20/699 differences. Three are historical carrier slots after their values
 * were restored to repeated expressions. Five compiled-out reads below set
 * constant/index/spacing/count web priorities; no original spelling is asserted.
 * The first-loop count increment follows the conditional before increment.
 * as1 hoists it and leaves a distinct empty target block, preventing the second
 * branch delay-copy. The following no-code count read restores its priority.
 * D8002EB94 and D80140BDC volatile views follow accepted sibling interfaces
 * and native repeated reads, not proof of asynchronous writers. See RESULTS.
 */
typedef signed char s8; typedef unsigned char u8;
typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct Item { s32 type, handle; f32 angle, matrix[3][3], position[3]; u8 alpha, pad[3]; } Item;
typedef union Color { u32 word; u8 channel[4]; } Color;
typedef struct Node {u8 pad[60]; u32 color; u8 tail[4];} Node;
extern Item D_80111998[4][17];
extern Node D_8012E700[];
extern f32 D_8011418C[3][3];
extern volatile f32 D_8002EB94;
extern f32 D_80112A9C,D_80112AA0,D_80112AA4,D_80112AA8,D_80112AAC,D_80112AB0,D_80112AB4;
extern s32 D_80154618[],D_80154630[],D_801140F0;
extern f32 D_80154648[],D_80154FC0[];
extern s16 D_8014A0F8[];
extern volatile u8 D_80140BDC;
extern char D_80120270[],D_80120280[];
void func_8008B32C(f32[3][3],f32[3][3],f32);
s32 func_800D8154(s8,s32);
void func_8008D870(s16,s32,s32);
typedef struct NameEntry NameEntry;
NameEntry *func_800B24EC(char *,s16 *,s8,s8,s32);
void func_800B5898(f32,f32[3][3]);
void func_800B5940(f32,f32[3][3]);
void *memcpy(void *,const void *,u32);
void model_data_load(s32,s32,u32);
void model_transform_setup(s32,s32,u32);
f32 fabsf(f32);
#pragma intrinsic(fabsf)
#define PI 3.1415927f
#define HALF_PI 1.5707964f
#define TWO_PI 6.2831855f
#define FOUR_PI 12.566371f

void func_800D816C(s8 player) {
    s8 settled;
    s32 count;
    s32 before;
    s32 i;
    s32 j;
    f32 previous[3];
    f32 span;
    f32 selected_y;
    f32 y;
    f32 distance;
    f32 current_y;
    s16 texture;
    Color color;
    s32 tex;
    s32 valid;
    s32 shown;
    s32 mask;
    s16 handle;
    f32 spacing;
    f32 top;
    s32 frame_residual3;

    settled = 1;
    color.word = D_801140F0;
    if (D_80154618[player] != 0) {
        count = 0;
        before = 0;
        for (j = 0; j != 12; j++) {
            if (j == 10) {}
            if (func_800D8154(player,j)) {
                if (j < D_8014A0F8[player]) before++;
                count++;
                if (count) {}
            }
        }
        if ((D_80112AA8 - D_80112AAC) < (count-1) * D_80112AA0 * D_80112A9C) {
            selected_y = D_80112AA8 - (before * (D_80112AA8 - D_80112AAC))/(count-1);
        } else {
            selected_y = D_80112AA8 - before * D_80112AA0 * D_80112A9C;
        }
        memcpy(previous, D_80111998[player][14].position, 12);
        D_80111998[player][14].angle += TWO_PI * D_8002EB94;
        if (D_80111998[player][14].angle > TWO_PI) D_80111998[player][14].angle -= TWO_PI;
        D_80111998[player][14].position[0] = D_80112AA4 * D_80112AB4 / D_80112AB0 + -110.0f * D_80112A9C;
        D_80111998[player][14].position[1] = selected_y * D_80112AB4 / D_80112AB0;
        D_80111998[player][14].position[2] = D_80112AB4;
        if (!D_80154630[player]) {
            if (D_80154648[player] < fabsf(D_80111998[player][14].position[1] - previous[1]) * 60 * D_80112A9C / 10) {
                D_80154648[player] = fabsf(D_80111998[player][14].position[1] - previous[1]) * 60.0f * D_80112A9C / 10.0f;
                D_80154648[player] = D_80154648[player] < 540.0f * D_80112A9C ? D_80154648[player] : 540.0f * D_80112A9C;
                D_80154648[player] = D_80154648[player] > 180.0f * D_80112A9C ? D_80154648[player] : 180.0f * D_80112A9C;
            }
            if (D_80154648[player] * D_8002EB94 + previous[1] < D_80111998[player][14].position[1]) {
                D_80111998[player][14].position[1] = previous[1] + D_80154648[player] * D_8002EB94;
            } else if (previous[1] - D_80154648[player] * D_8002EB94 > D_80111998[player][14].position[1]) {
                D_80111998[player][14].position[1] = previous[1] - D_80154648[player] * D_8002EB94;
            } else {
                D_80154648[player] = 180.0f * D_80112A9C;
            }
        }
        func_8008B32C(D_8011418C,D_80111998[player][14].matrix,D_80112A9C * 0.4f);
        func_800B5940(D_80111998[player][14].angle,D_80111998[player][14].matrix);
        func_800B5898(HALF_PI,D_80111998[player][14].matrix);
        y = before * D_80112AA0 * D_80112A9C + selected_y;
        for (i = 0; i != 12; i++) {
            if (i == 10) {}
            if (i == D_8014A0F8[player]) {
                if (D_80111998[player][i].angle < PI) D_80111998[player][i].angle += FOUR_PI * D_8002EB94;
                if (D_80111998[player][i].angle > PI) D_80111998[player][i].angle = PI;
                if (D_80154630[player]) D_80111998[player][i].angle = PI;
            } else {
                if (D_80111998[player][i].angle > 0.0f) D_80111998[player][i].angle -= FOUR_PI * D_8002EB94;
                if (D_80111998[player][i].angle < 0.0f) D_80111998[player][i].angle = 0.0f;
                if (D_80154630[player]) D_80111998[player][i].angle = 0.0f;
            }
            memcpy(previous, D_80111998[player][i].position, 12);
            D_80111998[player][i].position[0] = -40.0f * D_80112A9C + D_80112AA4;
            D_80111998[player][i].position[1] = y;
            D_80111998[player][i].position[2] = D_80112AB0;
            if (i == 10) D_80111998[player][i].position[0] -= 20 * D_80112A9C;
            else if (i == 11) D_80111998[player][i].position[0] += 20.0f * D_80112A9C;
            if (!D_80154630[player]) {
                if (D_80154FC0[player] < fabsf(D_80111998[player][i].position[1] - previous[1]) * 60 / 10) {
                    D_80154FC0[player] = fabsf(D_80111998[player][i].position[1] - previous[1]) * 60.0f / 10.0f;
                    D_80154FC0[player] = D_80154FC0[player] < 540.0f * D_80112A9C ? D_80154FC0[player] : 540.0f * D_80112A9C;
                    D_80154FC0[player] = D_80154FC0[player] > 180.0f * D_80112A9C ? D_80154FC0[player] : 180.0f * D_80112A9C;
                }
                if (D_80154FC0[player] * D_8002EB94 + previous[1] < D_80111998[player][i].position[1]) {
                    D_80111998[player][i].position[1] = previous[1] + D_80154FC0[player] * D_8002EB94;
                    settled = 0;
                } else if (previous[1] - D_80154FC0[player] * D_8002EB94 > D_80111998[player][i].position[1]) {
                    D_80111998[player][i].position[1] = previous[1] - D_80154FC0[player] * D_8002EB94;
                    settled = 0;
                }
            }
            func_8008B32C(D_8011418C,D_80111998[player][i].matrix,D_80112A9C);
            func_800B5898(D_80111998[player][i].angle-HALF_PI,D_80111998[player][i].matrix);
            if (D_80111998[player][i].angle > HALF_PI) tex=(s32)func_800B24EC(D_80120270,&texture,0,(s8)(D_80140BDC-1),1);
            else tex=(s32)func_800B24EC(D_80120280,&texture,0,(s8)(D_80140BDC-1),1);
            func_8008D870((s16)D_80111998[player][i].handle,(s32)tex,-1);
            if (i == 10) {}
            top = D_80112AA8;
            spacing = D_80112AA0*D_80112A9C;
            if (spacing) {}
            if (D_80111998[player][i].position[1] > top + spacing || D_80111998[player][i].position[1] < D_80112AAC-spacing) D_80111998[player][i].alpha=0;
            else if (D_80111998[player][i].position[1] > D_80112AA8) D_80111998[player][i].alpha=255-(D_80111998[player][i].position[1]-D_80112AA8)*255.0f/(spacing);
            else if (D_80111998[player][i].position[1] < D_80112AAC) D_80111998[player][i].alpha=255-(D_80112AAC-D_80111998[player][i].position[1])*255.0f/(spacing);
            else D_80111998[player][i].alpha=255;
            valid=func_800D8154(player,i);
            if(!valid)D_80111998[player][i].alpha=0;
            color.channel[3]=D_80111998[player][i].alpha;
            handle = D_80111998[player][i].handle;
            D_8012E700[handle].color=color.word;
            if (i==10) {
                D_80111998[player][12].position[0]=D_80111998[player][i].position[0];
                D_80111998[player][12].position[1]=D_80111998[player][i].position[1];
                D_80111998[player][12].position[2]=D_80111998[player][i].position[2];
                D_80111998[player][12].position[0]+=180.0f*D_80112A9C;
                func_8008B32C(D_8011418C,D_80111998[player][12].matrix,D_80112A9C);
                func_800B5898(D_80111998[player][i].angle-HALF_PI,D_80111998[player][12].matrix);
                func_8008D870((s16)D_80111998[player][12].handle,(s32)tex,-1);
                handle = D_80111998[player][12].handle;
                D_8012E700[handle].color=color.word;
            }
            shown=func_800D8154(player,i);
            if(!shown) {
                model_data_load(D_80111998[player][i].handle,1,15);
                if(i==10)model_data_load(D_80111998[player][12].handle,1,15);
            } else if(!D_80111998[player][i].alpha) {
                model_data_load(D_80111998[player][i].handle,1,15);
                if(i==10)model_data_load(D_80111998[player][12].handle,1,15);
                y-=D_80112AA0*D_80112A9C;
            } else {
                mask=1<<player;
                model_transform_setup(D_80111998[player][i].handle,0,mask);
                if(i==10)model_transform_setup(D_80111998[player][12].handle,0,mask);
                y-=D_80112AA0*D_80112A9C;
            }
        }
        if(settled==1)D_80154FC0[player]=0.0f;
        D_80154630[player]=0;
    }
}
/* w36k note: identity updateButtons@controlscr.c (MAT3 controlscr.c updateButtons, GC 0x8006D26C). Body is
 * Astra's PR #322 packet unchanged; it now matches inside the w36k group whose caller func_800D91A0 also matches.
 * The 'nonmatching caller / no install claim' lines above are historical. */
