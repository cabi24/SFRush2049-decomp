/**
 * @file inflate.h
 * @brief DEFLATE decompression (inflate) interface
 *
 * Rush 2049 uses a DEFLATE/zlib inflate implementation optimized for N64:
 * - 4KB double-buffered sliding window (vs standard 32KB)
 * - Async I/O for ROM access optimization
 * - Based on standard inflate algorithm (RFC 1951)
 */

#ifndef _INFLATE_H_
#define _INFLATE_H_

#include "types.h"

/**
 * Huffman decoding table entry
 */
struct huft {
    u8 e;                /* number of extra bits or operation */
    u8 b;                /* number of bits in this code or subcode */
    union {
        u16 n;           /* literal, length base, or distance base */
        struct huft *t;  /* pointer to next level of table */
    } v;
};

/* Special e values */
#define HUFT_LITERAL   16  /* literal value */
#define HUFT_END       15  /* end of block */
#define HUFT_INVALID   99  /* invalid code */

/* Maximum Huffman code bit length */
#define BMAX 16

/* Maximum number of codes */
#define N_MAX 288

/* Buffer sizes */
#define INFLATE_WINDOW_SIZE  0x1000  /* 4KB sliding window */
#define INFLATE_TABLE_SIZE   0x2EE0  /* Huffman table allocation size */

/**
 * Decompress DEFLATE-compressed data
 * @param src Source compressed data pointer
 * @param dst Destination buffer pointer
 * @param use_heap If non-zero, allocate Huffman tables from heap
 * @return Number of bytes written to dst
 */
s32 inflate_entry(void *src, void *dst, s32 use_heap);

/**
 * Alternative entry point (always uses heap allocation)
 * @param src Source compressed data pointer
 * @param dst Destination buffer pointer
 * @return Number of bytes written to dst
 */
s32 inflate_entry_alt(void *src, void *dst);

/**
 * Build Huffman decoding tables
 * @param b Array of code lengths
 * @param n Number of codes
 * @param s Number of simple-valued codes
 * @param d Base values for non-simple codes
 * @param e Extra bits for non-simple codes
 * @param t Result: starting table pointer
 * @param m Maximum lookup bits (input), actual bits (output)
 * @return 0 on success, non-zero on error
 */
s32 huft_build(u32 *b, u32 n, u32 s, u16 *d, u8 *e, struct huft **t, s32 *m);

#endif /* _INFLATE_H_ */
