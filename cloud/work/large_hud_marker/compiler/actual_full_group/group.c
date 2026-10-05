/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Sprite {
    u32 word0, word4, word8;
    u16 half12;
    s16 x, y;
    u16 half18;
    s16 width, height;
    u8 alpha, byte25;
    s8 hidden;
    u8 byte27;
    s16 left, right, top, bottom;
    u32 word36;
    s32 state;
    u32 selector;
    u32 word48;
    u16 half52;
} Sprite;

typedef struct MarkerRecord {
    u8 prefix[48];
    f32 position[3];
    u8 alpha;
    u8 remaining[3];
} MarkerRecord;

typedef struct PlayerInput {
    u8 prefix;
    u8 state;
    u8 remaining[74];
} PlayerInput;

extern s16 D_8014A100[];
extern s16 D_8014A10A;
extern s16 D_80151AD0;
extern s8 D_80149DA8[][10];
extern s8 D_8015418C[];
extern PlayerInput input_rec0[];
extern s32 D_80154618[];
extern MarkerRecord D_80111998[][17];
extern u8 D_80150B70[][152];
extern f32 D_80112A9C;
extern f32 D_80112AB0;
extern u8 D_80113EE0[];
extern u8 D_80120234[];
extern s32 D_801140E8;
extern s32 D_8011407C[];
extern s8 D_8013C068[][10];
extern void Input_ApplyPadConfig(Sprite *);
extern s8 input_new_data_wrapper(s8 *,s32);
extern void func_800EF5B0(Sprite *, void *, s32);
extern void brake_light_update(s32, f32 *, void *, f32 *, s16 *);
extern void stat_race_update(Sprite *, s32, s32, s32);

typedef struct Backend {u32 word0,word4;u16 half8;s16 x,y;u8 opaque14[2];s16 width,height;u8 opaque20[12];} Backend;
extern Backend D_80140BF0[];
extern void input_analog_write(u32,s32,s32,s32,s32);
extern void input_status_update(u32,s32,s32,u16);
extern void input_button_adjust(u32,s32);
extern void input_connected_check(u32,s32);
void Input_ApplyPadConfig(Sprite *sprite) {
    Backend *backend;
    s16 height,width,y,x;
    u32 word4,word8;
    u16 half12;
    backend=&D_80140BF0[sprite->half52];
    height=sprite->height;
    width=sprite->width;
    y=sprite->y;
    x=sprite->x;
    word4=sprite->word4;
    word8=sprite->word8;
    half12=sprite->half12;
    backend->height=height;
    backend->width=width;
    backend->y=y;
    backend->x=x;
    backend->word0=word4;
    backend->word4=word8;
    backend->half8=half12;
    input_analog_write(sprite->half52,sprite->left,sprite->right,sprite->top,sprite->bottom);
    input_status_update(sprite->half52,sprite->hidden,sprite->alpha,sprite->half18);
    input_button_adjust(sprite->half52,sprite->byte25);
    input_connected_check(sprite->half52,(s32)sprite->word0==-1);
}

extern u8 D_80140BDC;
extern void *func_800B24EC(void *,s16 *,s8,s8,s32);
extern void collision_sound_play(void *);
void func_800EF5B0(Sprite *sprite,void *name,s32 direct) {
    sprite->word0=(u32)name;
    if(direct) sprite->word8=(u32)func_800B24EC(name,(s16 *)&sprite->half12,0,(s8)(D_80140BDC-1),1);
    else collision_sound_play(sprite);
    Input_ApplyPadConfig(sprite);
}
typedef struct DataCount {u8 other[16];u16 count;} DataCount;
void stat_race_update(Sprite *sprite,s32 index,s32 step,s32 span) {
    s32 rows,col,page;
    DataCount *buffer;
    if(sprite==0)return;
    buffer=(DataCount *)sprite->word8;
    if(buffer==0 || buffer->count==0) {
        func_800EF5B0(sprite,(void *)sprite->word0,1);
        buffer=(DataCount *)sprite->word8;
        if(buffer==0)return;
    }
    rows=buffer->count/step;
    if(rows==0)rows=1;
    col=index%rows;
    page=index/rows;
    rows-=col;
    sprite->left=page*span;
    sprite->right=sprite->left+span-1;
    if(sprite->byte25) {
        sprite->top=((DataCount *)sprite->word8)->count%step+(rows-1)*step;
        sprite->bottom=sprite->top+step-1;
    } else {
        sprite->top=col*step;
        sprite->bottom=sprite->top+step-1;
    }
    Input_ApplyPadConfig(sprite);
}
typedef struct Transform48 {f32 basis[9],position[3];} Transform48;
typedef struct Descriptor72 {u8 opaque[28];f32 horizontal,vertical,width,height,offset_x,offset_y;u8 tail[20];} Descriptor72;
extern Descriptor72 D_8017A510[];
extern f32 D_80123BD4,D_80123BD8;
extern void func_800A61B0(f32 *,f32 *,Transform48 *);
void brake_light_update(s32 index,f32 *position,void *matrix,f32 *view_output,s16 *screen_output) {
    Transform48 *transform=matrix;
    f32 difference[3],view[3],screen[2],inverse;
    Descriptor72 *descriptor;
    difference[0]=position[0]-transform->position[0];
    difference[1]=position[1]-transform->position[1];
    difference[2]=position[2]-transform->position[2];
    func_800A61B0(difference,view,transform);
    if(view[2]<2.5f)view[2]=2.5f;
    inverse=1.0f/view[2];
    descriptor=&D_8017A510[index];
    screen[0]=view[0]*inverse*descriptor->horizontal*descriptor->width+descriptor->offset_x;
    screen[1]=descriptor->offset_y-view[1]*inverse*descriptor->vertical*descriptor->height;
    if(screen[0]<-32768.0f)screen_output[0]=-32768;
    else if(screen[0]>D_80123BD4)screen_output[0]=32767;
    else screen_output[0]=(s32)screen[0];
    if(screen[1]<-32768.0f)screen_output[1]=-32768;
    else if(screen[1]>D_80123BD8)screen_output[1]=32767;
    else screen_output[1]=(s32)screen[1];
    if(view_output!=0) {
        view_output[0]=view[0];
        view_output[1]=view[1];
        view_output[2]=view[2];
    }
}

