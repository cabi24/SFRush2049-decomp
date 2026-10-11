cd ~/rush2049/scratch/frontier/w15g
H='typedef long long s64; typedef unsigned long long u64; typedef short s16; typedef unsigned short u16; typedef int s32; typedef unsigned u32;'
i=0
while IFS= read -r body; do i=$((i+1)); echo "$H
$body" > /tmp/qd$i.c
for fl in -O1 -O2; do echo "v$i $fl: $(python3 sc.py /tmp/qd$i.c __qdivrem --flags "-g0 $fl -mips3 -32 -G 0 -non_shared" 2>&1 | tail -1 | cut -c1-40) :: $body"; done; done <<'EOF'
void __qdivrem(u64 *q, u64 *r, u64 a, s16 b){ u64 d = b; *q = a/d; *r = a%d; }
void __qdivrem(u64 *q, u64 *r, u64 a, s16 b){ *q = a/(u64)b; *r = a%(u64)b; }
void __qdivrem(u64 *q, u64 *r, u64 a, s16 b){ *q = a/(u64)(s64)b; *r = a%(u64)(s64)b; }
void __qdivrem(u64 *q, u64 *r, u64 a, u16 b){ *q = a/b; *r = a%b; }
void __qdivrem(u64 *q, u64 *r, u64 a, s16 b){ s64 d = b; *q = a/d; *r = a%d; }
void __qdivrem(u64 *q, u64 *r, u64 a, s16 b){ *q = a/(s64)b; *r = a%(s64)b; }
void __qdivrem(u64 *q, u64 *r, u64 a, s16 b){ s32 d = b; *q = a/d; *r = a%d; }
void __qdivrem(u64 *q, u64 *r, u64 a, s16 b){ u32 d = b; *q = a/d; *r = a%d; }
EOF
