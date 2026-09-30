/* flags: -g0 -O2 -mips2 -G 0 -non_shared */

/*@@HDR 1 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3306@@*/

/*@@HDR 3307 3697@@*/
#endif

extern f32 D_801243E8;
typedef struct CarE {
  u8 pad0[0xA]; s8 fA;
  u8 padB[0x3D0 - 0xB]; f32 f3D0;
  u8 pad3D4[0x3F4 - 0x3D4]; s16 f3F4;
  u8 pad3F6[0x404 - 0x3F6]; f32 f404; f32 f408; s32 f40C;
  u8 pad410[0x614 - 0x410]; s32 f614; s32 f618;
  u8 pad61C[0x640 - 0x61C]; s8 f640;
} CarE;
void func_800E2F00(CarE *arg0)
{
  if (arg0->f640 != 0) {
    arg0->f404 = func_800E2D18(arg0, (s16) (s32) (arg0->f408 * D_801243E0), 0, arg0->f40C);
  } else if (arg0->f3F4 < 0) {
    arg0->f404 = func_800E2D18(arg0, (s16) (s32) (arg0->f408 * D_801243E4), (s16) (s32) (arg0->f3D0 * 128.0f), arg0->f40C);
  } else {
    arg0->f404 = func_800E2D18(arg0, (s16) (s32) (arg0->f408 * D_801243E8), (s16) (s32) (arg0->f3D0 * 128.0f), arg0->f40C);
  }
  if ((arg0->f614 == 1 || arg0->f614 == 2) && (arg0->f618 == 1 || arg0->f618 == 2)) {
    arg0->f404 = arg0->f404 * 1.5f;
  }
  if (arg0->fA != 0 && arg0->f3F4 != 4) {
    arg0->f404 = arg0->f404 * D_801243EC;
  }
}
