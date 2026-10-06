/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * records_screen (historical name), 0x800D58CC..0x800D5A04.
 * Releases two voice lists, removes both scene-node IDs in each 104-byte row,
 * clears row status and cleanup state, then invokes the mode 4/6 callback.
 * No arcade ancestor established; reconstructed from the complete native body.
 *
 * Matching details: the named, consumed -1 sentinel preserves comparison
 * operand order. The status-then-row initializers share one source line to
 * preserve the IDO/as1 load-address schedule. Keep the statement layout.
 * All locals are consumed; no artificial padding, volatile, or helper context.
 */
typedef signed short s16;
typedef unsigned char u8;
typedef struct Entry {s16 id;u8 data[50];} Entry;
typedef struct Row {Entry a,b;} Row;
typedef struct Voice Voice;
extern Voice *D_80154198, *D_801541A0;
extern int D_80154360,D_80154350[];
extern s16 D_80151AD0;
extern Row D_801541A8[];
extern int D_8014A110;
void sound_handles_clear(int);
void sound_stop(Voice *);
void entity_spawn_callback(s16,int,int);
void func_8039244C(void);
void records_screen(void) {
    int i,id;
    const int sentinel = -1;
    Row *row;
    int *status;
    if(D_80154198!=0){
        sound_handles_clear(1);
        sound_stop(D_80154198);
        D_80154198=0;
        i=0;
        if(D_80151AD0>0){
            status=D_80154350; row=D_801541A8;
            do{
            id=row->a.id;
            *status=0;
            if(sentinel!=id){
                entity_spawn_callback((s16)id,0,0);
                row->a.id=sentinel;
            }
            id=row->b.id;
            if(sentinel!=id){
                entity_spawn_callback((s16)id,0,0);
                row->b.id=sentinel;
            }
            i++;status++;row++;
            }while(i<D_80151AD0);
        }
        D_80154360=0;
    }
    if(D_801541A0!=0){
        sound_stop(D_801541A0);
        D_801541A0=0;
    }
    if(D_8014A110==6 || D_8014A110==4)func_8039244C();
}
