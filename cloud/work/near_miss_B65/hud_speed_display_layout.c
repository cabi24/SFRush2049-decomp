/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;typedef float f32;
typedef struct Node {struct Node *next,*previous;u8 enabled,busy;} Node;
typedef struct Node60 {Node *next,*previous;u8 enabled,busy;u8 rest[50];} Node60;
typedef struct Node44 {Node *next,*previous;u8 enabled,busy;u8 rest[34];} Node44;
typedef struct Header {u8 indirect,doubly;u8 gap[2];int count;Node *head,*tail;} Header;
extern Header D_80146170,D_80146188,D_801461B0,D_801461E8;
extern int D_80110268,D_8011026C;
extern Node60 *D_8011025C;
extern Node44 *D_80110260;
extern f32 D_80123F90,D_80139318,D_8013C090;
extern u8 D_801497E8;
extern void *audio_dma_sync(int,int);
extern void audio_loop_control(void *,int),func_80091FBC(Header *,Node *,Node *);
#define INIT(h) (h).doubly=1;(h).indirect=0;(h).head=0;(h).tail=0;(h).count=0
void hud_speed_display(int first,int second,f32 scale,f32 other,int mode) {
 int i;
 INIT(D_80146170);
 INIT(D_80146188);
 INIT(D_801461B0);
 INIT(D_801461E8);
 if(first>0 && second>0) {
  D_80110268=first;
  D_8011025C=audio_dma_sync(0,D_80110268*60);
  audio_loop_control(D_8011025C,0);
  for(i=0;i<D_80110268;i++) {
   D_8011025C[i].enabled=1;
   D_8011025C[i].busy=0;
   func_80091FBC(&D_80146170,(Node *)&D_8011025C[i],D_80146170.head);
  }
  D_8011026C=second;
  D_80110260=audio_dma_sync(0,second*44);
  audio_loop_control(D_80110260,0);
  for(i=0;i<D_8011026C;i++) {
   D_80110260[i].enabled=1;
   D_80110260[i].busy=0;
   func_80091FBC(&D_801461B0,(Node *)&D_80110260[i],D_801461B0.head);
  }
  D_80139318=D_80123F90*scale;
  D_8013C090=other;
  D_801497E8=mode;
 }
}
