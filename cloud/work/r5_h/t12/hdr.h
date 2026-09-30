typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct SNode { u8 pad0[52]; u16 slot; u8 pad36[6]; struct SNode *next; } SNode;
typedef struct { u8 pad[22]; u8 state; u8 pad2[9]; } SSlot;
extern s32 D_80149788;
extern SNode *D_80149450[];
extern SSlot D_80140BF0[];
