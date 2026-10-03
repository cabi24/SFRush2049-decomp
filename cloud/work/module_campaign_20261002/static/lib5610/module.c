/* Research only: complete original lib_5610 source closure. No coverage credit.
 * Recipe: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul.
 * Provenance and source adaptations: provenance.json and README.md.
 */
#include "rom_tu.h"
typedef struct InflateHuft InflateHuft;
struct InflateHuft { u8 e, b; union { u16 n; InflateHuft *t; } v; };
extern u8 *gInflateInPtr, *gInflateInEnd, *gInflateOutPtr;
extern u32 gInflateBitBuf, gInflateBitCount, gInflateSrc, gLzssSrc;
extern s32 gInflateToggle, gLzssToggle;
extern u8 gInflateBufferA[], gInflateBufferB[];
extern OSIoMesg gInflateDmaState, gLzssDmaState;
extern OSMesgQueue gInflateMsgQueue;
extern u16 gInflateCplens[], gInflateCplext[], gInflateCpdist[], gInflateCpdext[], gInflateMaskBits[];
extern u32 gInflateBorder[];
extern s32 gInflateHuftPointerLit, gInflateHuftPointerDist;
extern void osInvalDCache(void *, s32);
extern void osInvalICache_full(void *, s32);
extern void *sound_play_menu(s32, s32);
extern void dma_queue_sync(void *);
extern void *memset(void *, s32, u32);
extern void *memcpy(void *, const void *, u32);
u8 *inflate_io_wait(void);
s32 lzss_decode(u32, u8 *);
void inflate_flush_window(s32, s32);
s32 huft_alloc(s32);
s32 huft_build(u32 *, u32, u32, u16 *, u16 *, InflateHuft **, s32 *);
s32 inflate_free_window(InflateHuft *, InflateHuft *, s32, s32);
s32 inflate_stored(void), inflate_fixed(void), inflate_dynamic(void), inflate_block(s32 *), inflate_loop(void), inflate_read_bits(void);
s32 inflate_entry(u32, u8 *, s32), inflate_entry_alt(u8 *, s32, u8 * volatile);
#define BMAX 16
#define NEEDBITS(n) do {while(k<(n)) {u32 word; if(gInflateInPtr<gInflateInEnd) {gInflateInPtr+=2;word=(gInflateInPtr[-1]<<8)|gInflateInPtr[-2];} else word=inflate_read_bits();b|=word<<k;k+=16;}} while(0)
#define DUMPBITS(n) do {b>>=(n);k-=(n);} while(0)
#define C94_NEED(n) do { while (count < (n)) { u32 word; if (gInflateInPtr < gInflateInEnd) { gInflateInPtr += 2; word = (gInflateInPtr[-1] << 8) | gInflateInPtr[-2]; } else word = inflate_read_bits(); buffer |= word << count; count += 16; } } while (0)
#define C94_DROP(n) do { buffer >>= (n); count -= (n); } while (0)

/* Source member: cloud/work/static_C21/module.c */
u8 *inflate_io_wait(void) {
 OSMesg msg;
 u8 *fill,*next;
 while(osRecvMesg(&gInflateMsgQueue,&msg,OS_MESG_NOBLOCK)==-1) {}
 if(gLzssToggle==1) {fill=gInflateBufferB;next=gInflateBufferA;}
 else {fill=gInflateBufferA;next=gInflateBufferB;}
 osInvalDCache(fill,4096);
 __osPiRawStartDma(&gLzssDmaState,0,0,gLzssSrc,fill,4096,&gInflateMsgQueue);
 gLzssSrc+=4096;
 gLzssToggle^=1;
 return next;
}

