/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct Resource {u8 *data;} Resource;
typedef struct Object {u8 other0[8];void *lookup;u8 other12[32];Resource *resource;} Object;
typedef struct Handle {Object *object;} Handle;
extern u32 format_string_parse(u8 *,u32);
extern void slot_state_lookup(void *,u32 *,int);
u32 format_string_parse(u8 *data,u32 size)
{
    u8 *cursor=data;
    u32 result=0,value;
    while(size!=0) {
        value=*cursor;
        switch(value&7) {
        case 0:result-=value;cursor++;break;
        case 1:result|=value;cursor++;break;
        case 2:result&=value;cursor++;break;
        case 3:result^=value;cursor++;break;
        case 4:result*=value;cursor++;break;
        case 5:result/=value;cursor++;break;
        default:result+=value;cursor++;break;
        }
        size--;
    }
    return result;
}

void menu_dialog_close(Handle *handle,u8 kind)
{
    u8 *record=handle->object->resource->data+kind*28+1668;
    *(u32 *)record=format_string_parse(record+4,24);
    slot_state_lookup(handle->object->lookup,(u32 *)record,28);
}

void object_data_allocate(Handle *handle)
{
    u8 *record=handle->object->resource->data+1780;
    *(u32 *)record=format_string_parse(record+4,72);
    slot_state_lookup(handle->object->lookup,(u32 *)record,76);
}
