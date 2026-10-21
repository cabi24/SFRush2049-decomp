/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned int u32;
typedef int s32;
typedef signed char s8;

/* libc ANSI rand(): seed at D_8011735C. No callers (all inlined elsewhere). */
extern int D_8011735C;
int func_8008B2B4(void) {
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (D_8011735C >> 16) & 0x7fff;
}
