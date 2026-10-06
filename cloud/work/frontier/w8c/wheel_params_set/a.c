typedef signed char s8;
typedef unsigned char u8;
typedef unsigned int u32;

typedef struct List16 { u8 indirect, doubly, pad2[2]; u32 count; void *head, *tail; } List16;

extern volatile s8 D_8011028C;
extern s8 D_80110284;
extern volatile List16 D_80146160;
extern u32 D_80144C48, D_80146100;
void func_800958B8(void);
void audio_effect_process(u32 address);

void wheel_params_set(void)
{
    func_800958B8();
    while (D_8011028C != 0) {}
    while (D_80146160.head != 0) {}
    D_80110284 = 0;
    audio_effect_process(D_80144C48);
    audio_effect_process(D_80146100);
}
