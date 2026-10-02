/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef int s32;typedef unsigned int u32;
typedef struct Config {void *name;u32 p4;void *selection;s16 key;u8 p14[42];} Config;
extern u8 D_80140BDC;
void *func_800B24EC(void *,s16 *,s8,s8,s32);
void collision_sound_play(void *);void Input_ApplyPadConfig(void *);
void func_800EF5B0(void *object,void *name,s32 direct) {
 Config *config=object;
 config->name=name;
 if(direct)config->selection=func_800B24EC(name,&config->key,0,(s8)(D_80140BDC-1),1);
 else collision_sound_play(config);
 Input_ApplyPadConfig(config);
}
