/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct Resource {u8 *data;} Resource;
typedef struct Object {u8 other0[8];void *lookup;u8 other12[32];Resource *resource;} Object;
typedef struct Handle {Object *object;} Handle;
extern u32 format_string_parse(u8 *,u32);
extern void slot_state_lookup(void *,void *,u32);
void func_800CD798(Handle *handle,u8 mode)
{
    u8 *data=handle->object->resource->data;
    u8 *record;
    u32 *hash;
    if((mode>=0 && mode<6) || (mode>=19 && mode<25)) {
        if(mode>=19) mode-=19;
        record=data; record+=mode*96;
        hash=(u32 *)(record+140);
        *hash=format_string_parse(record+144,92);
        slot_state_lookup(handle->object->lookup,hash,1);
        slot_state_lookup(handle->object->lookup,(u32 *)((u8 *)hash+92),2);
        hash=(u32 *)(record+716);
        slot_state_lookup(handle->object->lookup,hash,1);
        slot_state_lookup(handle->object->lookup,(u32 *)((u8 *)hash+92),2);
    } else if(mode>=14 && mode<18) {
        record=data; record+=mode*64;
        hash=(u32 *)(record+396);
        *hash=format_string_parse(record+400,60);
        slot_state_lookup(handle->object->lookup,hash,1);
        slot_state_lookup(handle->object->lookup,(u32 *)((u8 *)hash+60),2);
    }
}
