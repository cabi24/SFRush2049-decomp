/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct Resource {u8 *data;} Resource;
typedef struct Object {u8 other0[8];void *lookup;u8 other12[32];Resource *resource;} Object;
typedef struct Handle {Object *object;} Handle;
extern u32 format_string_parse(u8 *,u32);
extern void slot_state_lookup(void *,void *,u32);
void func_800CD8EC(Handle *handle,u8 mode)
{
    u8 *data=handle->object->resource->data,*record;
    u32 *hash;
    if((mode>=0 && mode<6) || (mode>=19 && mode<25)) {
        if(mode>=19) { record=data; record+=mode*96; record-=1108; }
        else { record=data; record+=mode*96; record+=140; }
        *(u32 *)record=format_string_parse(record+4,92);
        slot_state_lookup(handle->object->lookup,(u32 *)record,96);
    } else if(mode>=6 && mode<14) {
        record=data; record+=mode*12;
        hash=(u32 *)(record+1476);
        *hash=format_string_parse(record+1480,8);
        slot_state_lookup(handle->object->lookup,hash,12);
    } else if(mode>=14 && mode<18) {
        record=data; record+=mode*64;
        hash=(u32 *)(record+396);
        *hash=format_string_parse(record+400,60);
        slot_state_lookup(handle->object->lookup,hash,64);
    } else {
        record=data; record+=mode*24;
        hash=(u32 *)(record+1212);
        *hash=format_string_parse(record+1216,20);
        slot_state_lookup(handle->object->lookup,hash,24);
    }
}
