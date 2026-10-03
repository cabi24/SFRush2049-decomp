/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned int u32;
typedef unsigned char u8;
void *entity_name_copy(const void *key, void *array, u32 count, u32 size,
                      int (*compare)(const void *,const void *)) {
    u8 *base=array;
    u8 *end;
    u8 *middle;
    u8 *next;
    int half;
    int result;
    if(count==0 || size==0)return 0;
    end=(u8 *)((u32)base+count*size);
    while(count!=0){
        half=count>>1;
        middle=(u8 *)((u32)base+half*size);
        next=middle;
        result=compare(key,middle);
        if(result<0)count=half;
        else if(result>0){
            base=next+size;
            count=half-((count&1)?0:1);
        }else return middle;
    }
    if(base<end && compare(key,base)==0)return base;
    return 0;
}
