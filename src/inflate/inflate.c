/**
 * @file inflate.c
 * @brief DEFLATE decompression (inflate) implementation
 *
 * Decompiled from asm/us/5610.s
 * Based on standard DEFLATE inflate algorithm (RFC 1951)
 *
 * Key differences from standard implementations:
 * - Uses 4KB double-buffered sliding window
 * - Async I/O via message queues for ROM access
 * - Optimized for N64 cartridge latency
 */

#include "inflate/inflate.h"
#include "PR/os_message.h"

/* External OS functions */
extern void bzero(void *ptr, u32 size);
extern void *memcpy(void *dst, const void *src, u32 size);
extern s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
extern void osInvalDCache(void *addr, u32 size);

/* External async I/O functions */
extern void dma_read_async(void *state, s32 a1, s32 a2, void *src, void *dst, u32 size, void *mq);
extern void dma_finalize(void *dst, u32 size);
extern void *heap_alloc(s32 unused, u32 size);
extern void heap_free(void *ptr);

/* Global inflate state */
static u8 *g_inptr;           /* Current input position (D_800354B0) */
static u8 *g_inend;           /* Input buffer end (D_800354B4) */
static u8 *g_outptr;          /* Output write position (D_800354B8) */
static u32 g_bitbuf;          /* Bit accumulator (D_800354BC) */
static u32 g_bitcount;        /* Valid bits in buffer (D_800354C0) */
static void *g_huft_base;     /* Huffman table base (D_800354C4) */
static u32 g_huft_offset;     /* Huffman table offset (D_800354C8) */
static u32 g_huft_size;       /* Huffman table size (D_800354CC) */
static void *g_src;           /* Compressed source (D_800354D0) */
static u32 g_buffer_toggle;   /* Double-buffer toggle (D_800354D4) */

/* I/O state */
extern void *g_dma_state;     /* DMA state structure (D_800354D8) */
extern OSMesgQueue g_mq;      /* Message queue (D_80035428) */

/* Double buffers (4KB each) */
extern u8 g_buffer_a[INFLATE_WINDOW_SIZE];  /* D_80084A50 */
extern u8 g_buffer_b[INFLATE_WINDOW_SIZE];  /* D_80085A50 */

/* Pre-built fixed Huffman table in ROM */
extern struct huft g_fixed_huft_table[];    /* D_803FD120 */

/* DEFLATE static tables */
static const u8 border[] = {
    16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15
};

static const u16 cplens[] = {
    3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
    35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0
};

static const u8 cplext[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
    3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 99, 99
};

static const u16 cpdist[] = {
    1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193,
    257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
    8193, 12289, 16385, 24577
};

static const u8 cpdext[] = {
    0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
    7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13
};

static const u16 mask_bits[] = {
    0x0000,
    0x0001, 0x0003, 0x0007, 0x000f, 0x001f, 0x003f, 0x007f, 0x00ff,
    0x01ff, 0x03ff, 0x07ff, 0x0fff, 0x1fff, 0x3fff, 0x7fff, 0xffff
};

/**
 * Initialize Huffman table allocator
 */
static void huft_init(void *base, u32 size) {
    g_huft_base = base;
    g_huft_offset = 0;
    g_huft_size = size;
}

/**
 * Allocate memory from Huffman table pool
 */
static void *huft_alloc(u32 size) {
    void *ptr;

    if (g_huft_offset + size > g_huft_size) {
        return NULL;
    }

    ptr = (u8 *)g_huft_base + g_huft_offset;
    g_huft_offset += size;
    return ptr;
}

/**
 * Refill input buffer (async I/O with double-buffering)
 */
static void refill_buffer(void) {
    OSMesg msg;
    u8 *buffer;

    /* Wait for any pending I/O */
    while (osRecvMesg(&g_mq, &msg, OS_MESG_NOBLOCK) == -1) {
        /* Spin */
    }

    /* Select buffer based on toggle */
    if (g_buffer_toggle == 1) {
        buffer = g_buffer_b;
        g_buffer_toggle = 0;
    } else {
        buffer = g_buffer_a;
        g_buffer_toggle = 1;
    }

    /* Clear buffer and start async read */
    osInvalDCache(buffer, INFLATE_WINDOW_SIZE);
    dma_read_async(&g_dma_state, 0, 0, g_src, buffer, INFLATE_WINDOW_SIZE, &g_mq);

    /* Update source pointer */
    g_src = (u8 *)g_src + INFLATE_WINDOW_SIZE;

    /* Set up input pointers */
    g_inptr = buffer;
    g_inend = buffer + INFLATE_WINDOW_SIZE;
}