/* Source member: cloud/work/static_C21/module.c */
s32 lzss_decode(u32 src,u8 *dst) {
 u8 *input=gInflateBufferA,*start=dst,*copy;
 s32 remaining=0,flags_left=0,length;
 u32 flags,offset,byte;
 OSMesg msg;
 gLzssToggle=1;
 osInvalDCache(input,4096);
 __osPiRawStartDma(&gLzssDmaState,0,0,src,input,4096,&gInflateMsgQueue);
 gLzssSrc=src+4096;
 for(;;) {
  if(flags_left==0) {
   if(remaining<=0) {input=inflate_io_wait();remaining=4096;}
   flags=*input++;remaining--;flags_left=8;
  }
  flags_left--;
  if(flags&1) {
   if(remaining<=0) {input=inflate_io_wait();remaining=4096;}
   byte=*input++;remaining--;*dst++=byte;
  } else {
   if(remaining<=0) {input=inflate_io_wait();remaining=4096;}
   byte=*input++;remaining--;offset=(byte&240)<<4;length=byte&15;
   if(remaining<=0) {input=inflate_io_wait();remaining=4096;}
   byte=*input++;remaining--;offset=(offset+byte)&4095;
   if(offset==0 && length==0) {
    while(osRecvMesg(&gInflateMsgQueue,&msg,OS_MESG_NOBLOCK)==-1) {}
    return dst-start;
   }
   length++;
   copy=dst-offset;
   do {*dst++=*copy++;} while(--length>=0);
  }
  flags>>=1;
 }
}

/* Source member: cloud/work/static_C21/module.c */
void inflate_flush_window(s32 arg0, s32 arg1) {
    gDisplayListHead = arg0;
    gDisplayListEnd = arg1;
    *(s32 *)&gDisplayListSize = 0;
}

/* Source member: cloud/work/static_C21/module.c */
s32 huft_alloc(s32 arg0)
{
  int new_var;
  gDisplayListSize += arg0;
  new_var = gDisplayListSize;
  new_var = (new_var - arg0) + gDisplayListHead;
  if (1)
  {
  }
  return new_var;
}

/* Source member: cloud/work/static_C23/huft_build.c */
s32 huft_build(u32 *b, u32 n, u32 s, u16 *d, u16 *e, InflateHuft **t, s32 *m)
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
	register InflateHuft *q; /* points to current table */
	InflateHuft r;           /* table entry for structure assignment */
	InflateHuft *u[BMAX];    /* table stack */
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
	u[0] = (InflateHuft *)NULL;   /* just to keep compilers happy */
	q = (InflateHuft *)NULL;      /* ditto */
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
				q = (InflateHuft*)huft_alloc((z+1)*sizeof(InflateHuft));
				if(q==NULL) return 3;         /* track memory usage */
				*t = q + 1;             /* link to list for huft_free() */
				*(t = &(q->v.t)) = (InflateHuft *)NULL;
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

/* Source member: cloud/work/static_C94/inflate_free_window.c */
s32 inflate_free_window(InflateHuft *literals, InflateHuft *distances, s32 literal_bits, s32 distance_bits) {
    register u32 extra, length;
    register InflateHuft *entry;
    u32 literal_mask, distance_mask;
    register u32 buffer, count;
    s32 displacement;
    literal_mask = gInflateMaskBits[literal_bits];
    distance_mask = gInflateMaskBits[distance_bits];
    buffer = gInflateBitBuf;
    count = gInflateBitCount;
    for (;;) {
        C94_NEED(literal_bits);
        entry = literals + (buffer & literal_mask);
        extra = entry->e;
        if (extra > 16) {
            do {
                if (extra == 99) return 1;
                C94_DROP(entry->b);
                extra -= 16;
                C94_NEED(extra);
                entry = entry->v.t + (buffer & gInflateMaskBits[extra]);
                extra = entry->e;
            } while (extra > 16);
        }
        C94_DROP(entry->b);
        if (extra == 16) {
            *gInflateOutPtr++ = entry->v.n;
        } else if (extra == 15) {
            break;
        } else {
            C94_NEED(extra);
            length = (buffer & gInflateMaskBits[extra]) + entry->v.n;
            C94_DROP(extra);
            C94_NEED(distance_bits);
            entry = distances + (buffer & distance_mask);
            extra = entry->e;
            if (extra > 16) {
                do {
                    if (extra == 99) return 1;
                    C94_DROP(entry->b);
                    extra -= 16;
                    C94_NEED(extra);
                    entry = entry->v.t + (buffer & gInflateMaskBits[extra]);
                    extra = entry->e;
                } while (extra > 16);
            }
            C94_DROP(entry->b);
            C94_NEED(extra);
            displacement = -entry->v.n - (buffer & gInflateMaskBits[extra]);
            C94_DROP(extra);
            if (displacement < -8) {
                memcpy(gInflateOutPtr, gInflateOutPtr + displacement, length);
                gInflateOutPtr += length;
            } else {
                while (length--) {
                    *gInflateOutPtr = gInflateOutPtr[displacement];
                    gInflateOutPtr++;
                }
            }
        }
    }
    gInflateBitBuf = buffer;
    gInflateBitCount = count;
    return 0;
}

