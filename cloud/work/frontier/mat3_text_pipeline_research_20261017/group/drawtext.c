/* _DrawText donor with native N64 backend calls. Glyph/header member views
 * retain the genuine font-context declarations; 87110 remains external. */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;
typedef unsigned short u16;typedef signed int s32;typedef unsigned int u32;typedef float f32;
typedef struct Bank {u8 pad0;u8 mode;u8 f2;u8 f3;u8 f4;u8 pad5;u8 f6;u8 f7;u8 f8;u8 f9;u8 f10;u8 f11;u8 f12;u8 padD[3];} Bank;
typedef struct {u8 *ptr;u8 id;u8 pad[7];} Entry;
typedef struct {u8 prefix[16];u16 width,height;u32 field20;void *image;u32 tail[2];} FontTexture36;
extern u8 *D_8012E6D0;
extern Bank *D_801497F0;extern Entry *D_80149800;extern s16 D_80149878[256];extern s32 D_80149820[];
extern s32 D_8002AFC0,D_8002AFC4,D_80149B88,D_80149B08,D_80149B28;
typedef struct NativeTextColor { union { unsigned char rgba[8]; unsigned int rgbaU32; } fore; } NativeTextColor;
extern NativeTextColor D_80149B48;extern f32 D_80114748;extern s16 D_80149D92,D_80149D9E;extern s8 D_80149B60,D_80149B70;
void sound_update_channel(s32);void func_8008A3E4(s32,s32,s32,s32);
void func_800878E0(u32),func_8008705C(u32),func_8008A148(u8 *,s32,s32,s32);
void object_render(void *,u16,u16,u16,u16,u16,u16,u16,u16,u16,s32);
void func_80087110(s32,s32,s32,s32,s32,s32);
#define TEX(n) ((FontTexture36*)D_80149820[n])
#define LOADPAGE(n) object_render(TEX(n)->image,D_80149B08,D_80149B28,TEX(n)->width,TEX(n)->height,0,0,TEX(n)->width-1,TEX(n)->height-1,0,0)
#define ULS(l) ((l)->pad[1])
#define ULT(l) ((l)->pad[2])
#define LRS(l) ((l)->pad[3])
#define LRT(l) ((l)->pad[4])
void audio_doppler_calc(const u8 *text,u16 length)
{
 s32 i; s32 x; s32 x0; s32 y0; s32 step; s32 ch; s32 chPrev; s32 lastPage;
 const u8 *t; s32 dbl; Entry *l;
 sound_update_channel(0);
 func_8008A3E4(0,0,D_8002AFC0-1,D_8002AFC4-1);
 func_800878E0(D_80149B88);func_8008705C(~D_80149B88);D_8012E6D0=0;
 if(D_80149B88&32)D_80149B48.fore.rgba[3]=D_80114748;
 func_8008A148(D_80149B48.fore.rgba,D_80149B08,D_80149B28,0);
 lastPage=0;LOADPAGE(lastPage);
 if(text[0]==255){t=text+1;dbl=1;step=2;length-=2;}
 else {t=text;dbl=0;step=1;length-=1;}
 x0=D_80149D92;y0=D_80149D9E;i=0;chPrev=-1;
 for(;i<length;){
  if(dbl)ch=(t[i]<<8)|t[i+1];else ch=t[i];
  if(ch==10){ch=-1;y0+=D_801497F0->f2+D_801497F0->f3+D_80149B60;x0=D_80149D92;}
  else {
   if(ch==32||ch>255){
space:
    x0+=D_801497F0->f9?D_801497F0->f6+D_80149B70:D_801497F0->f7;ch=-1;
   }else{
    ch=D_80149878[ch];if(ch<0)goto space;
    if(chPrev>0&&D_801497F0->f10&&!D_801497F0->f9)x0-=D_80149800[ch].ptr[chPrev];
    l=D_80149800+ch;
    if(l->pad[0]!=lastPage){lastPage=l->pad[0];LOADPAGE(lastPage);}
    x=D_801497F0->f9?(D_801497F0->f6-(LRS(l)-ULS(l)+1))/2:0;
    x=x0+x;
    func_80087110(x,y0,x+LRS(l)-ULS(l),y0+LRT(l)-ULT(l),ULS(l),ULT(l));
    x0+=(D_801497F0->f9?D_801497F0->f6:LRS(l)-ULS(l)+1)+D_80149B70;
   }
  }
  i+=step;chPrev=ch;
 }
}