/**
 * Read next byte from input, refilling buffer if needed
 */
static u32 read_byte(void) {
    if (g_inptr >= g_inend) {
        refill_buffer();
    }
    return *g_inptr++;
}

/**
 * Ensure we have at least n bits in the bit buffer
 */
static void need_bits(u32 n) {
    while (g_bitcount < n) {
        g_bitbuf |= read_byte() << g_bitcount;
        g_bitcount += 8;
    }
}

/**
 * Consume n bits from the bit buffer
 */
static void dump_bits(u32 n) {
    g_bitbuf >>= n;
    g_bitcount -= n;
}

/**
 * Build Huffman decoding tables from code lengths
 * (huft_build - func_80004D98)
 */
s32 huft_build(u32 *b, u32 n, u32 s, u16 *d, u8 *e, struct huft **t, s32 *m) {
    u32 a;
    u32 c[BMAX + 1];
    u32 f;
    s32 g;
    s32 h;
    u32 i, j;
    s32 k;
    s32 l;
    u32 *p;
    struct huft *q;
    struct huft r;
    struct huft *u[BMAX];
    u32 v[N_MAX];
    s32 w;
    u32 x[BMAX + 1];
    u32 *xp;
    s32 y;
    u32 z;

    /* Clear bit length count table */
    bzero(c, sizeof(c));

    /* Count codes of each length */
    p = b;
    i = n;
    do {
        c[*p]++;
        p++;
    } while (--i);

    /* All zero length codes - null input */
    if (c[0] == n) {
        *t = NULL;
        *m = 0;
        return 0;
    }

    /* Find min/max code lengths */
    l = *m;
    for (j = 1; j <= BMAX; j++) {
        if (c[j]) break;
    }
    k = j;
    if (l < (s32)j) l = j;

    for (i = BMAX; i; i--) {
        if (c[i]) break;
    }
    g = i;
    if (l > (s32)i) l = i;
    *m = l;

    /* Adjust last length count */
    for (y = 1 << j; j < (s32)i; j++, y <<= 1) {
        if ((y -= c[j]) < 0) return 2;
    }
    if ((y -= c[i]) < 0) return 2;
    c[i] += y;

    /* Generate starting offsets */
    x[1] = j = 0;
    p = c + 1;
    xp = x + 2;
    while (--i) {
        *xp++ = (j += *p++);
    }

    /* Make table of values in order of bit lengths */
    p = b;
    i = 0;
    do {
        if ((j = *p++) != 0) {
            v[x[j]++] = i;
        }
    } while (++i < n);

    /* Generate Huffman codes and build tables */
    x[0] = i = 0;
    p = v;
    h = -1;
    w = -l;
    u[0] = NULL;
    q = NULL;
    z = 0;

    for (; k <= g; k++) {
        a = c[k];
        while (a--) {
            while (k > w + l) {
                h++;
                w += l;

                z = g - w;
                z = (z > (u32)l) ? l : z;

                if ((f = 1 << (j = k - w)) > a + 1) {
                    f -= a + 1;
                    xp = c + k;
                    while (++j < z) {
                        if ((f <<= 1) <= *++xp) break;
                        f -= *xp;
                    }
                }

                z = 1 << j;

                q = huft_alloc((z + 1) * sizeof(struct huft));
                if (q == NULL) return 3;

                *t = q + 1;
                t = &(q->v.t);
                u[h] = ++q;

                if (h) {
                    x[h] = i;
                    r.b = (u8)l;
                    r.e = (u8)(16 + j);
                    r.v.t = q;
                    j = i >> (w - l);
                    u[h-1][j] = r;
                }
            }

            r.b = (u8)(k - w);
            if (p >= v + n) {
                r.e = 99;
            } else if (*p < s) {
                r.e = (u8)(*p < 256 ? 16 : 15);
                r.v.n = (u16)*p++;
            } else {
                r.e = e[*p - s];
                r.v.n = d[*p++ - s];
            }

            f = 1 << (k - w);
            for (j = i >> w; j < z; j += f) {
                q[j] = r;
            }

            for (j = 1 << (k - 1); i & j; j >>= 1) {
                i ^= j;
            }
            i ^= j;

            while ((i & ((1 << w) - 1)) != x[h]) {
                h--;
                w -= l;
            }
        }
    }

    return y != 0 && g != 1;
}

