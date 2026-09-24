/* deflate104 — compress a file exactly as the Rush 2049 cartridge's game-code
 * blob was compressed: zlib 1.0.4, raw DEFLATE (windowBits -15), level 9,
 * memLevel 8, default strategy. Parameters are fixed on purpose: they are a
 * fact about the original build, not a choice.
 *
 *     deflate104 <in> <out>
 *
 * See tools/zlib-1.0.4/PROVENANCE.md. Build-only tool; never feed it
 * untrusted input. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zlib.h"

int main(int argc, char **argv) {
    FILE *in, *out;
    long in_size;
    unsigned char *src, *dst;
    unsigned long dst_cap;
    z_stream s;
    int rc;

    if (argc != 3) {
        fprintf(stderr, "usage: %s <in> <out>\n", argv[0]);
        return 2;
    }
    in = fopen(argv[1], "rb");
    if (!in) { perror(argv[1]); return 1; }
    fseek(in, 0, SEEK_END);
    in_size = ftell(in);
    fseek(in, 0, SEEK_SET);
    src = malloc((size_t)in_size);
    dst_cap = (unsigned long)in_size + in_size / 1000 + 64;
    dst = malloc(dst_cap);
    if (!src || !dst || fread(src, 1, (size_t)in_size, in) != (size_t)in_size) {
        fprintf(stderr, "read failed\n");
        return 1;
    }
    fclose(in);

    memset(&s, 0, sizeof s);
    rc = deflateInit2(&s, 9, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY);
    if (rc != Z_OK) { fprintf(stderr, "deflateInit2: %d\n", rc); return 1; }
    s.next_in = src;
    s.avail_in = (uInt)in_size;
    s.next_out = dst;
    s.avail_out = (uInt)dst_cap;
    rc = deflate(&s, Z_FINISH);
    if (rc != Z_STREAM_END) { fprintf(stderr, "deflate: %d\n", rc); return 1; }
    deflateEnd(&s);

    out = fopen(argv[2], "wb");
    if (!out || fwrite(dst, 1, s.total_out, out) != s.total_out) {
        perror(argv[2]);
        return 1;
    }
    fclose(out);
    printf("deflate104: %ld -> %lu bytes (zlib %s)\n", in_size, s.total_out,
           zlibVersion());
    return 0;
}
