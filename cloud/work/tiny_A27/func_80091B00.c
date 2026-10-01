/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef int s32;typedef unsigned int u32;typedef float f32;
typedef struct Slot {s16 id;u8 pad2;s8 used;u8 pad4[20];} Slot;
extern Slot D_80142DD8[128];
Slot *func_80091B00(void) {
 Slot *slot;
 for(slot=D_80142DD8;slot!=D_80142DD8+128;slot+=4) {
  if(slot[0].used==0) {slot[0].used=1;slot[0].id=-1;return slot+0;}
if(slot[1].used==0) {slot[1].used=1;slot[1].id=-1;return slot+1;}
if(slot[2].used==0) {slot[2].used=1;slot[2].id=-1;return slot+2;}
if(slot[3].used==0) {slot[3].used=1;slot[3].id=-1;return slot+3;}
 }
 return 0;
}
