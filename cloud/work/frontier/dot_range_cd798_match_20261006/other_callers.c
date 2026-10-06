typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u32;
#define M2C_FIELD(expr, type, offset) (*(type)((u8 *)(expr) + (offset)))
extern u32 format_string_parse(u8 *, u32);
extern void slot_state_lookup(void *, void *, u32);
void object_data_allocate(void *obj) {
 u8 *data=M2C_FIELD(M2C_FIELD(M2C_FIELD(obj,void **,0),void **,44),u8 **,0)+1780;
 M2C_FIELD(data,u32 *,0)=format_string_parse(data+4,72);
 slot_state_lookup(M2C_FIELD(M2C_FIELD(obj,void **,0),void **,8),data,76);
}
void menu_dialog_close(void *obj,u8 index) {
 u8 *data=M2C_FIELD(M2C_FIELD(M2C_FIELD(obj,void **,0),void **,44),u8 **,0)+index*28+1668;
 M2C_FIELD(data,u32 *,0)=format_string_parse(data+4,24);
 slot_state_lookup(M2C_FIELD(M2C_FIELD(obj,void **,0),void **,8),data,28);
}
