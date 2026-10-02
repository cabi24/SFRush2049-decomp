/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef signed short s16;
typedef unsigned char u8;
typedef struct Entry {s16 id;u8 data[50];} Entry;
typedef struct Row {Entry a,b;} Row;
extern int D_80154198,D_801541A0,D_80154360,D_80154350[];
extern s16 D_80151AD0;
extern Row D_801541A8[];
extern int D_8014A110;
void sound_handles_clear(int);
void sound_stop(int);
void entity_spawn_callback(s16,int,int);
void func_8039244C(void);
void records_screen(void) {
    int i,id;
    Row *row;
    int *status;
    if(D_80154198!=0){
        sound_handles_clear(1);
        sound_stop(D_80154198);
        D_80154198=0;
        i=0;
        if(D_80151AD0>0){
            status=D_80154350;
            row=D_801541A8;
            do{
            id=row->a.id;
            *status=0;
            switch(id){
            case -1:break;
            default:
                entity_spawn_callback((s16)id,0,0);
                row->a.id=-1;
                break;
            }
            id=row->b.id;
            switch(id){
            case -1:break;
            default:
                entity_spawn_callback((s16)id,0,0);
                row->b.id=-1;
                break;
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
