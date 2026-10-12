/* GENERATED ROM-aligned TU — segment 0x5610 (rom/lib_5610)
 * layout map 5f28eb4dc56fe3d76b0707d5fd55277a08a82e5e5499a3e3974d597d0c80bee6; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_io_wait.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/lzss_decode.s")
/* PROMOTED 2026-10-01 — inflate_flush_window
 * Source:   cloud/work/static_C/inflate_flush_window.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C/inflate_flush_window.c:inflate_flush_window (score0);
 *           w26a: `gDisplayListSize = 0` (was `*(s32 *)&...`, a cast around rom_tu.h's
 *           former volatile), whole TU IDENTICAL under ido5.3s
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void inflate_flush_window(s32 arg0, s32 arg1) {
    gDisplayListHead = arg0;
    gDisplayListEnd = arg1;
    gDisplayListSize = 0;
}

/* PROMOTED 2026-09-24 — huft_alloc
 * Source:   work/auto/huft_alloc/matched.c (in-repo, locked); re-shaped 2026-10-12
 *           for the TU's ido5.3s recipe (w26a: cloud/work/frontier/w26a/sources/huft_alloc.c)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared, compiler ido5.3s (cc -S, no .loc, as1 -O1)
 * Evidence: lock:work/auto/huft_alloc/matched.c:huft_alloc (score0, standard -O2);
 *           w26a whole-TU IDENTICAL under ido5.3s (as1 -O1 keeps the final copy
 *           of a returned named local, so the sum is returned directly)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 huft_alloc(s32 arg0)
{
  int new_var;
  gDisplayListSize += arg0;
  new_var = gDisplayListSize;
  if (1)
  {
  }
  return (new_var - arg0) + gDisplayListHead;
}

/* PROMOTED 2026-10-12 — huft_build
 * Source:   cloud/work/frontier/w26a/sources/huft_build.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w26a/sources/huft_build.c:huft_build (score0); rom_tu.h context: cloud/work/frontier/w26a/ctx53s_b.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "types.h"
typedef struct huft { u8 e; u8 b; union { u16 n; struct huft *t; } v; } huft;
extern s32 huft_alloc(s32);
extern void *memset(void *, s32, u32);
#define BMAX 16
s32 huft_build(u32 *b, u32 n, u32 s, u16 *d, u16 *e, huft **t, s32 *m)
{
	u32 a;                   /* counter for codes of length k */
	u32 c[BMAX+1];           /* bit length count table */
	u32 f;                   /* i repeats in table every f entries */
	s32 g;                   /* maximum code length */
	s32 h;                   /* table level */
	register u32 i;          /* counter, current code */
	register u32 j;          /* counter */
	register s32 k;          /* number of bits in current code */
	s32 l;                   /* bits per table (returned in m) */
	register u32 *p;         /* pointer into c[], b[], or v[] */
	register huft *q; /* points to current table */
	huft r;           /* table entry for structure assignment */
	huft *u[BMAX];    /* table stack */
	u32 *v;            /* values in order of bit length */
	register s32 w;          /* bits before this table == (l * h) */
	u32 x[BMAX+1];           /* bit offsets, then code stack */
	u32 *xp;                 /* pointer into x */
	s32 y;                   /* number of dummy codes added */
	u32 z;                   /* number of entries in current table */
	

	/* Generate counts for each bit length */
	memset(c,0,sizeof(c));

	p = b;
	i = n;

	do {
		c[*p]++;                  /* assume all entries <= BMAX */
		p++;                      /* Can't combine with above line (Solaris bug) */
	} while (--i);

	if (c[0] == n) {              /* null input--all zero length codes */
		*t = NULL;
		*m = 0;
		return 0;
	}

	/* Find minimum and maximum length, bound *m by those */
	l = *m;

	for (j = 1; j <= BMAX; j++) {
		if (c[j]) {
			break;
		}
	}

	k = j;                        /* minimum code length */

	if (l < j) {
		l = j;
	}

	for (i = BMAX; i; i--) {
		if (c[i]) {
			break;
		}
	}

	g = i;                        /* maximum code length */

	if (l > i) {
		l = i;
	}

	*m = l;

	/* Adjust last length count to fill out codes, if needed */
	for (y = 1 << j; j < i; j++, y <<= 1) {
		if ((y -= c[j]) < 0) return 2;
	}

	if ((y -= c[i]) < 0) return 2;
	c[i] += y;

	/* Generate starting offsets into the value table for each length */
	x[1] = j = 0;
	p = c + 1;
	xp = x + 2;

	while (--i) {                 /* note that i == g from above */
		*xp++ = (j += *p++);
	}

	/* Make a table of values in order of bit lengths */
	v=(u32*)huft_alloc(288*sizeof(u32));
	p = b;
	i = 0;

	do {
		if ((j = *p++) != 0) {
			v[x[j]++] = i;
		}
	} while (++i < n);

	/* Generate the Huffman codes and for each, make the table entries */
	x[0] = i = 0;                 /* first Huffman code is zero */
	p = v;                        /* grab values in bit order */
	h = -1;                       /* no tables yet--level -1 */
	w = -l;                       /* bits decoded == (l * h) */
	u[0] = (huft *)NULL;   /* just to keep compilers happy */
	q = (huft *)NULL;      /* ditto */
	z = 0;                        /* ditto */

	/* go through the bit lengths (k already is bits in shortest code) */
	for (; k <= g; k++) {
		a = c[k];

		while (a--) {
			/* here i is the Huffman code of length k bits for value *p */
			/* make tables up to required level */
			while (k > w + l) {
				h++;
				w += l;                 /* previous table always l bits */

				/* compute minimum size table less than or equal to l bits */
				z = (z = g - w) > l ? l : z;  /* upper limit on table size */

				if ((f = 1 << (j = k - w)) > a + 1) {   /* try a k-w bit table */
					/* too few codes for k-w bit table */
					/* deduct codes from patterns left */
					f -= a + 1;
					xp = c + k;

					while (++j < z) {     /* try smaller tables up to z bits */
						if ((f <<= 1) <= *++xp) {
							break;            /* enough codes to use up j bits */
						}

						f -= *xp;           /* else deduct codes from patterns */
					}
				}

				z = 1 << j;             /* table entries for j-bit table */

				/* allocate and link in new table */
				q = (huft*)huft_alloc((z+1)*sizeof(huft));
				if(q==NULL) return 3;         /* track memory usage */
				*t = q + 1;             /* link to list for huft_free() */
				*(t = &(q->v.t)) = (huft *)NULL;
				u[h] = ++q;             /* table starts after link */

				/* connect to last table, if there is one */
				if (h) {
					x[h] = i;             /* save pattern for backing up */
					r.b = l;              /* bits to dump before this table */
					r.e = 16 + j;         /* bits in this table */
					r.v.t = q;            /* pointer to this table */
					j = i >> (w - l);     /* (get around Turbo C bug) */
					u[h-1][j] = r;        /* connect to last table */
				}
			}

			/* set up table entry in r */
			r.b = (k - w);

			if (p >= v + n) {
				r.e = 99;               /* out of values--invalid code */
			} else if (*p < s) {
				r.e = (*p < 256 ? 16 : 15);    /* 256 is end-of-block code */
				r.v.n = *p;             /* simple code is just the value */
				p++;                    /* one compiler does not like *p++ */
			} else {
				r.e = e[*p - s];   /* non-simple--look up in lists */
				r.v.n = d[*p++ - s];
			}

			/* fill code-like entries with r */
			f = 1 << (k - w);

			for (j = i >> w; j < z; j += f) {
				q[j] = r;
			}

			/* backwards increment the k-bit code i */
			for (j = 1 << (k - 1); i & j; j >>= 1) {
				i ^= j;
			}

			i ^= j;

			/* backup over finished tables */
			while ((i & ((1 << w) - 1)) != x[h]) {
				h--;                    /* don't need to update q */
				w -= l;
			}
		}
	}

	/* Return true (1) if we were given an incomplete table */
	return y != 0 && g != 1;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_free_window.s")
