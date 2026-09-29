/*
 * car_collision_init / func_800A8174 are not game logic: they are zlib's
 * deflate bit writer, _tr_stored_block() (with copy_block() inlined) and
 * bi_windup() from trees.c. Source follows zlib 1.0.4 (tools/zlib-1.0.4/
 * trees.c, deflate.h). The game's deflate_state has the same tail as 1.0.4
 * without DEBUG (compressed_len 0x1694, matches 0x1698, last_eob_len 0x169C,
 * bi_buf 0x16A0, bi_valid 0x16A4) but a head 4 bytes shorter (pending_buf at
 * 0x4, pending at 0xC), so only the fields these functions use are placed.
 */
typedef unsigned char uch;
typedef unsigned short ush;
typedef unsigned long ulg;
typedef char charf;

typedef struct deflate_state {
    /* 0x0000 */ void *strm;
    /* 0x0004 */ uch *pending_buf;
    /* 0x0008 */ uch *pending_out;
    /* 0x000C */ int pending;
    /* 0x0010 */ char pad10[0x1694 - 0x10];
    /* 0x1694 */ ulg compressed_len;
    /* 0x1698 */ unsigned int matches;
    /* 0x169C */ int last_eob_len;
    /* 0x16A0 */ ush bi_buf;
    /* 0x16A4 */ int bi_valid;
} deflate_state;

#define local static
#define Buf_size (8 * 2*sizeof(char))
#define STORED_BLOCK 0

#define put_byte(s, c) {s->pending_buf[s->pending++] = (c);}
#define put_short(s, w) { \
    put_byte(s, (uch)((w) & 0xff)); \
    put_byte(s, (uch)((ush)(w) >> 8)); \
}
#define send_bits(s, value, length) \
{ int len = length;\
  if (s->bi_valid > (int)Buf_size - len) {\
    int val = value;\
    s->bi_buf |= (val << s->bi_valid);\
    put_short(s, s->bi_buf);\
    s->bi_buf = (ush)val >> (Buf_size - s->bi_valid);\
    s->bi_valid += len - Buf_size;\
  } else {\
    s->bi_buf |= (value) << s->bi_valid;\
    s->bi_valid += len;\
  }\
}

void car_collision_init(deflate_state *s, charf *buf, ulg stored_len, int eof);
void func_800A8174(deflate_state *s);
local void copy_block(deflate_state *s, charf *buf, unsigned len, int header);

/* zlib: _tr_stored_block */
void car_collision_init(deflate_state *s, charf *buf, ulg stored_len, int eof)
{
    send_bits(s, (STORED_BLOCK<<1)+eof, 3);  /* send block type */
    s->compressed_len = (s->compressed_len + 3 + 7) & (ulg)~7L;
    s->compressed_len += (stored_len + 4) << 3;

    copy_block(s, buf, (unsigned)stored_len, 1); /* with header */
}

/* zlib: bi_windup */
void func_800A8174(deflate_state *s)
{
    if (s->bi_valid > 8) {
        put_short(s, s->bi_buf);
    } else if (s->bi_valid > 0) {
        put_byte(s, (uch)s->bi_buf);
    }
    s->bi_buf = 0;
    s->bi_valid = 0;
}

/* zlib: copy_block (inlined into _tr_stored_block under -O3) */
local void copy_block(deflate_state *s, charf *buf, unsigned len, int header)
{
    func_800A8174(s);    /* align on byte boundary */
    s->last_eob_len = 8; /* enough lookahead for inflate */

    if (header) {
        put_short(s, (ush)len);
        put_short(s, (ush)~len);
    }
    while (len--) {
        put_byte(s, *buf++);
    }
}

/* stand-in caller: keeps func_800A8174 out of line under -O3 */
void __standin_func_800A8174(void)
{
    func_800A8174(0);
}
