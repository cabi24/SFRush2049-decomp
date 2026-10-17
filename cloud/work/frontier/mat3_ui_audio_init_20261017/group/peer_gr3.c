/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Bank {
    /* 0x0 */ u8 pad0;
    /* 0x1 */ u8 mode;
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ u8 f4;
    /* 0x5 */ u8 pad5;
    /* 0x6 */ u8 f6;
    /* 0x7 */ u8 f7;
    /* 0x8 */ u8 f8;
    /* 0x9 */ s8 f9;
    /* 0xA */ u8 f10;
    /* 0xB */ u8 f11;
    /* 0xC */ u8 f12;
} Bank;

typedef struct { u8 *ptr; u8 id; u8 pad[7]; } Entry;
typedef struct { Bank *bank; } Hdr;
typedef struct { Hdr *hdr; s32 pad[4]; } Tbl;
typedef struct { s32 first; s32 second; } Pair;

extern Bank *D_801497F0;
extern s32 D_80149780;
extern s32 D_801497A4;
extern Tbl D_80156D44[];
extern Entry *D_80149800;
extern s16 D_80149878[256];
extern s32 D_80149820[];
extern Pair gTexDefPtrs[];
extern s8 D_80149B60;
extern u8 D_80149B70;
extern s32 D_80149B08;
extern s32 D_80149B28;

void func_80096288(s32 a,s32 b,s32 c) ;
extern s32 func_80097694(s32, s32);

extern int __unit_dead[];
void sound_update_channel(s32 force)
;


void mode_byte2_set(s16 a) ;
u8 object_type_byte2_get(void) ;
u8 object_type_byte3_get(void) ;

void *slot_value_get(s32 arg) ;

void mode_byte_set(s16 a) ;

s8 object_byte9_set(s8 value) {
 s8 old;
 sound_update_channel(0);
 old = D_801497F0->f9;
 D_801497F0->f9 = value;
 return old;
  if (0) { /* w27b: dead block, stops umerge inlining this context copy (blob_unit's inline-blocker device) */
    __unit_dead[0] = 1;
    __unit_dead[1] = 2;
    __unit_dead[2] = 3;
    __unit_dead[3] = 4;
    __unit_dead[4] = 5;
    __unit_dead[5] = 6;
    __unit_dead[6] = 7;
    __unit_dead[7] = 8;
  }

}
s16 object_bytes23_sum(void) { u8 first; first=object_type_byte2_get(); return (s16)(first + object_type_byte3_get()); }
s8 func_800B3D0C(void) { return D_80149B60; }
s16 object_bytes_sum_global(void) { return object_type_byte2_get() + object_type_byte3_get() + func_800B3D0C(); }
void func_800F68A4(u8 *out) {
 s32 offset;
 s32 i;
 sound_update_channel(0);
 for (i = 0, offset = 0; i < D_801497F0->f12; offset += 12, i++) {
  out[i] = *((u8 *)D_80149800 + offset + 4);
 }
 out[i] = 0;
}
s32 camera_shake_update(u16 id) {
 s16 index;
 sound_update_channel(0);
 if ((u8)D_801497F0->f9 != 0) return D_801497F0->f6;
 if (id == 32 || id >= 256) return D_801497F0->f7;
 if (id == 10) return 0;
 index = D_80149878[id];
 if (index < 0) return D_801497F0->f7;
 return D_80149800[index].pad[3] - D_80149800[index].pad[1] + 1;
}
