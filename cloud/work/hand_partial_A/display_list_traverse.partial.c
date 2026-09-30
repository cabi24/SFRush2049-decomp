void display_list_traverse(u32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 (*arg4)(u32 *))
{
  u32 *p;
  u32 w0;
  u32 op;
  u32 addr;

  p = arg0;
  w0 = *p;
  if (w0 != 0xDF000000) {
    do {
      op = w0 & 0xFF000000;
      if (op == 0xDE000000) {
        addr = p[1];
        p += 1;
        if (addr & 0x0F000000) {
          addr &= 0xF0FFFFFF;
        } else if (!(addr & 0xFF000000)) {
          addr += 0x80000000;
        }
        if ((arg3 & 8) && addr == arg1) {
          *p = arg2;
        }
        p += 1;
        if (!(w0 & 0xFF0000)) {
          if (!(arg3 & 4)) {
            display_list_traverse((u32 *) addr, arg1, arg2, arg3, arg4);
          }
        } else {
          p = (u32 *) addr;
        }
      } else if (op == 0xE1000000) {
        addr = p[1];
        p += 2;
        if (addr & 0x0F000000) {
          addr &= 0xF0FFFFFF;
        } else if (!(addr & 0xFF000000)) {
          addr += 0x80000000;
        }
        if ((*p & 0xFF000000) == 0x04000000) {
          p += 2;
          if (arg3 & 2) {
            p = (u32 *) addr;
          } else if (arg3 & 1) {
            display_list_traverse((u32 *) addr, arg1, arg2, arg3, arg4);
          }
        } else {
          p += 2;
        }
      } else if (arg4 != 0) {
        p += arg4(p);
      } else {
        p += 2;
      }
      w0 = *p;
    } while (w0 != 0xDF000000);
  }
}
