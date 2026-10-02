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
extern u32 gInflateBorder[];
extern u16 gInflateMaskBits[];
extern s32 gInflateHuftPointerLit,gInflateHuftPointerDist;
#define NEEDBITS(n) do {while(k<(n)) {u32 word; if(gInflateInPtr<gInflateInEnd) {gInflateInPtr+=2;word=(gInflateInPtr[-1]<<8)|gInflateInPtr[-2];} else word=inflate_read_bits();b|=word<<k;k+=16;}} while(0);
#define DUMPBITS(n) do {b>>=(n);k-=(n);} while(0);
s32 inflate_dynamic(void)
{
	s32 i;                /* temporary variables */
	u32 j;
	u32 l;           /* last length */
	u32 m;           /* mask for bit lengths table */
	u32 n;           /* number of lengths to get */
	C23Huft *tl;      /* literal/length code table */
	C23Huft *td;      /* distance code table */
	s32 bl;               /* lookup bits for tl */
	s32 bd;               /* lookup bits for td */
	u32 nb;          /* number of bit length codes */
	u32 nl;          /* number of literal/length codes */
	u32 nd;          /* number of distance codes */
	register u32 k;  /* number of bits in bit buffer */
	register u32 b;  /* bit buffer */
	u32 *ll=(u32*)huft_alloc((286+30)*sizeof(u32));  /* literal/length and distance code lengths */

	/* make local bit buffer */
	k = gInflateBitCount;
	b = gInflateBitBuf;

	/* read in table lengths */
	NEEDBITS(5)
	nl = 257 + (b & 0x1f);      /* number of literal/length codes */
	DUMPBITS(5)
	NEEDBITS(5)
	nd = 1 + (b & 0x1f);        /* number of distance codes */
	DUMPBITS(5)
	NEEDBITS(4)
	nb = 4 + (b & 0xf);         /* number of bit length codes */
	DUMPBITS(4)
	if (nl>286 || nd>30) return 1;

	/* read in bit-length-code lengths */
	for (j = 0; j < nb; j++) {
		NEEDBITS(3)
		ll[gInflateBorder[j]] = b & 7;
		DUMPBITS(3)
	}

	for (; j < 19; j++) {
		ll[gInflateBorder[j]] = 0;
	}

	/* build decoding table for trees--single level, 7 bit lookup */
	bl = 7;

	if ((i=huft_build(ll, 19, 19, NULL, NULL, &tl, &bl))!=0) {gDisplayListSize=0;return i;}

	/* read in literal and distance code lengths */
	n = nl + nd;
	m = gInflateMaskBits[bl];
	i = l = 0;

	while (i < n) {
		NEEDBITS(bl)
		j = (td = tl + (b & m))->b;
		DUMPBITS(j)

		j = td->v.n;

		if (j < 16) {                 /* length of code in bits (0..15) */
			ll[i++] = l = j;          /* save last length in l */
		} else if (j == 16) {         /* repeat last length 3 to 6 times */
			NEEDBITS(2)
			j = 3 + (b & 3);
			DUMPBITS(2)

			if (i+j>n) return 1;
			while (j--) {
				ll[i++] = l;
			}
		} else if (j == 17) {         /* 3 to 10 zero length codes */
			NEEDBITS(3)
			j = 3 + (b & 7);
			DUMPBITS(3)

			if (i+j>n) return 1;
			while (j--) {
				ll[i++] = 0;
			}

			l = 0;
		} else {                      /* j == 18: 11 to 138 zero length codes */
			NEEDBITS(7)
			j = 11 + (b & 0x7f);
			DUMPBITS(7)

			if (i+j>n) return 1;
			while (j--) {
				ll[i++] = 0;
			}

			l = 0;
		}
	}

	/* restore the global bit buffer */
	gDisplayListSize=0x4F0;
	gInflateBitBuf=b;
	gInflateBitCount=k;

	/* build the decoding tables for literal/length and distance codes */
	bl = gInflateHuftPointerLit;

	if ((i=huft_build(ll, nl, 257, gInflateCplens, gInflateCplext, &tl, &bl))!=0) {gDisplayListSize=0;return i;}

	bd = gInflateHuftPointerDist;

	if ((i=huft_build(ll + nl, nd, 0, gInflateCpdist, gInflateCpdext, &td, &bd))!=0) {gDisplayListSize=0;return i;}

	/* decompress until an end-of-block code */
	if ((i=inflate_free_window(tl, td, bl, bd))!=0) return i<0?i:1;
	gDisplayListSize=0;

	return 0;
}

