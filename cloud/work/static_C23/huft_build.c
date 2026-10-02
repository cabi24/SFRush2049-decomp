/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
typedef struct C23Huft C23Huft;
struct C23Huft {u8 e,b;union {u16 n;C23Huft *t;} v;};
extern u16 gInflateCplens[],gInflateCplext[],gInflateCpdist[],gInflateCpdext[];
extern s32 huft_alloc(s32);
extern s32 huft_build(u32*,u32,u32,u16*,u16*,C23Huft**,s32*);
extern s32 inflate_free_window(C23Huft*,C23Huft*,s32,s32);
extern u32 gInflateBitBuf,gInflateBitCount;
extern u8 *gInflateInPtr,*gInflateInEnd;
extern s32 inflate_read_bits(void),inflate_dynamic(void),inflate_stored(void),inflate_fixed(void);
#define BMAX 16
extern void *memset(void*,s32,u32);
s32 huft_build(u32 *b, u32 n, u32 s, u16 *d, u16 *e, C23Huft **t, s32 *m)
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
	register C23Huft *q; /* points to current table */
	C23Huft r;           /* table entry for structure assignment */
	C23Huft *u[BMAX];    /* table stack */
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
	u[0] = (C23Huft *)NULL;   /* just to keep compilers happy */
	q = (C23Huft *)NULL;      /* ditto */
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
				q = (C23Huft*)huft_alloc((z+1)*sizeof(C23Huft));
				if(q==NULL) return 3;         /* track memory usage */
				*t = q + 1;             /* link to list for huft_free() */
				*(t = &(q->v.t)) = (C23Huft *)NULL;
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