/* Forward declarations */
static s32 inflate_block(s32 *e);
static s32 inflate_stored(void);
static s32 inflate_fixed(void);
static s32 inflate_dynamic(void);
static s32 inflate_codes(struct huft *tl, struct huft *td, s32 bl, s32 bd);

/**
 * Read next 16-bit word from input with buffer management
 * (func_800066D4 - read_word helper)
 */
static u32 read_word(void) {
    OSMesg msg;
    u8 *buffer;
    u32 lo, hi;

    /* Wait for any pending I/O */
    while (osRecvMesg(&g_mq, &msg, OS_MESG_NOBLOCK) == -1) {
        /* Spin */
    }

    /* Select next buffer based on toggle */
    if (g_buffer_toggle != 0) {
        buffer = g_buffer_a;
        g_buffer_toggle = 0;
    } else {
        buffer = g_buffer_b;
        g_buffer_toggle = 1;
    }

    /* Set up new buffer bounds */
    g_inptr = buffer;
    g_inend = buffer + INFLATE_WINDOW_SIZE;

    /* Invalidate cache and start async read for next buffer */
    osInvalDCache(buffer == g_buffer_a ? g_buffer_b : g_buffer_a, INFLATE_WINDOW_SIZE);
    dma_read_async(&g_dma_state, 0, 0, g_src,
                   buffer == g_buffer_a ? g_buffer_b : g_buffer_a,
                   INFLATE_WINDOW_SIZE, &g_mq);
    g_src = (u8 *)g_src + INFLATE_WINDOW_SIZE;

    /* Read 16-bit word (little-endian) */
    g_inptr += 2;
    lo = g_inptr[-2];
    hi = g_inptr[-1];

    return (hi << 8) | lo;
}

/**
 * Inflate codes using literal/length and distance Huffman tables
 * (inflate_codes - decode compressed data)
 */
static s32 inflate_codes(struct huft *tl, struct huft *td, s32 bl, s32 bd) {
    u32 e;              /* table entry flag/extra bits */
    u32 n, d;           /* match length, distance */
    struct huft *t;     /* pointer to table entry */
    u32 ml, md;         /* masks for bl and bd bits */
    u8 *src;

    ml = mask_bits[bl];
    md = mask_bits[bd];

    for (;;) {
        need_bits(bl);
        t = tl + (g_bitbuf & ml);
        e = t->e;

        if (e > 16) {
            do {
                if (e == 99) {
                    return 1;
                }
                dump_bits(t->b);
                e -= 16;
                need_bits(e);
                t = t->v.t + (g_bitbuf & mask_bits[e]);
                e = t->e;
            } while (e > 16);
        }

        dump_bits(t->b);

        if (e == 16) {
            /* Literal */
            *g_outptr++ = (u8)t->v.n;
        } else if (e == 15) {
            /* End of block */
            break;
        } else {
            /* Length/distance pair */
            need_bits(e);
            n = t->v.n + (g_bitbuf & mask_bits[e]);
            dump_bits(e);

            need_bits(bd);
            t = td + (g_bitbuf & md);
            e = t->e;

            if (e > 16) {
                do {
                    if (e == 99) {
                        return 1;
                    }
                    dump_bits(t->b);
                    e -= 16;
                    need_bits(e);
                    t = t->v.t + (g_bitbuf & mask_bits[e]);
                    e = t->e;
                } while (e > 16);
            }

            dump_bits(t->b);
            need_bits(e);
            d = t->v.n + (g_bitbuf & mask_bits[e]);
            dump_bits(e);

            /* Copy from sliding window */
            src = g_outptr - d;
            do {
                *g_outptr++ = *src++;
            } while (--n);
        }
    }

    return 0;
}

/**
 * Handle stored (uncompressed) block - type 0
 * (func_8000595C)
 */
