/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed char s8;
typedef struct Color4 {u8 r,g,b,a;} Color4;
typedef struct Color8 {Color4 color;u8 extra[4];} Color8;
extern s8 D_80118E1C;
extern Color4 D_80118E2C,D_80118E28;
extern float D_8017A630;
extern Color8 D_80118E34[];
extern void func_800947F0(void);
extern void func_800B7438(void (*)(void));
extern void func_800B7360(u8,u8,u8,u8);
void dispatch_handler(int mode)
{
 if(mode==22) {
  if(!D_80118E1C) {
   if(!D_80118E1C) {
    D_80118E1C=1;func_800947F0();func_800B7438(func_800947F0);
   }
  }
  func_800B7360(
   (u8)(D_80118E2C.r*(1.0f-D_8017A630)+D_80118E28.r*D_8017A630),
   (u8)(D_80118E2C.g*(1.0f-D_8017A630)+D_80118E28.g*D_8017A630),
   (u8)(D_80118E2C.b*(1.0f-D_8017A630)+D_80118E28.b*D_8017A630),
   (u8)(D_80118E2C.a*(1.0f-D_8017A630)+D_80118E28.a*D_8017A630));
 } else {
  func_800B7360(D_80118E34[mode].color.r,D_80118E34[mode].color.g,
   D_80118E34[mode].color.b,D_80118E34[mode].color.a);
 }
}
