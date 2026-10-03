import zlib,sys
d=open('baserom.us.z64','rb').read(); mv=memoryview(d)
def trial(o):
    z=zlib.decompressobj(-15); out=0; p=o
    try:
        while p<len(d):
            chunk=mv[p:p+4096]; r=z.decompress(chunk); out+=len(r); 
            if z.eof:
                used=len(chunk)-len(z.unused_data); return p+used-o,out
            p+=4096
            if out==0 and p-o>16384: return None
    except zlib.error: return None
    return None
o=0x10000; end=0xB80000; n=0
while o<end:
    t=trial(o)
    if t and t[1]>=512:
        print(f'{o:07X} comp={t[0]} out={t[1]}',flush=True); o+=t[0]; n+=1; continue
    o+=1
print('streams',n)