static s32 inflate_stored(void) {
    u32 n;      /* number of bytes in block */
    u32 nlen;   /* one's complement of n */

    /* Discard remaining bits in bit buffer to byte boundary */
    dump_bits(g_bitcount & 7);

    /* Read block length and its complement */
    need_bits(16);
    n = g_bitbuf & 0xFFFF;
    dump_bits(16);

    need_bits(16);
    nlen = g_bitbuf & 0xFFFF;
    dump_bits(16);

    /* Check for complement match */
    if (n != (~nlen & 0xFFFF)) {
        return 1;
    }

    /* Copy n bytes from input to output */
    while (n--) {
        need_bits(8);
        *g_outptr++ = (u8)(g_bitbuf & 0xFF);
        dump_bits(8);
    }

    return 0;
}

/**
 * Handle fixed Huffman block - type 1
 * (func_80005B7C)
 */
static s32 inflate_fixed(void) {
    s32 i;
    struct huft *tl, *td;
    s32 bl, bd;
    u32 l[288];
    s32 result;

    /* Set up literal length table (fixed) */
    for (i = 0; i < 144; i++) {
        l[i] = 8;
    }
    for (; i < 256; i++) {
        l[i] = 9;
    }
    for (; i < 280; i++) {
        l[i] = 7;
    }
    for (; i < 288; i++) {
        l[i] = 8;
    }

    bl = 7;
    if ((result = huft_build(l, 288, 257, (u16 *)cplens, (u8 *)cplext, &tl, &bl)) != 0) {
        return result;
    }

    /* Set up distance table (fixed) */
    for (i = 0; i < 30; i++) {
        l[i] = 5;
    }

    bd = 5;
    if ((result = huft_build(l, 30, 0, (u16 *)cpdist, (u8 *)cpdext, &td, &bd)) > 1) {
        return result;
    }

    /* Decompress data */
    result = inflate_codes(tl, td, bl, bd);

    return result;
}

/**
 * Handle dynamic Huffman block - type 2
 * (func_80005D44)
 */
static s32 inflate_dynamic(void) {
    s32 i, j;
    u32 l;              /* last length */
    u32 m;              /* mask for bit lengths */
    u32 n;              /* number of lengths to get */
    struct huft *tl, *td;   /* literal/length and distance trees */
    s32 bl, bd;         /* bit lengths for tl and td */
    u32 nb;             /* number of bit length codes */
    u32 nl;             /* number of literal/length codes */
    u32 nd;             /* number of distance codes */
    u32 ll[286 + 30];   /* code lengths */
    s32 result;

    /* Read number of literal/length codes, distance codes, bit length codes */
    need_bits(5);
    nl = 257 + (g_bitbuf & 0x1F);
    dump_bits(5);

    need_bits(5);
    nd = 1 + (g_bitbuf & 0x1F);
    dump_bits(5);

    need_bits(4);
    nb = 4 + (g_bitbuf & 0xF);
    dump_bits(4);

    if (nl > 286 || nd > 30) {
        return 1;
    }

    /* Read code lengths for code length alphabet */
    for (j = 0; j < (s32)nb; j++) {
        need_bits(3);
        ll[border[j]] = g_bitbuf & 0x7;
        dump_bits(3);
    }
    for (; j < 19; j++) {
        ll[border[j]] = 0;
    }

    /* Build code length tree */
    bl = 7;
    if ((result = huft_build(ll, 19, 19, NULL, NULL, &tl, &bl)) != 0) {
        return result;
    }

    /* Read literal/length and distance code lengths */
    n = nl + nd;
    m = mask_bits[bl];
    i = l = 0;

    while ((u32)i < n) {
        need_bits(bl);
        j = (tl + (g_bitbuf & m))->b;
        dump_bits(j);
        j = (tl + (g_bitbuf & m))->v.n;

        if (j < 16) {
            ll[i++] = l = j;
        } else if (j == 16) {
            need_bits(2);
            j = 3 + (g_bitbuf & 0x3);
            dump_bits(2);
            if ((u32)i + j > n) {
                return 1;
            }
            while (j--) {
                ll[i++] = l;
            }
        } else if (j == 17) {
            need_bits(3);
            j = 3 + (g_bitbuf & 0x7);
            dump_bits(3);
            if ((u32)i + j > n) {
                return 1;
            }
            while (j--) {
                ll[i++] = 0;
            }
            l = 0;
        } else {
            need_bits(7);
            j = 11 + (g_bitbuf & 0x7F);
            dump_bits(7);
            if ((u32)i + j > n) {
                return 1;
            }
            while (j--) {
                ll[i++] = 0;
            }
            l = 0;
        }
    }

    /* Build literal/length tree */
    bl = 9;
    if ((result = huft_build(ll, nl, 257, (u16 *)cplens, (u8 *)cplext, &tl, &bl)) != 0) {
        return result;
    }

    /* Build distance tree */
    bd = 6;
    if ((result = huft_build(ll + nl, nd, 0, (u16 *)cpdist, (u8 *)cpdext, &td, &bd)) != 0) {
        return result;
    }

    /* Decompress data */
    result = inflate_codes(tl, td, bl, bd);

    return result;
}

