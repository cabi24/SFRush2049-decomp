/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef signed short s16;
typedef unsigned char u8;
typedef struct Record {int word0,id;u8 tail[56];} Record;
extern Record D_80111998[4][17];
extern int D_80154618[4],D_80113ED8;
extern s8 D_8015418C[4],D_8014A10C[4],D_80113ED4;
void entity_spawn_callback(s16,int,int);
void sound_stop(int);
void render_replay_ui(s8 row) {
    int i;
    Record *record=D_80111998[row];
    for(i=0;i<17;i++,record++){
        if(record->id!=-1){
            entity_spawn_callback((s16)record->id,0,0);
            record->id=-1;
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