/* Source member: cloud/work/static_C20/inflate_stored.c */
s32 inflate_stored(void) {
 u32 count=gInflateBitCount, bits=gInflateBitBuf, n,value;
 u32 alignment=count&7;
 bits>>=alignment;count-=alignment;
 while(count<16) {if(gInflateInPtr<gInflateInEnd) {gInflateInPtr+=2;value=(gInflateInPtr[-1]<<8)|gInflateInPtr[-2];} else value=inflate_read_bits();bits|=value<<count;count+=16;}
 n=bits&65535;bits>>=16;count-=16;
 while(count<16) {if(gInflateInPtr<gInflateInEnd) {gInflateInPtr+=2;value=(gInflateInPtr[-1]<<8)|gInflateInPtr[-2];} else value=inflate_read_bits();bits|=value<<count;count+=16;}
 if(n!=((~bits)&65535)) return 1;
 bits>>=16;count-=16;
 while(n--) {
  while(count<8) {if(gInflateInPtr<gInflateInEnd) {gInflateInPtr+=2;value=(gInflateInPtr[-1]<<8)|gInflateInPtr[-2];} else value=inflate_read_bits();bits|=value<<count;count+=16;}
  *gInflateOutPtr=(u8)bits;gInflateOutPtr++;
  bits>>=8;count-=8;
 }
 gInflateBitBuf=bits;gInflateBitCount=count;
 return 0;
}

/* Source member: cloud/work/static_C23/inflate_fixed.best.c */
s32 inflate_fixed(void) {
 s32 i;
 InflateHuft *tl,*td;
 s32 bl,bd;
 u32 *l=(u32*)huft_alloc(288*sizeof(u32));
 for(i=0;i<144;i++) l[i]=8;
 for(;i<256;i++) l[i]=9;
 for(;i<280;i++) l[i]=7;
 for(;i<288;i++) l[i]=8;
 bl=7;
 if((i=huft_build(l,288,257,gInflateCplens,gInflateCplext,&tl,&bl))!=0) {gDisplayListSize=0;return i;}
 for(i=0;i<30;i++) l[i]=5;
 bd=5;
 if((i=huft_build(l,30,0,gInflateCpdist,gInflateCpdext,&td,&bd))>1) {gDisplayListSize=0;return i;}
 if((i=inflate_free_window(tl,td,bl,bd))!=0) return i<0?i:1;
 gDisplayListSize=0;
 return 0;
}