/**
 * Decompress one DEFLATE block
 * (func_800064D4)
 *
 * @param e Output: 1 if this is the last block, 0 otherwise
 * @return 0 on success, non-zero on error
 */
static s32 inflate_block(s32 *e) {
    u32 t;              /* block type */

    /* Read block header */
    need_bits(1);
    *e = g_bitbuf & 1;
    dump_bits(1);

    need_bits(2);
    t = g_bitbuf & 3;
    dump_bits(2);

    /* Dispatch based on block type */
    if (t == 2) {
        return inflate_dynamic();
    } else if (t == 0) {
        return inflate_stored();
    } else if (t == 1) {
        return inflate_fixed();
    }

    /* Invalid block type */
    return 1;
}

/**
 * Main inflate loop - process all blocks
 * (func_80006678)
 *
 * @return 0 on success, non-zero on error
 */
static s32 inflate_loop(void) {
    s32 e;              /* last block flag */
    s32 r;              /* result code */

    /* Reset bit buffer */
    g_bitcount = 0;

    /* Process blocks until last block */
    do {
        r = inflate_block(&e);
        if (r != 0) {
            return 1;
        }
    } while (e == 0);

    return 0;
}

/**
 * Main inflate entry point (func_80006814)
 */
s32 inflate_entry(void *src, void *dst, s32 use_heap) {
    void *huft_mem;
    OSMesg msg;
    s32 result;

    /* Initialize state */
    g_outptr = dst;
    g_src = src;
    g_inptr = g_buffer_a;
    g_inend = g_buffer_a;
    g_buffer_toggle = 1;

    /* Clear input buffer */
    osInvalDCache(g_buffer_a, INFLATE_WINDOW_SIZE);

    /* Start initial async read */
    dma_read_async(&g_dma_state, 0, 0, src, g_buffer_a, INFLATE_WINDOW_SIZE, &g_mq);
    g_src = (u8 *)src + INFLATE_WINDOW_SIZE;

    /* Allocate or use pre-built Huffman tables */
    if (use_heap) {
        huft_mem = heap_alloc(0, INFLATE_TABLE_SIZE);
        huft_init(huft_mem, INFLATE_TABLE_SIZE);
    } else {
        huft_init((void *)0x803FD120, INFLATE_TABLE_SIZE);
    }

    /* Main decompression loop */
    inflate_loop();

    /* Cleanup */
    if (use_heap) {
        heap_free(huft_mem);
    }

    /* Wait for final I/O */
    while (osRecvMesg(&g_mq, &msg, OS_MESG_NOBLOCK) == -1) {
        /* Spin */
    }

    /* Finalize DMA */
    result = (u8 *)g_outptr - (u8 *)dst;
    dma_finalize(dst, result);

    return result;
}

/**
 * Alternative inflate entry point (func_8000697C)
 * Always uses heap allocation for Huffman tables
 * Takes input buffer directly instead of ROM source
 */
s32 inflate_entry_alt(void *src, void *dst) {
    void *huft_mem;
    s32 result;

    /* Initialize state - use src directly as input buffer */
    g_outptr = dst;
    g_inptr = src;

    /* Allocate Huffman tables from heap */
    huft_mem = heap_alloc(0, INFLATE_TABLE_SIZE);
    huft_init(huft_mem, INFLATE_TABLE_SIZE);

    /* Main decompression loop */
    inflate_loop();

    /* Cleanup */
    heap_free(huft_mem);

    /* Return bytes written */
    result = (u8 *)g_outptr - (u8 *)dst;
    return result;
}
