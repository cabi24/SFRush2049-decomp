typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define TRUE 1
typedef struct { u32 opaque[6]; } OSMesgQueue;   /* 24-byte libultra queue */
extern OSMesgQueue D_801461D0;
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osJamMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