/* Source member: cloud/work/static_C23/inflate_dynamic.c */
s32 inflate_dynamic(void)
{
	s32 i;                /* temporary variables */
	u32 j;
	u32 l;           /* last length */
	u32 m;           /* mask for bit lengths table */
	u32 n;           /* number of lengths to get */
	InflateHuft *tl;      /* literal/length code table */
	InflateHuft *td;      /* distance code table */
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
	NEEDBITS(5);
	nl = 257 + (b & 0x1f);      /* number of literal/length codes */
	DUMPBITS(5);
	NEEDBITS(5);
	nd = 1 + (b & 0x1f);        /* number of distance codes */
	DUMPBITS(5);
	NEEDBITS(4);
	nb = 4 + (b & 0xf);         /* number of bit length codes */
	DUMPBITS(4);
	if (nl>286 || nd>30) return 1;

	/* read in bit-length-code lengths */
	for (j = 0; j < nb; j++) {
		NEEDBITS(3);
		ll[gInflateBorder[j]] = b & 7;
		DUMPBITS(3);
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
		NEEDBITS(bl);
		j = (td = tl + (b & m))->b;
		DUMPBITS(j);
		j = td->v.n;

		if (j < 16) {                 /* length of code in bits (0..15) */
			ll[i++] = l = j;          /* save last length in l */
		} else if (j == 16) {         /* repeat last length 3 to 6 times */
			NEEDBITS(2);
			j = 3 + (b & 3);
			DUMPBITS(2);
			if (i+j>n) return 1;
			while (j--) {
				ll[i++] = l;
			}
		} else if (j == 17) {         /* 3 to 10 zero length codes */
			NEEDBITS(3);
			j = 3 + (b & 7);
			DUMPBITS(3);
			if (i+j>n) return 1;
			while (j--) {
				ll[i++] = 0;
			}

			l = 0;
		} else {                      /* j == 18: 11 to 138 zero length codes */
			NEEDBITS(7);
			j = 11 + (b & 0x7f);
			DUMPBITS(7);
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

/* Source member: cloud/work/static_C23/inflate_block.c */
s32 inflate_block(s32 *last) {
 u32 t;
 register u32 b=gInflateBitBuf,k=gInflateBitCount;
 NEEDBITS(1);*last=b&1;DUMPBITS(1);
 NEEDBITS(2);t=b&3;DUMPBITS(2);
 gInflateBitBuf=b;gInflateBitCount=k;
 if(t==2)return inflate_dynamic();
 if(t==0)return inflate_stored();
 if(t==1)return inflate_fixed();
 return 1;
}

/* Source member: cloud/work/static_C5/inflate_loop.c */
s32 inflate_loop(void) { s32 final; gInflateBitCount = 0; gInflateBitBuf = 0; do { if (inflate_block(&final) != 0) return 1; } while(!final); return 0; }

/* Source member: cloud/work/static_C20/inflate_read_bits.c */
s32 inflate_read_bits(void) {
 OSMesg msg;
 u8 *next,*ptr;u32 high;
 while(osRecvMesg(&gInflateMsgQueue,&msg,OS_MESG_NOBLOCK)==-1) {}
 if(gInflateToggle) {gInflateInPtr=gInflateBufferA;next=gInflateBufferB;gInflateToggle=0;}
 else {gInflateInPtr=gInflateBufferB;next=gInflateBufferA;gInflateToggle=1;}
 gInflateInEnd=gInflateInPtr+4096;
 gInflateSrc+=4096;
 osInvalDCache(gInflateInPtr,4096);
 __osPiRawStartDma(&gInflateDmaState,0,0,gInflateSrc,next,4096,&gInflateMsgQueue);
 gInflateInPtr+=2;
 ptr=gInflateInPtr;high=ptr[-1]<<8;return high|ptr[-2];
}

/* Source member: cloud/work/static_C99/inflate_entry.c */
s32 inflate_entry(u32 source, u8 *destination, s32 allocate_window)
{
    void *window;
    OSMesg message;
    gInflateOutPtr = destination;
    gInflateSrc = source;
    gInflateInPtr = gInflateBufferA;
    gInflateInEnd = gInflateInPtr;
    gInflateToggle = 1;
    osInvalDCache(gInflateInPtr,4096);
    __osPiRawStartDma(&gInflateDmaState,0,0,gInflateSrc,gInflateInPtr,4096,&gInflateMsgQueue);
    if (allocate_window) {
        window = sound_play_menu(0,12000);
        inflate_flush_window((s32)window,12000);
    } else inflate_flush_window((s32)0x803FD120,12000);
    inflate_loop();
    if (allocate_window) dma_queue_sync(window);
    while (osRecvMesg(&gInflateMsgQueue,&message,0) == -1) { }
    osInvalICache_full(destination,gInflateOutPtr-destination);
    return gInflateOutPtr-destination;
}

/* Source member: cloud/work/static_C20/inflate_entry_alt.c */
s32 inflate_entry_alt(u8 *src, s32 size, u8 * volatile dst) {
 void *window;
 gInflateOutPtr=dst;
 gInflateInPtr=src;
 gInflateInEnd=gInflateInPtr+size;
 window=sound_play_menu(0,12000);
 inflate_flush_window((s32)window,12000);
 inflate_loop();
 dma_queue_sync(window);
 return gInflateOutPtr-dst;
}
