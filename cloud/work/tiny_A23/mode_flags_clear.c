typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct {u8 first,second;} Flags;
extern Flags D_80110680;
void audio_dsp_process(void);
void mode_flags_clear(void) {D_80110680.first=0;D_80110680.second=0;audio_dsp_process();}
