s=open("as1.c").read()
hdr="static void func_42aa0c(uint8_t *mem, uint32_t sp) {"
i=s.index(hdr)
j=s.index("L42aa0c:",i)
inj=r'''
if (getenv("AS1BB")) { uint32_t n = MEM_U32(0x100232fc); uint32_t arr = MEM_U32(0x100235d4);
  for (uint32_t k = 0; k < n; k++) { uint32_t b = MEM_U32(arr + 4*k); printf("BBX %u id=%u w=%u st=%u fl=%08x preds:", k, MEM_U32(b+64), MEM_U16(b+72), MEM_U16(b+76), MEM_U32(b+68));
    for (uint32_t p = MEM_U32(b+20); p; p = MEM_U32(p+4)) printf(" %u", MEM_U32(MEM_U32(p)+64));
    printf("\n"); } }
'''
s=s[:j]+inj+s[j:]
open("as1.c","w").write(s)
