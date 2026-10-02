/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef int s32;typedef unsigned int u32;
void *memcpy(void *,const void *,u32);
typedef struct Info72 {u8 p0[56];u32 checksum;u8 name[11];} Info72;
typedef struct Object48 {u8 p0[8];void *control;u8 p12[21];u8 name[11];Info72 **info;} Object48;
s32 func_800A1910(void *,u8 *,s32);
u32 format_string_parse(u8 *,u32);
void slot_state_lookup(void *,u32 *,u32);
void func_800CCE5C(Object48 **object,u8 *name) {
 Info72 **link=(*object)->info;
 if(link) {
  Info72 *info=*link;
  u8 *stored=info->name;
  if(func_800A1910(stored,name,11)!=0) {
   memcpy((*object)->name,name,11);
   memcpy(stored,name,11);
   info->checksum=format_string_parse(stored,11);
   slot_state_lookup((*object)->control,&info->checksum,15);
  }
 }
}
