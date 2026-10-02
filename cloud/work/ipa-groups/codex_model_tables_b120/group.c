/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned int u32;
typedef unsigned short u16;
typedef struct Model20 {u32 *data;unsigned char tail[16];} Model20;
typedef struct Model40 {
    u16 address_count;
    u16 reserved0;
    u32 *addresses;
    u16 index_count;
    u16 address2_count;
    u16 *indices;
    u32 *addresses2;
    u16 value_count;
    u16 value2_count;
    u16 value3_count;
    u16 reserved1;
    u16 *values;
    u16 *values2;
    u32 *values3;
} Model40;
extern int D_80140AF0;
extern Model20 D_80156D44[];
extern Model40 D_8017A4E0;
/* The protected four-word original tests a2 and returns on both paths.
 * Its caller deliberately supplies all three actual argument slots. */
void func_80096288(int first,int second,int condition)
{
    if(condition) {}
}
void func_800A4E58(void)
{
    u32 *base,*cursor;
    int i;
    func_80096288(D_80140AF0,0,0);
    base=D_80156D44[D_80140AF0].data;
    cursor=base;
    D_8017A4E0.address_count=*cursor++;
    D_8017A4E0.addresses=cursor;
    cursor+=D_8017A4E0.address_count;
    D_8017A4E0.index_count=*cursor++;
    D_8017A4E0.indices=(u16 *)cursor;
    cursor=(u32 *)((u16 *)cursor+((D_8017A4E0.index_count+1)&~1));
    D_8017A4E0.address2_count=*cursor++;
    D_8017A4E0.addresses2=cursor;
    cursor+=D_8017A4E0.address2_count;
    D_8017A4E0.value_count=*cursor++;
    D_8017A4E0.values=(u16 *)cursor;
    cursor=(u32 *)((u16 *)cursor+((D_8017A4E0.value_count+1)&~1));
    D_8017A4E0.value2_count=*cursor++;
    D_8017A4E0.values2=(u16 *)cursor;
    cursor=(u32 *)((u16 *)cursor+((D_8017A4E0.value2_count+1)&~1));
    D_8017A4E0.value3_count=*cursor++;
    D_8017A4E0.values3=cursor;
    for(i=0;i<D_8017A4E0.address_count;i++)D_8017A4E0.addresses[i]+=(u32)base;
    for(i=0;i<D_8017A4E0.address2_count;i++)D_8017A4E0.addresses2[i]+=(u32)base;
    for(i=0;i<D_8017A4E0.value3_count;i++)D_8017A4E0.values3[i]+=(u32)base;
}
