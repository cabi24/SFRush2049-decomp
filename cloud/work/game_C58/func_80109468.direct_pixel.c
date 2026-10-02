/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef struct Texture {u8 opaque0[24];u8 *image;} Texture;
typedef struct Sprite {
    u32 word0,word4;
    Texture *texture;
    u16 half12;
    s16 x,y;
    u16 half18;
    s16 width,height;
    u8 byte24,byte25;
    s8 hidden;
    u8 byte27;
    s16 left,right,top,bottom;
    u32 word36;
    s32 state;
    u32 selector;
    u32 word48;
    u16 half52;
} Sprite;
typedef struct Point {s16 x,y,z;} Point;
typedef struct Position {s32 x,y;} Position;
typedef struct Section {u8 active,opaque1[9];u16 count;Point *points;} Section;
typedef struct Route {u16 count,half2;Point *points;u8 section_count,opaque9[3];Section *sections;} Route;
extern s16 D_80151AD0;
extern s8 D_80156BDC,D_80140A04;
extern s32 D_801161C4;
extern Position D_801160A8[];
extern Point D_801407B4,D_801407D4;
extern Route D_801407F0;
extern Point *D_801409E8;
extern char D_80120E40[];
extern void Input_ApplyPadConfig(Sprite *);
extern void func_800EF5B0(Sprite *,char *,s32);
s32 func_80109468(Sprite *sprite) {
    s32 hidden;
    s32 dx,dz,scaled_x,scaled_z,offset_x,offset_z;
    s32 row,column,index,section_index;
    s32 px,py,x,y,left,right,top,bottom,plot_x;
    u8 *pixel;
    Point *point,*section_point;
    Section *section;
    hidden=D_80151AD0>=5||!D_80156BDC;
    if(hidden!=sprite->hidden) {
        sprite->hidden=hidden;
        Input_ApplyPadConfig(sprite);
    }
    if(sprite->hidden) return 1;
    if(!(sprite->selector&0x10)) return 1;
    if(D_80151AD0==4) func_800EF5B0(sprite,D_80120E40,0);
    D_801161C4=sprite->width;
    if(sprite->selector&0x10) {
        sprite->selector&=0xf;
        sprite->x=D_801160A8[D_80151AD0-1].x-D_801161C4/2;
        sprite->y=D_801160A8[D_80151AD0-1].y-D_801161C4/2;
        Input_ApplyPadConfig(sprite);
    }
    dx=D_801407B4.x-D_801407D4.x;
    dz=D_801407B4.z-D_801407D4.z;
    if(dz<dx) {
        scaled_x=D_801161C4-8;
        scaled_z=dz*scaled_x/dx;
    } else {
        scaled_z=D_801161C4-8;
        scaled_x=dx*scaled_z/dz;
    }
    offset_x=(D_801161C4-scaled_x-8)/2+4;
    offset_z=(D_801161C4-scaled_z-8)/2+4;
    for(row=0;row<D_801161C4;row++) {
        for(column=0;column<D_801161C4/2;column++) {
            sprite->texture->image[(row*D_801161C4)/2+column]=0;
        }
    }
    for(index=0;index<D_801407F0.count;index++) {
        if(index>=D_801407F0.count) {
            section_point=&D_801409E8[index];
            for(section_index=0;section_index<D_801407F0.section_count;section_index++) {
                section=&D_801407F0.sections[section_index];
                if(section_point>=section->points&&section_point<section->points+section->count) break;
            }
            section=&D_801407F0.sections[section_index];
            if(!section->active) {
                index+=section->count-1;
                continue;
            }
        }
        point=&D_801407F0.points[index];
        px=(point->x-D_801407D4.x)*scaled_x/dx+offset_x;
        py=(point->z-D_801407D4.z)*scaled_z/dz+offset_z;
        left=px-2;right=px+2;top=py-2;bottom=py+2;
        for(y=top;y<=bottom;y++) {
            for(x=left;x<=right;x++) {
                if(x>=0&&x<D_801161C4&&y>=0&&y<D_801161C4) {
                    pixel=sprite->texture->image+((D_801161C4-y-1)*D_801161C4)/2;
                    plot_x=x;
                    if(!D_80140A04) plot_x=D_801161C4-x-1;
                    pixel+=plot_x/2;
                    if(left<x&&x<right&&top<y&&y<bottom) {
                        if(plot_x&1) *pixel=(*pixel&0xf0)|14;
                        else *pixel=(*pixel&0xf)|224;
                    } else {
                        if(plot_x&1) {
                            if((*pixel&0xf)!=14) *pixel=(*pixel&0xf0)|13;
                        } else {
                            if((*pixel&0xf0)!=224) *pixel=(*pixel&0xf)|208;
                        }
                    }
                }
            }
        }
    }
    return 1;
}
