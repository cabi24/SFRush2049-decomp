import sys
# extends the GETTEMP print: index of the chosen home (entry+0) and the free flags (+12) of the first 10 list entries
src, dst = sys.argv[1], sys.argv[2]
s = open(src).read()
old = 'fprintf(stderr, "GETTEMP proc=%s size=%u found=%u area=%d\\n", nm, a2, v0, (int)MEM_U32(0x1001c4b4)); }'
assert s.count(old) == 1, s.count(old)
new = ('{ char fl[64]; uint32_t e = MEM_U32(0x1001c4b8); int n = 0; while (e && n < 12) { fl[n++] = MEM_U8(e + 12) ? \'F\' : \'b\'; e = MEM_U32(e + 16); } fl[n] = 0; '
       'fprintf(stderr, "GETTEMP proc=%s size=%u found=%u area=%d idx=%d flags=%s\\n", nm, a2, v0, (int)MEM_U32(0x1001c4b4), (v0 && v1) ? (int)MEM_U32(v1 + 0) : -1, fl); } }')
s = s.replace(old, new)
open(dst, "w").write(s)
