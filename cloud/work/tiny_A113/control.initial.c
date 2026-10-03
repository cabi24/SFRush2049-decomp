/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned int u32;
void display_list_traverse(u32 *cursor,u32 original,u32 replacement,u32 flags,int (*callback)(u32 *))
{
    while(*cursor!=0xdf000000) {
        u32 command=*cursor;
        if((command&0xff000000)==0xde000000) {
            u32 address=cursor[1];
            cursor++;
            if(address&0x0f000000)address&=0xf0ffffff;
            else if((address&0xff000000)==0)address+=0x80000000;
            if((flags&8) && address==original)*cursor=replacement;
            cursor++;
            if((command&0x00ff0000)==0) {
                if((flags&4)==0)display_list_traverse((u32 *)address,original,replacement,flags,callback);
            } else cursor=(u32 *)address;
        } else if((command&0xff000000)==0xe1000000) {
            u32 address=cursor[1];
            cursor+=2;
            if(address&0x0f000000)address&=0xf0ffffff;
            else if((address&0xff000000)==0)address+=0x80000000;
            if((*cursor&0xff000000)==0x04000000) {
                cursor+=2;
                if(flags&2)cursor=(u32 *)address;
                else if(flags&1)display_list_traverse((u32 *)address,original,replacement,flags,callback);
            } else cursor+=2;
        } else {
            if(callback)cursor+=callback(cursor);
            else cursor+=2;
        }
    }
}
