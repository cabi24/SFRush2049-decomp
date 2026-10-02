/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef int s32;
void Input_ApplyPadConfig(void *);
typedef struct Config64 {u8 pad0[14];s16 x,y;u8 pad18[42];struct Config64 *next;} Config64;
void game_timer_reset(Config64 *config,s16 dx,s16 dy) {
 if(config) {
  config->x+=dx;
  config->y+=dy;
  Input_ApplyPadConfig(config);
  game_timer_reset(config->next,dx,dy);
 }
}
