/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
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

extern s8 D_8014978C;
void voice_stop_2(Handle *handle,u8 selector,s8 value)
{
    u8 *data=handle->object->resource->data;
    s8 *field;
    switch(selector) {
    case 21:field=(s8 *)(data+80);break;
    case 22:field=(s8 *)(data+81);break;
    case 23:field=(s8 *)(data+82);break;
    case 24:field=(s8 *)(data+83);break;
    case 26:field=(s8 *)(data+84);break;
    case 27:field=(s8 *)(data+85);break;
    case 28:field=(s8 *)(data+86);break;
    case 29:field=(s8 *)(data+87);break;
    case 30:field=(s8 *)(data+88);break;
    case 31:field=(s8 *)(data+89);break;
    default:return;
    }
    if(value==*field)return;
    *field=value;
    *(u32 *)(data+72)=format_string_parse(data+76,16);
    slot_state_lookup(handle->object->lookup,(u32 *)(data+72),20);
}

void audio_bus_mix(Handle *handle,u8 selector,s8 value)
{
    Resource *resource=handle->object->resource;
    u8 *data;
    s8 *field;
    int mode;
    if(resource==0)return;
    data=resource->data;
    if(selector<21) {
        field=(s8 *)(data+32+selector);
        if(value==*field)return;
        *field=value;
        *(u32 *)(data+28)=format_string_parse(data+32,21);
        slot_state_lookup(handle->object->lookup,(u32 *)(data+28),25);
        return;
    }
    mode=D_8014978C;
    if(mode>=0 && mode<6) {voice_stop_2(handle,selector,value);return;}
    else if(mode>=19 && mode<25) {voice_stop_2(handle,selector,value);return;}
    else if(mode>=6 && mode<14) {
        switch(selector) {
        case 23:field=(s8 *)(data+100);break;
        case 28:field=(s8 *)(data+101);break;
        case 29:field=(s8 *)(data+102);break;
        case 30:field=(s8 *)(data+103);break;
        case 37:field=(s8 *)(data+104);break;
        case 38:field=(s8 *)(data+105);break;
        case 39:field=(s8 *)(data+106);break;
        case 40:field=(s8 *)(data+107);break;
        default:return;
        }
        if(value==*field)return;
        *field=value;
        *(u32 *)(data+92)=format_string_parse(data+96,12);
        slot_state_lookup(handle->object->lookup,(u32 *)(data+92),16);
    } else if(mode>=14 && mode<18) {
        switch(selector) {
        case 23:field=(s8 *)(data+116);break;
        case 28:field=(s8 *)(data+117);break;
        case 29:field=(s8 *)(data+118);break;
        case 30:field=(s8 *)(data+119);break;
        case 32:field=(s8 *)(data+120);break;
        case 33:field=(s8 *)(data+121);break;
        case 34:field=(s8 *)(data+122);break;
        case 35:field=(s8 *)(data+123);break;
        case 36:field=(s8 *)(data+124);break;
        default:return;
        }
        if(value==*field)return;
        *field=value;
        *(u32 *)(data+108)=format_string_parse(data+112,16);
        slot_state_lookup(handle->object->lookup,(u32 *)(data+108),20);
    } else {
        switch(selector) {
        case 23:field=(s8 *)(data+136);break;
        case 28:field=(s8 *)(data+137);break;
        case 29:field=(s8 *)(data+138);break;
        case 30:field=(s8 *)(data+139);break;
        default:return;
        }
        if(value==*field)return;
        *field=value;
        *(u32 *)(data+128)=format_string_parse(data+132,8);
        slot_state_lookup(handle->object->lookup,(u32 *)(data+128),12);
    }
}
