/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16; typedef unsigned int u32; typedef float f32;
typedef struct Gfx { u32 w0,w1; } Gfx;
typedef union AnimatedData { u16 *palette; Gfx *commands; } AnimatedData;
typedef struct PaletteAnimation20 { void *owner; s16 first,last; s8 step; u8 action; s16 ticks; f32 delay; AnimatedData data; } PaletteAnimation20;
typedef struct ObjectView { u8 before24[24]; Gfx *commands; u32 flags; } ObjectView;
typedef struct AnimationFrame12 { u32 tag; ObjectView *object; Gfx *commands; } AnimationFrame12;
typedef struct ObjectAnimation20 { s16 count,object_index,direction,index; f32 delay,interval; AnimationFrame12 *frames; } ObjectAnimation20;
extern u32 D_801174B4;
extern s8 D_80156994,D_8014978C;
extern PaletteAnimation20 *D_8011A840[];
extern ObjectAnimation20 *D_8011A31C[];
extern AnimationFrame12 D_8011905C[];
extern f32 D_8002EB94,D_80123E28;
extern int D_8002EB98;
extern void camera_smooth_follow(Gfx *,int,int,int,int);
extern void func_800BD080(Gfx *,Gfx *);
void race_setup_1(void)
{
    PaletteAnimation20 *palette_animation;
    ObjectAnimation20 *object_animation;
    u16 *palette;
    u16 saved;
    int i,offset;
    ObjectView *object;
    if((D_801174B4&8) && !D_80156994) return;
    palette_animation=D_8011A840[D_8014978C];
    if(palette_animation && palette_animation->owner) {
        do {
            palette_animation->delay-=D_8002EB94;
            if(!(palette_animation->delay>0.0f)) {
                palette_animation->delay+=(f32)palette_animation->ticks*D_80123E28;
                palette=palette_animation->data.palette;
                switch(palette_animation->action) {
                case 0:
                    saved=palette[palette_animation->last];
                    for(i=palette_animation->last-1;i>=palette_animation->first;i--) palette[i+1]=palette[i];
                    palette[palette_animation->first]=saved;
                    break;
                case 1:
                    saved=palette[palette_animation->first];
                    for(i=palette_animation->first+1;i<=palette_animation->last;i++) palette[i-1]=palette[i];
                    palette[palette_animation->last]=saved;
                    break;
                case 2:
                    offset=palette_animation->last-palette_animation->first+palette_animation->step+1;
                    for(i=palette_animation->first;i<=palette_animation->last;i++) {
                        saved=palette[i]; palette[i]=palette[i+offset]; palette[i+offset]=saved;
                    }
                    break;
                case 3:
                    offset=palette_animation->last-palette_animation->first+palette_animation->step+1;
                    for(i=palette_animation->first;i<=palette_animation->last;i++) {
                        saved=palette[i]; palette[i]=palette[i+offset]; palette[i+offset]=palette[i+offset+offset]; palette[i+offset+offset]=saved;
                    }
                    break;
                case 6:
                    palette[palette_animation->first+2]=1985; palette[palette_animation->first+1]=10561; palette[palette_animation->first]=10241;
                    palette_animation->action=5; palette_animation->delay=10.0f;
                    break;
                case 5:
                    palette[palette_animation->first+2]=321; palette[palette_animation->first+1]=0xffc1; palette[palette_animation->first]=10241;
                    palette_animation->action=4; palette_animation->delay=0.5f;
                    break;
                case 4:
                    palette[palette_animation->first+2]=321; palette[palette_animation->first+1]=10561; palette[palette_animation->first]=0xf801;
                    palette_animation->action=6; palette_animation->delay=10.5f;
                    break;
                case 7:
                    if(palette_animation->step<10) palette[palette_animation->step]=0xf001;
                    else if(palette_animation->step>=21) {
                        for(i=0;i<10;i++) palette[i]=12421;
                        palette_animation->step=-1;
                    }
                    palette_animation->step++;
                    break;
                case 8:
                    if(palette_animation->step==0) palette[palette_animation->step+7]=0xce73;
                    else if(palette_animation->step<8) {
                        palette[palette_animation->step-1]=0xf141;
                        palette[palette_animation->step+7]=0xce73;
                    } else if(palette_animation->step>=23) {
                        for(i=0;i<7;i++) { palette[i]=12289; palette[i+7]=4229; }
                        palette[14]=4229; palette_animation->step=-1;
                    }
                    palette_animation->step++;
                    break;
                case 9:
                    palette_animation->first+=palette_animation->step*D_8002EB98;
                    if(palette_animation->first<0) palette_animation->first+=palette_animation->last;
                    else if(palette_animation->first>=palette_animation->last) palette_animation->first-=palette_animation->last;
                    camera_smooth_follow((Gfx *)palette,palette_animation->first>>2,-1,-1,-1);
                    break;
                case 10:
                    palette_animation->first+=palette_animation->step*D_8002EB98;
                    if(palette_animation->first<0) palette_animation->first+=palette_animation->last;
                    else if(palette_animation->first>=palette_animation->last) palette_animation->first-=palette_animation->last;
                    camera_smooth_follow((Gfx *)palette,-1,palette_animation->first>>2,-1,-1);
                    break;
                }
            }
            palette_animation++;
        } while(palette_animation->owner);
    }
    object_animation=D_8011A31C[D_8014978C];
    if(object_animation && object_animation->count) {
        do {
            if(!(!D_80156994 && D_8014978C>=0 && D_8014978C<6 && object_animation->frames==D_8011905C)) {
                object_animation->delay-=D_8002EB94;
                if(object_animation->delay<=0.0f) {
                    object_animation->delay=object_animation->interval;
                    if(object_animation->direction) {
                        object_animation->index++;
                        if(object_animation->index>=object_animation->count) object_animation->index=0;
                    } else {
                        object_animation->index--;
                        if(object_animation->index<0) object_animation->index=object_animation->count-1;
                    }
                    object=object_animation->frames[object_animation->object_index].object;
                    if(object->flags&0x08000000) object->commands=object_animation->frames[object_animation->index].commands;
                    else func_800BD080(object->commands,object_animation->frames[object_animation->index].commands);
                }
            }
            object_animation++;
        } while(object_animation->count);
    }
}
