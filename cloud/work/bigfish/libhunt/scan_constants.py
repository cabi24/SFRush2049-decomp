import struct,sys,re
img=open('/tmp/claude-0/-home-user-SFRush2049-decomp/37f33778-7a6e-51d1-85db-44651954a6c5/scratchpad/img.bin','rb').read()
B=0x80086A50
consts={
'crc32_poly_rev':0xEDB88320,'crc32_tab1':0x77073096,'crc32_fwd':0x04C11DB7,
'lcg_numrec':1664525,'lcg_libc':1103515245,'lcg_ansi_hex':0x41C64E6D,'lcg_borland':22695477,'lcg_69069':69069,'lcg_drand48':0x5DEECE66D&0xffffffff,
'mt19937':0x9908B0DF,'md5_a':0xD76AA478,'sha1_h0':0x67452301,'sha1_k':0x5A827999,'sha256_k':0x428A2F98,'fastinvsqrt':0x5F3759DF,
'adler_base':65521,'fnv':0x811C9DC5,'fnv_prime':16777619,'golden':0x9E3779B9,
}
for n,v in consts.items():
    b=struct.pack('>I',v); hits=[m.start() for m in re.finditer(re.escape(b),img) if m.start()%4==0]
    print(n,hex(v),[hex(B+h) for h in hits[:6]])
words=struct.unpack('>%dI'%(len(img)//4),img)
for n,v in consts.items():
    hi=v>>16; lo=v&0xffff; hits=[]
    for i,w in enumerate(words):
        if w>>26==0xF and (w&0xffff)==hi:
            for j in range(1,6):
                if i+j>=len(words): break
                x=words[i+j]
                if x>>26 in (0xD,9) and (x&0xffff)==lo and ((x>>21)&31)==((w>>16)&31): hits.append(hex(B+4*i))
    if hits: print('  code',n,hits)
