/* Independent host formatting-length check. Input formats are authenticated by
 * verify.py and passed only through a temporary pipe. No native data is saved.
 * All arrays have ample storage; this does not intentionally overflow C. */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
static unsigned read32(void) {
    unsigned char b[4];
    if (fread(b,1,4,stdin)!=4) exit(2);
    return ((unsigned)b[0]<<24)|((unsigned)b[1]<<16)|((unsigned)b[2]<<8)|b[3];
}
int main(void) {
    unsigned i, maximum=0;
    for(i=0;i<12;i++) {
        char format[128], name[32], output[256];
        unsigned n=read32(), k=read32(), expected=read32(); int written;
        if(n>=sizeof(format)||k>=sizeof(name))return 2;
        if(fread(format,1,n,stdin)!=n||fread(name,1,k,stdin)!=k)return 2;
        format[n]=0; name[k]=0;
        written=snprintf(output,sizeof(output),format,name);
        if(written<0||(unsigned)written>=sizeof(output)||
           (unsigned)written+1!=expected)return 3;
        if((unsigned)written+1>maximum)maximum=(unsigned)written+1;
    }
    if(getchar()!=EOF||maximum!=65)return 4;
    printf("12 authentic-format cases; maximum %u bytes including NUL\n",maximum);
    return 0;
}
