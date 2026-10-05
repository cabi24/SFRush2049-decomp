typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32;

typedef struct {
    u8 pad0[20];
    s32 model;      /* 20 */
    u8 pad24[40];
} CarPart;          /* 64 */

extern CarPart D_80139320[4][13];
extern char *D_8011B438[];
extern volatile u8 D_80140BDC;

void *func_800B24EC(char *name, u16 *index, s8 lo, s8 hi, s32 err);
void func_8008D870(s16 object, void *tex, s32 mode);
void model_data_load(s32 model, s32 mode, s32 mask);
void model_transform_setup(s32 model, s32 mode, s32 mask);

void sfx_stop(s16 part, s16 car, s16 tex)
{
    s16 mask;
    u16 index;
    CarPart *p;

    if (tex < -1 || tex >= 11) {
        return;
    }
    switch (car) {
    case 0:
        mask = 1;
        break;
    case 1:
        mask = 2;
        break;
    case 2:
        mask = 4;
        break;
    case 3:
        mask = 8;
        break;
    }
    if (tex == -1) {
        model_data_load(D_80139320[car][part].model, 0, mask);
    } else {
        void *t = func_800B24EC(D_8011B438[tex], &index, 0, D_80140BDC - 1, 1);
        p = &D_80139320[car][part];
        func_8008D870(p->model, t, -1);
        model_transform_setup(p->model, 0, mask);
    }
}
