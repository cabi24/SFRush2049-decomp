/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef int s32;
typedef unsigned int u32;
typedef unsigned char u8;
typedef signed char s8;
typedef float f32;
typedef struct State {
    u8 opaque0[5]; s8 mode;
    u8 opaque6[46]; f32 rate;
    u8 opaque56[20]; u8 *data; u32 position, length;
} State;
typedef struct Link {State *state;} Link;
typedef struct Descriptor {u8 opaque0[40]; Link *link;} Descriptor;
typedef struct Object {Descriptor *descriptor;} Object;
typedef struct Car2056 {
    u8 opaque0[1824]; f32 analog;
    u8 opaque1828[4]; f32 low,high;
    s8 gear,flag1,flag2;
    u8 opaque1843[213];
} Car2056;
typedef struct Player952 {
    u8 opaque0[239]; s8 flag239; u8 opaque240[712];
} Player952;
extern f32 D_8002AFB8,D_80124490,D_80124494;
typedef struct Flag {s8 value;} Flag;
extern Flag D_80114738;
extern s32 D_80143FF4;
extern Object *D_80152698[];
extern Car2056 D_8014A250[];
extern Player952 player_array[];
s32 func_800E5D64(s32 index,f32 *output) {
    State *state;
    Object *object;
    *output=D_8002AFB8;
    if(D_80114738.value) return 0;
    object=D_80152698[index];
    if(!object) return 0;
    state=object->descriptor->link->state;
    if(!state->mode) return 0;
    if(state->mode>0) {
        Car2056 *car;
        *output=state->rate;
        if(state->rate!=D_8002AFB8 && state->rate==D_80124490) {
            s32 absolute=D_80143FF4<0?-D_80143FF4:D_80143FF4;
            if(absolute%6==2) return -1;
        }
        if(state->position==state->length) {
            car=&D_8014A250[index];
            car->flag2=0; car->flag1=0;
            car->analog=0.0f; car->high=0.0f; car->low=0.0f;
        } else {
            car=&D_8014A250[index];
            car->gear=(state->data[state->position]&7)-1;
            car->flag2=(state->data[state->position]&8)>>3;
            car->flag1=(state->data[state->position]&16)>>4;
            car->analog=(f32)((s8 *)state->data)[state->position+state->length]/127.0f;
            car->high=(f32)(state->data[state->position+state->length+state->length]>>4)/15.0f;
            car->low=(f32)(state->data[state->position+state->length+state->length]&15)/15.0f;
            state->position++;
        }
        if(state->rate!=D_8002AFB8 && state->rate==D_80124494) {
            s32 absolute=D_80143FF4<0?-D_80143FF4:D_80143FF4;
            if(absolute%5==2) return 1;
        }
        return 0;
    } else {
        Car2056 *car;
        f32 rounded;
        u8 low,high;
        if(player_array[index].flag239) return 0;
        if(state->position>=state->length) return 0;
        car=&D_8014A250[index];
        state->data[state->position]=(car->flag1<<4)|(car->gear+1)|(car->flag2<<3);
        if(car->analog*127.0f<0.0f) rounded=car->analog*127.0f-0.5f;
        else rounded=car->analog*127.0f+0.5f;
        state->data[state->position+state->length]=(s32)rounded;
        low=(u32)(car->low*15.0f+0.5f);
        high=(u32)(car->high*15.0f+0.5f);
        state->data[state->position+state->length+state->length]=low|(high<<4);
        state->position++;
        return 0;
    }
}
