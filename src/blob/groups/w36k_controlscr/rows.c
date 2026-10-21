/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef signed short s16;
typedef unsigned char u8;
typedef struct Item { int type, handle; float angle, matrix[3][3], position[3]; u8 alpha, pad[3]; } Item;
extern Item D_80111998[4][17];
extern int D_80154618[4];
extern int D_80113ED8;
extern s8 D_8015418C[4],D_8014A10C[4],D_80113ED4;
void entity_spawn_callback(s16,int,int);
void sound_stop(int);
void render_replay_ui(s8 row) {
    int i;
    Item *record=D_80111998[row];
    /* Native loop counter measures the actual row span in bytes. */
    for(i=0;i<sizeof(D_80111998[0]);i+=sizeof(Item),record++){
        if(record->handle!=-1){
            entity_spawn_callback((s16)record->handle,0,0);
            record->handle=-1;
        }
    }
    D_80154618[row]=0;
}
void render_results_screen(void) {
    int i;
    for(i=0;i<4;i++){
        D_8015418C[i]=0;
        D_8014A10C[i]=0;
        render_replay_ui((s8)i);
    }
    if(D_80113ED8!=0){
        sound_stop(D_80113ED8);
        D_80113ED8=0;
    }
    D_80113ED4=0;
}
/* w36k note: render_replay_ui = uninitButtons@controlscr.c (GC 0x8006D1D8; p in s4, private ABI),
 * render_results_screen = cleanupControlScreen (GC 0x8006BB9C). Bodies from Astra's PR #322 packet, unchanged. */
