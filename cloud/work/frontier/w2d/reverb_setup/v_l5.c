/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef int s32;
typedef struct Fields {
    u8 opaque0[32];
    s8 normal[21];
    u8 opaque53[27];
    s8 modeA[10];
    u8 opaque90[10];
    s8 primary[8];
    u8 opaque108[8];
    s8 secondary[9];
    u8 opaque125[11];
    s8 fallback[4];
} Fields;
typedef struct Resource {Fields *fields;} Resource;
typedef struct Object {u8 opaque0[44];Resource *resource;} Object;
typedef struct Owner {Object *object;} Owner;
extern s8 D_8014978C;

s32 func_800B4B00(Owner *owner, u8 selector, s32 mode) {
    Fields *fields = owner->object->resource->fields;

    switch (selector) {
    case 21: return fields->modeA[0];
    case 22: return fields->modeA[1];
    case 23: return fields->modeA[2];
    case 24: return fields->modeA[3];
    case 26: return fields->modeA[4];
    case 27: return fields->modeA[5];
    case 28: return fields->modeA[6];
    case 29: return fields->modeA[7];
    case 30: return fields->modeA[8];
    case 31: return fields->modeA[9];
    default: return 0;
    }
}

s32 reverb_setup(Owner *owner, u8 selector) {
    Fields *fields;
    s32 mode;

    if (selector < 21) {
        return owner->object->resource->fields->normal[selector];
    }
    mode = D_8014978C;
    if (mode >= 0 && mode < 6) {
        return func_800B4B00(owner, selector, mode);
    }
    if (mode >= 19 && mode < 25) {
        return func_800B4B00(owner, selector, mode);
    }
    if (mode >= 6 && mode < 14) {
        fields = owner->object->resource->fields;
        switch (selector) {
        case 23: return fields->primary[0];
        case 28: return fields->primary[1];
        case 29: return fields->primary[2];
        case 30: return fields->primary[3];
        case 37: return fields->primary[4];
        case 38: return fields->primary[5];
        case 39: return fields->primary[6];
        case 40: return fields->primary[7];
        default: goto zero;
        }
    }
    if (mode >= 14 && mode < 18) {
        fields = owner->object->resource->fields;
        switch (selector) {
        case 23: return fields->secondary[0];
        case 28: return fields->secondary[1];
        case 29: return fields->secondary[2];
        case 30: return fields->secondary[3];
        case 32: return fields->secondary[4];
        case 33: return fields->secondary[5];
        case 34: return fields->secondary[6];
        case 35: return fields->secondary[7];
        case 36: return fields->secondary[8];
        default: goto zero;
        }
    }
    fields = owner->object->resource->fields;
    switch (selector) {
    case 23: return fields->fallback[0];
    case 28: return fields->fallback[1];
    case 29: return fields->fallback[2];
    case 30: break;
    default: return 0;
    }
    return fields->fallback[3];
zero:
    return 0;
}
