/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed short s16;
typedef unsigned int u32;
typedef struct Entry20 {void *object;char name[16];} Entry20;
extern s16 D_80149D90;
extern u32 *D_801497FC;
extern Entry20 *D_80149818;
extern void * volatile D_80149B80;
extern char *func_800A473C(char *,char *);
extern Entry20 *entity_name_copy(Entry20 *,Entry20 *,u32,u32,int (*)(Entry20 *,Entry20 *));
extern int pointer_offset_wrapper(Entry20 *,Entry20 *);
extern void differential_output(void *,s16);
extern void func_800AB638(void);
s16 wheel_torque_apply(char *name,s16 index)
{
    Entry20 *found;
    Entry20 key;
    D_80149D90=-1;
    func_800A473C(key.name,name);
    found=entity_name_copy(&key,D_80149818,*D_801497FC,20,pointer_offset_wrapper);
    D_80149B80=found->object;
    differential_output(D_80149B80,index);
    func_800AB638();
    return D_80149D90;
}
