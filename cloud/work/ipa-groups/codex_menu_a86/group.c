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

void menu_option_toggle(Handle *handle,u8 value)
{
    u8 *data=handle->object->resource->data;
    switch(data[71]) {
    case 0:case 1:case 2:case 3:
        if(value==data[78])return;
        data[78]=value;
        *(u32 *)(data+72)=format_string_parse(data+76,16);
        slot_state_lookup(handle->object->lookup,(u32 *)(data+72),20);
        break;
    case 6:
        if(value==data[98])return;
        data[98]=value;
        *(u32 *)(data+92)=format_string_parse(data+96,12);
        slot_state_lookup(handle->object->lookup,(u32 *)(data+92),16);
        break;
    case 4:
        if(value==data[114])return;
        data[114]=value;
        *(u32 *)(data+108)=format_string_parse(data+112,16);
        slot_state_lookup(handle->object->lookup,(u32 *)(data+108),20);
        break;
    case 5:
        if(value==data[134])return;
        data[134]=value;
        *(u32 *)(data+128)=format_string_parse(data+132,8);
        slot_state_lookup(handle->object->lookup,(u32 *)(data+128),12);
        break;
    }
}
