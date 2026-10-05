/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef struct GhostHandle GhostHandle;
extern int func_800CBF2C(GhostHandle *,int,int);
int menu_item_value_get(GhostHandle *handle)
{
    return func_800CBF2C(handle,1,1);
}