s8 input_new_data_wrapper(s8 *arg0, s32 arg1) {
    if (arg1 != arg0[0x1A]) {
        arg0[0x1A] = arg1;
        Input_ApplyPadConfig((Sprite *)arg0);
    }
    return arg0[0x1A];
}

s32 game_results_input(Sprite *sprite)
{
    s32 marker;
    s32 extra;
    s32 initial;
    s32 player;
    s32 previous;
    s32 slot;
    s16 count;
    s16 offset;
    s16 first;
    s32 visible;
    s32 hidden;
    s32 x_offset;
    f32 scale;
    s16 screen[2];
    PlayerInput *input;
    MarkerRecord *record;

    extra = (sprite->selector >> 4) & 15;
    player = (sprite->selector >> 12) & 15;
    initial = (sprite->selector >> 8) & 15;
    marker = sprite->selector & 15;
    previous = ((sprite->selector >> 16) & 255) - 1;
    first = D_8014A100[player];
    slot = first + marker - 1;
    scale = D_80112A9C;
    if (D_80151AD0 >= 2) {
        scale *= 0.5f;
    }
    for (count = 0, offset = 0; count < marker; offset++) {
        visible = 1;
        if (first + offset == 8) {
            visible = 0;
        }
        if (!visible) {
            slot++;
        } else {
            count++;
        }
    }
    input = &input_rec0[player];
    hidden = input->state == 5 || (extra && !D_80149DA8[player][slot]) || slot >= 10 || !D_8015418C[player] || D_8014A10A < marker;
    if (input_new_data_wrapper((s8 *)sprite,hidden)) {
        return 1;
    }
    extra = extra != 0;
    if (initial) {
        if (D_80151AD0 >= 2) {
            func_800EF5B0(sprite, D_80120234, 0);
        }
        if (D_80154618[player]) {
            record = &D_80111998[player][slot];
            brake_light_update(player, record->position, D_80150B70[player], 0, screen);
            sprite->x = screen[0] + 55.0f * scale * 300.0f / D_80112AB0;
            sprite->y = screen[1] - sprite->height / 2;
            sprite->alpha = record->alpha;
        } else {
            sprite->x = *(s32 *)(D_80113EE0 + D_80151AD0 * 96 + player * 24 - 80) + 20;
            sprite->y = *(s32 *)(D_80113EE0 + D_80151AD0 * 96 + player * 24 - 92) + marker * 16 + 8;
        }
        if (extra) {
            x_offset = D_80151AD0 == 1 ? 20 : 10;
            sprite->x += x_offset;
        }
        Input_ApplyPadConfig(sprite);
        sprite->selector &= 0x00FFF0FF;
        if (extra) {
            sprite->selector = (sprite->selector & 0xFFFF) | 0x001C0000;
            stat_race_update(sprite, D_801140E8, sprite->height, sprite->height);
        }
    }
    if (D_80154618[player]) {
        record = &D_80111998[player][slot];
        brake_light_update(player, record->position, D_80150B70[player], 0, screen);
        sprite->x = screen[0] + 75.0f * scale * 300.0f / D_80112AB0;
        sprite->y = screen[1] - sprite->height / 2;
        sprite->half18 = 0x7F00;
        if (extra) {
            x_offset = D_80151AD0 == 1 ? 20 : 10;
            sprite->x += x_offset;
        }
        sprite->alpha = record->alpha < 255 ? record->alpha : 254;
        hidden = sprite->alpha == 0;
        if (input_new_data_wrapper((s8 *)sprite,hidden)) {
            return 1;
        }
        Input_ApplyPadConfig(sprite);
    } else {
        sprite->alpha = 254;
        Input_ApplyPadConfig(sprite);
    }
    if (extra) {
        return 1;
    }
    marker = D_8013C068[input->state][slot];
    if (previous != marker) {
        previous = marker;
        sprite->selector = (sprite->selector & 0xFFFF) | (((marker + 1) << 16) & 0x00FF0000);
        if (previous == 25 && (slot == 0 || slot == 1)) {
            previous++;
        }
        stat_race_update(sprite, D_8011407C[previous], sprite->height, sprite->height);
    }
    return 1;
}
