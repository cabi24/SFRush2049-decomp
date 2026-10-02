/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct Resource {u8 *data;} Resource;
typedef struct Object {u8 other0[8];void *lookup;u8 other12[32];Resource *resource;} Object;
typedef struct Handle {Object *object;} Handle;
extern u32 format_string_parse(u8 *,u32);
extern void slot_state_lookup(void *,u32 *,int);
void func_800CD798(Handle *handle,u8 mode)
{
    u8 *data=handle->object->resource->data;
    u8 *record;
    u32 *hash;
    if((mode>=0 && mode<6) || (mode>=19 && mode<25)) {
        if(mode>=19) mode-=19;
        record=data+mode*96;
        *(u32 *)(record+140)=format_string_parse(record+144,92);
        hash=(u32 *)(record+140);
        slot_state_lookup(handle->object->lookup,hash,1);
        slot_state_lookup(handle->object->lookup,(u32 *)((u8 *)hash+92),2);
        hash=(u32 *)(record+716);
        slot_state_lookup(handle->object->lookup,hash,1);
        slot_state_lookup(handle->object->lookup,(u32 *)((u8 *)hash+92),2);
    } else if(mode>=14 && mode<18) {
        record=data+mode*64;
        *(u32 *)(record+396)=format_string_parse(record+400,60);
        hash=(u32 *)(record+396);
        slot_state_lookup(handle->object->lookup,hash,1);
        slot_state_lookup(handle->object->lookup,(u32 *)((u8 *)hash+60),2);
    }
}
