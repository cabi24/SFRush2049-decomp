typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u32;
#define M2C_FIELD(expr, type, offset) (*(type)((u8 *)(expr) + (offset)))
extern u32 format_string_parse(u8 *, u32);
extern void slot_state_lookup(void *, void *, u32);
typedef struct Setting { u32 checksum; s8 fields[12]; } Setting;
typedef struct SaveData { u8 prior[1856]; Setting settings[13]; } SaveData;
void func_800C7578(void *obj,u8 index,u8 field,s8 value) {
 SaveData *base=M2C_FIELD(M2C_FIELD(M2C_FIELD(obj,void **,0),void **,44),SaveData **,0);
 if(base->settings[index].fields[field] != value) {
  base->settings[index].fields[field]=value;
  base->settings[index].checksum=format_string_parse((u8 *)base->settings[index].fields,12);
  slot_state_lookup(M2C_FIELD(M2C_FIELD(obj,void **,0),void **,8),&base->settings[index],16);
 }
}