/* PROMOTED 2026-10-12 — inflate_stored
 * Source:   cloud/work/frontier/w26a/sources/inflate_stored.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w26a/sources/inflate_stored.c:inflate_stored (score0); rom_tu.h context: cloud/work/frontier/w26a/ctx53s_b.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern u32 gInflateBitBuf, gInflateBitCount;
extern u8 *gInflateInPtr, *gInflateInEnd, *gInflateOutPtr;
extern s32 inflate_read_bits(void);
#define NEEDBITS(n) do { while (k < (n)) { b |= (u32)(gInflateInPtr < gInflateInEnd ? (gInflateInPtr += 2, gInflateInPtr[-2] | (gInflateInPtr[-1] << 8)) : inflate_read_bits()) << k; k += 16; } } while (0)
#define DUMPBITS(n) do { b >>= (n); k -= (n); } while (0)
s32 inflate_stored(void) {
    u32 n;
    register u32 b = gInflateBitBuf, k = gInflateBitCount;

    DUMPBITS(k & 7);
    NEEDBITS(16);
    n = b & 0xffff;
    DUMPBITS(16);
    NEEDBITS(16);
    if (n != ((~b) & 0xffff)) return 1;
    DUMPBITS(16);
    while (n--) {
        NEEDBITS(8);
        *gInflateOutPtr++ = (u8)b;
        DUMPBITS(8);
    }
    gInflateBitBuf = b;
    gInflateBitCount = k;
    return 0;
}

/* PROMOTED 2026-10-12 — inflate_fixed
 * Source:   cloud/work/frontier/w26a/sources/inflate_fixed.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w26a/sources/inflate_fixed.c:inflate_fixed (score0); rom_tu.h context: cloud/work/frontier/w26a/ctx53s_b.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern u16 gInflateCplens[], gInflateCplext[], gInflateCpdist[], gInflateCpdext[];
extern unsigned int gDisplayListSize;
extern s32 huft_build(u32 *, u32, u32, u16 *, u16 *, huft **, s32 *);
extern s32 inflate_free_window(huft *, huft *, s32, s32);
s32 inflate_fixed(void) {
    s32 i;
    huft *tl, *td;
    s32 bl, bd;
    u32 *l = (u32 *)huft_alloc(288 * sizeof(u32));

    for (i = 0; i < 144; i++) l[i] = 8;
    for (; i < 256; i++) l[i] = 9;
    for (; i < 280; i++) l[i] = 7;
    for (; i < 288; i++) l[i] = 8;
    bl = 7;
    if ((i = huft_build(l, 288, 257, gInflateCplens, gInflateCplext, &tl, &bl)) != 0) {
        gDisplayListSize = 0;
        return i;
    }
    for (i = 0; i < 30; i++) l[i] = 5;
    bd = 5;
    if ((i = huft_build(l, 30, 0, gInflateCpdist, gInflateCpdext, &td, &bd)) > 1) {
        gDisplayListSize = 0;
        return i;
    }
    if ((i = inflate_free_window(tl, td, bl, bd)) != 0) return i < 0 ? i : 1;
    gDisplayListSize = 0;
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_dynamic.s")
/* PROMOTED 2026-10-12 — inflate_block
 * Source:   cloud/work/frontier/w26a/sources/inflate_block.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w26a/sources/inflate_block.c:inflate_block (score0); rom_tu.h context: cloud/work/frontier/w26a/ctx53s.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "types.h"
extern u32 gInflateBitBuf, gInflateBitCount;
extern u8 *gInflateInPtr, *gInflateInEnd;
extern s32 inflate_read_bits(void), inflate_dynamic(void), inflate_stored(void), inflate_fixed(void);
#define NEEDBITS(n) do { while (k < (n)) { b |= (u32)(gInflateInPtr < gInflateInEnd ? (gInflateInPtr += 2, gInflateInPtr[-2] | (gInflateInPtr[-1] << 8)) : inflate_read_bits()) << k; k += 16; } } while (0)
#define DUMPBITS(n) do { b >>= (n); k -= (n); } while (0)
s32 inflate_block(s32 *last) {
    u32 t;
    register u32 b = gInflateBitBuf, k = gInflateBitCount;

    NEEDBITS(1);
    *last = b & 1;
    DUMPBITS(1);
    NEEDBITS(2);
    t = b & 3;
    DUMPBITS(2);
    gInflateBitBuf = b;
    gInflateBitCount = k;
    if (t == 2) return inflate_dynamic();
    if (t == 0) return inflate_stored();
    if (t == 1) return inflate_fixed();
    return 1;
}

/* PROMOTED 2026-10-12 — inflate_loop
 * Source:   cloud/work/frontier/w26a/sources/inflate_loop.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w26a/sources/inflate_loop.c:inflate_loop (score0); rom_tu.h context: cloud/work/frontier/w26a/ctx53s.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
extern s32 inflate_block(s32 *last);
s32 inflate_loop(void) {
    s32 e;

    gInflateBitCount = 0;
    gInflateBitBuf = 0;
    do {
        if (inflate_block(&e) != 0) {
            return 1;
        }
    } while (!e);
    return 0;
}

/* PROMOTED 2026-10-12 — inflate_read_bits
 * Source:   cloud/work/frontier/w26a/sources/inflate_read_bits.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w26a/sources/inflate_read_bits.c:inflate_read_bits (score0); rom_tu.h context: cloud/work/frontier/w26a/ctx53s.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#include "PR/os_message.h"
#include "PR/os_pi.h"
extern u32 gInflateSrc;
extern s32 gInflateToggle;
extern u8 gInflateBufferA[], gInflateBufferB[];
extern OSIoMesg gInflateDmaState;
extern OSMesgQueue gInflateMsgQueue;
extern void osInvalDCache(void *, s32);
extern s32 osPiStartDma(OSIoMesg *, s32, s32, u32, void *, u32, OSMesgQueue *);
s32 inflate_read_bits(void) {
    OSMesg msg;
    u8 *next;

    while (osRecvMesg(&gInflateMsgQueue, &msg, OS_MESG_NOBLOCK) == -1) {
    }
    if (gInflateToggle) {
        gInflateInPtr = gInflateBufferA;
        next = gInflateBufferB;
        gInflateToggle = 0;
    } else {
        gInflateInPtr = gInflateBufferB;
        gInflateToggle = 1;
        next = gInflateBufferA;
    }
    gInflateInEnd = gInflateInPtr + 4096;
    gInflateSrc += 4096;
    osInvalDCache(gInflateInPtr, 4096);
    osPiStartDma(&gInflateDmaState, 0, 0, gInflateSrc, next, 4096, &gInflateMsgQueue);
    gInflateInPtr += 2;
    return gInflateInPtr[-2] | (gInflateInPtr[-1] << 8);
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_entry.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_5610/inflate_entry_alt.s")
