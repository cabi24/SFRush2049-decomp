/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8; typedef unsigned short u16; typedef short s16; typedef float f32;
typedef union Dimension32 { int value; u16 half[2]; } Dimension32;
typedef struct Rectangle8 { s16 left,top,right,bottom; } Rectangle8;
typedef struct Viewport16 { s16 scale[4],translate[4]; } Viewport16;
typedef struct Descriptor72 { void *first,*bounds; int orthographic; f32 horizontal,vertical,tan_horizontal,tan_vertical,inverse_horizontal,inverse_vertical,width,height,near_plane,far_plane,aspect,zoom,fog; s16 range_start,range_end; unsigned char red,green,blue,alpha; } Descriptor72;
extern Dimension32 D_8002AFC0,D_8002AFC4;
extern s8 D_80146204,D_8017A63C;
extern f32 D_80154188,D_80123BDC,D_80123BE0,D_80123BE4,D_80123BE8;
extern Rectangle8 D_80149870,D_80149B00,D_80149B20,D_80149B40,D_80149B58,D_80149B68,D_80149B78;
extern Viewport16 D_8011EA30,D_8011EA40,D_8011EA50,D_8011EA60,D_8011EA70,D_8011EA80,D_8011EA90;
/* Original descriptor addresses are 72 bytes apart. These are external views, not owned storage. */
extern Descriptor72 D_8017A510,D_8017A558,D_8017A5A0,D_8017A5E8;
extern void arb_rate_set(int,void *,void *,f32,f32,f32,f32,f32,f32);
int car_render_full(int count)
{
    int width,height;
    if(count>0) {
        D_80146204=count;
        if(count==1) {
            width=D_8002AFC0.value; height=D_8002AFC4.value;
            arb_rate_set(0,&D_8011EA30,&D_80149870,D_80154188,0.0f,(f32)width,(f32)height,(f32)(width/2),(f32)(height/2));
            width=D_8002AFC0.half[1]; height=D_8002AFC4.half[1];
            D_80149870.left=0; D_80149870.top=0;
            D_8017A510.first=&D_8011EA30; D_8017A510.bounds=&D_80149870;
            D_80149870.right=width; D_80149870.bottom=height;
            D_8017A63C=0;
        } else if(count==2) {
            width=D_8002AFC0.value; height=D_8002AFC4.value;
            arb_rate_set(0,&D_8011EA40,&D_80149B00,(D_80154188*127.0f)/90.0f,0.0f,(f32)width,(f32)(height/2),(f32)(width/2),(f32)(height/4));
            height=D_8002AFC4.value; width=D_8002AFC0.value;
            D_80149B00.bottom=height/2-1; D_80149B00.right=width;
            D_80149B00.left=0; D_80149B00.top=0;
            D_8017A510.first=&D_8011EA40; D_8017A510.bounds=&D_80149B00;
            arb_rate_set(1,&D_8011EA50,&D_80149B20,(D_80154188*127.0f)/90.0f,0.0f,(f32)width,(f32)(height/2),(f32)(width/2),(f32)((height*3)/4));
            height=D_8002AFC4.value; width=D_8002AFC0.half[1];
            D_80149B20.top=height/2+1; D_80149B20.left=0;
            D_8017A558.first=&D_8011EA50; D_8017A558.bounds=&D_80149B20;
            D_80149B20.bottom=height; D_80149B20.right=width;
            D_8017A63C=1;
        } else if(count==3 || count==4) {
            width=D_8002AFC0.value; height=D_8002AFC4.value;
            arb_rate_set(0,&D_8011EA60,&D_80149B40,D_80154188*D_80123BDC,0.0f,(f32)(width/2),(f32)(height/2),(f32)(width/4+1),(f32)(height/4+1));
            width=D_8002AFC0.value; height=D_8002AFC4.value;
            D_80149B40.bottom=height/2-1; D_80149B40.right=width/2-1;
            D_80149B40.left=0; D_80149B40.top=0;
            D_8017A510.first=&D_8011EA60; D_8017A510.bounds=&D_80149B40;
            arb_rate_set(1,&D_8011EA70,&D_80149B58,D_80154188*D_80123BE0,0.0f,(f32)(width/2),(f32)(height/2),(f32)((width*3)/4-2),(f32)(height/4+1));
            width=D_8002AFC0.value; height=D_8002AFC4.value;
            D_80149B58.bottom=height/2-1; D_80149B58.right=width; D_80149B58.left=width/2+1; D_80149B58.top=0;
            D_8017A558.first=&D_8011EA70; D_8017A558.bounds=&D_80149B58;
            arb_rate_set(2,&D_8011EA80,&D_80149B68,D_80154188*D_80123BE4,0.0f,(f32)(width/2),(f32)(height/2+2),(f32)(width/4+1),(f32)((height*3)/4+2));
            height=D_8002AFC4.value; width=D_8002AFC0.value;
            D_80149B68.bottom=height; D_80149B68.right=width/2-1; D_80149B68.top=height/2+1; D_80149B68.left=0;
            D_8017A5A0.first=&D_8011EA80; D_8017A5A0.bounds=&D_80149B68;
            arb_rate_set(3,&D_8011EA90,&D_80149B78,D_80154188*D_80123BE8,0.0f,(f32)(width/2),(f32)(height/2+2),(f32)((width*3)/4-2),(f32)((height*3)/4+2));
            width=D_8002AFC0.value; height=D_8002AFC4.value;
            D_80149B78.top=height/2+1; D_80149B78.left=width/2+1;
            D_8017A5E8.first=&D_8011EA90; D_8017A5E8.bounds=&D_80149B78;
            D_80149B78.right=width; D_80149B78.bottom=height;
            D_8017A63C=2;
        }
    }
    return D_8017A63C;
}
