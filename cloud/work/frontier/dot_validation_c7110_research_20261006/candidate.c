typedef unsigned char u8; typedef signed char s8; typedef signed int s32; typedef unsigned int u32; typedef float f32;
#define NULL ((void*)0)
#define M2C_FIELD(expr,type,offset) (*(type)((u8 *)(expr)+(offset)))
extern void AdjustSpeed(void*);
extern u32 format_string_parse(u8*,u32);
s32 draw_ui_element(void *obj,s32 cancel,s32 check_payload) {
 void *state=M2C_FIELD(obj,void **,0);
 u8 *data=M2C_FIELD(M2C_FIELD(state,void **,40),u8 **,0);
 s32 status=0;
 void *child;
 if(data[4] != 17) { status=1; if(cancel) {
  child=M2C_FIELD(state,void **,4); if(child) AdjustSpeed(child);
  return 0;
 } }
 if(format_string_parse(data+4,64) != M2C_FIELD(data,u32 *,0)) {
  status=1;
  if(cancel) {child=M2C_FIELD(M2C_FIELD(obj,void **,0),void **,4);if(child)AdjustSpeed(child);return 0;}
 }
 if(!check_payload) return status;
 if(format_string_parse(data+88,M2C_FIELD(data,u32 *,60)) != M2C_FIELD(data,u32 *,68)) {
  status=1;
  if(cancel) {child=M2C_FIELD(M2C_FIELD(obj,void **,0),void **,4);if(child)AdjustSpeed(child);return 0;}
 }
 return status;
}
