/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* RESEARCH ONLY: ABI-blocked diagnostic, not a match submission.
 * Configure a named resource record or a null/sentinel fallback, then reset
 * the object's state, color, and five halfword fields. Historical function
 * name is not evidence that this plays audio; no arcade ancestor is proven.
 *
 * The native caller passes a fifth full word, 1. The accepted callee currently
 * has four formals. This deliberately unprototyped declaration records the
 * caller's observed arguments without inventing a fifth callee formal. It
 * does NOT establish a compatible C contract with the accepted definition.
 * Standalone and real-callee byte equality are diagnostic evidence only.
 *
 * The volatile count follows the accepted callee's address-form byte read.
 * Separate halfword assignments avoid the old chained assignment's reload.
 */
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned char u8;
typedef signed char s8;

typedef struct Resource {
    unsigned char prefix[16];
    u16 width, height;
} Resource;
typedef struct NameEntry {
    char name[36];
} NameEntry;
typedef struct Object {
    char *name;
    void *fallback;
    Resource *resource;
    s16 index;
    unsigned char opaque[4];
    u16 state, width, height;
    u8 color;
    unsigned char gap[3];
    s16 first, second, third, fourth, fifth;
} Object;
extern unsigned char D_80110664[];
extern volatile u8 D_80140BDC;
extern NameEntry *func_800B24EC();

void collision_sound_play(Object *object) {
    if (object->name == 0) {
        object->index = 0;
        object->resource = 0;
        object->width = 0;
        object->height = 0;
        object->fallback = D_80110664;
    } else if (object->name == (char *)-1) {
        object->index = 0;
        object->resource = 0;
        object->width = 0;
        object->height = 0;
        object->fallback = 0;
    } else {
        object->resource = (Resource *)func_800B24EC(object->name, &object->index, 0, (s8)(D_80140BDC - 1), 1);
        object->width = object->resource->width;
        object->height = object->resource->height;
        object->fallback = 0;
    }
    object->state = 0;
    object->color = 255;
    object->first = -1;
    object->second = -1;
    object->third = -1;
    object->fourth = -1;
    object->fifth = -1;
}
