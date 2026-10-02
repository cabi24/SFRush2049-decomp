/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
extern char *D_80110D3C[13],*D_8011B42C[3];
extern char D_801233B8[];
extern u16 D_801427C0[][3];
extern int sprintf(char *,const char *,...);
extern u16 string_copy_format(char *,s8,s8,int);
void sfx_volume_set(s16 group,s16 kind,s8 bank)
{
    char name[16];
    int i;
    for(i=0;i<3;i++) {
        sprintf(name,D_801233B8,D_80110D3C[kind],D_8011B42C[i]);
        D_801427C0[group][i] = string_copy_format(name,bank,bank,1);
    }
}
