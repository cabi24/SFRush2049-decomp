# as1_patch_w.py: instrument func_429534 (xbb block weights) with AS1W=1 prints. Run in as1dbg dir after build_as1dbg patches.
s=open("as1.c").read()
hdr="static void func_429534(uint8_t *mem, uint32_t sp) {"
i=s.index(hdr); j=s.index("\nstatic ", i+len(hdr))
L=s[i:j].split("\n")
P='if (getenv("AS1W")) printf'
out=[]; seen={}
for ln in L:
    k=ln.strip()
    c=seen.get(k,0); seen[k]=c+1
    if k=="MEM_U16(v1 + 72) = (uint16_t)s2;":
        out.append(P+('("W blk=%u w=%d\\n", s7, (int)(int16_t)s2);' if c==0 else '("  prop blk=%u from %u w=%d\\n", v0, s7, (int)(int16_t)s2);'))
    if k=="s0 = s0 + 0xfffffffd;" and c==0:
        out.append(P+'("  blk=%u pred=%u w=%d -3\\n", s7, MEM_U32(a3+64), (int)s0);')
    if k=="s0 = s0 + 0x1;" and c==0:
        out.append(P+'("  blk=%u pred=%u w=%d +1\\n", s7, MEM_U32(a3+64), (int)s0);')
    if k=="s0 = s0 + 0x3;" and c==0:
        out.append(P+'("  blk=%u pred=%u w=%d +3\\n", s7, MEM_U32(a3+64), (int)s0);')
    out.append(ln)
    if k.startswith("v0 = func_429414(") :
        out.append(P+'("  blk=%u pred=%u a2path -> %d\\n", s7, MEM_U32(a3+64), (int)v0);')
s=s[:i]+"\n".join(out)+s[j:]
open("as1.c","w").write(s)
print("patched", seen.get("MEM_U16(v1 + 72) = (uint16_t)s2;"))
