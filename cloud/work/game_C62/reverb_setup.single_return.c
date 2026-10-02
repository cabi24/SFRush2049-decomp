/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef int s32;
typedef struct Fields {
    u8 opaque0[32];s8 normal[21];u8 opaque53[47];
    s8 primary[8];u8 opaque108[8];
    s8 secondary[9];u8 opaque125[11];
    s8 fallback[4];
} Fields;
typedef struct Resource {Fields *fields;} Resource;
typedef struct Object {u8 opaque0[44];Resource *resource;} Object;
typedef struct Owner {Object *object;} Owner;
extern s8 D_8014978C;
extern s32 func_800B4B00(Owner *,u8);
s32 reverb_setup(Owner *owner,u8 selector) {
    Fields *fields;
    s32 mode,result;
    if(selector<21) {result=owner->object->resource->fields->normal[selector];goto finish;}
    mode=D_8014978C;
    if(mode>=0 && mode<6) {result=func_800B4B00(owner,selector);goto finish;}
    if(mode>=19 && mode<25) {result=func_800B4B00(owner,selector);goto finish;}
    if(mode>=6 && mode<14) {
        fields=owner->object->resource->fields;
        switch(selector) {
        case 23:{result=fields->primary[0];goto finish;}
        case 28:{result=fields->primary[1];goto finish;}
        case 29:{result=fields->primary[2];goto finish;}
        case 30:{result=fields->primary[3];goto finish;}
        case 37:{result=fields->primary[4];goto finish;}
        case 38:{result=fields->primary[5];goto finish;}
        case 39:{result=fields->primary[6];goto finish;}
        case 40:{result=fields->primary[7];goto finish;}
        default:{result=0;goto finish;}
        }
    }
    if(mode>=14 && mode<18) {
        fields=owner->object->resource->fields;
        switch(selector) {
        case 23:{result=fields->secondary[0];goto finish;}
        case 28:{result=fields->secondary[1];goto finish;}
        case 29:{result=fields->secondary[2];goto finish;}
        case 30:{result=fields->secondary[3];goto finish;}
        case 32:{result=fields->secondary[4];goto finish;}
        case 33:{result=fields->secondary[5];goto finish;}
        case 34:{result=fields->secondary[6];goto finish;}
        case 35:{result=fields->secondary[7];goto finish;}
        case 36:{result=fields->secondary[8];goto finish;}
        default:{result=0;goto finish;}
        }
    }
    fields=owner->object->resource->fields;
    switch(selector) {
    case 23:{result=fields->fallback[0];goto finish;}
    case 28:{result=fields->fallback[1];goto finish;}
    case 29:{result=fields->fallback[2];goto finish;}
    case 30:{result=fields->fallback[3];goto finish;}
    default:{result=0;goto finish;}
    }
finish:
    return result;
}
