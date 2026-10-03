/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct Resource24 { char name[16]; void *data; unsigned int length; } Resource24;
typedef struct Pool8 { Resource24 *base; int count; } Pool8;
extern u8 D_80140BDC;
extern Pool8 D_80138670[];
extern char *func_800A473C(char *,char *);
extern int func_80092D80(int);
extern int pointer_compare_thunk(char *,Resource24 *);
extern Resource24 *entity_name_copy(char *,Resource24 *,int,int,int (*)(char *,Resource24 *));
Resource24 *sound_bank_load(char *input,u16 *identifier,s8 first,s8 last)
{
    char name[16];
    int i;
    Resource24 *resource=0;
    func_800A473C(name,input);
    if(first<0)first=0;
    if(last<0 || last>=D_80140BDC)last=D_80140BDC-1;
    for(i=first;i<=last;i++) {
        if(func_80092D80(i)) {
            resource=entity_name_copy(name,D_80138670[i].base,D_80138670[i].count,24,pointer_compare_thunk);
            if(resource)break;
        }
    }
    if(!resource) { *identifier=0; return 0; }
    *identifier=(resource-D_80138670[i].base)|(i<<10);
    return resource;
}
