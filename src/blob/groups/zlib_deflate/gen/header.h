/*
 * zlib deflate as built into San Francisco Rush 2049 (N64): zlib 1.0.4
 * trees.c / deflate.c with a trimmed state (see ../ZLIB.md). Generated from
 * tools/zlib-1.0.4 by the edits listed in GENERATION.md; each zlib function
 * keeps its zlib name and is mapped to the game's label below.
 *
 * zlib (C) 1995-1996 Jean-loup Gailly and Mark Adler; see
 * tools/zlib-1.0.4/README for the licence.
 */

/* zlib name -> game label */
#define flush_pending       car_cg_height_set
#define bi_windup           func_800A8174
#define init_block          func_800A81FC
#define compress_block      func_800A8284
#define send_tree           func_800A8778
#define _tr_stored_block    car_collision_init
#define bi_reverse          func_800A8F38
#define gen_codes           car_collision_update
#define gen_bitlen          func_800A9054
#define pqdownheap          func_800A928C
#define build_tree          car_crash_detect
#define scan_tree           func_800A9710
#define _tr_flush_block     car_crash_response
#define _tr_tally           func_800AA028
#define longest_match       func_800AA224
#define fill_window         car_reset_position
#define deflate_slow        car_spawn_at_checkpoint
#define _tr_init            func_800AAB3C
#define deflate_mem         car_angular_velocity_clamp

typedef unsigned int size_t;
extern void *memcpy(void *, const void *, size_t);
extern void *memset(void *, int, size_t);

#define local  /* zlib's file-static functions: globals made internal by uld -kp */
#define OF(args) args
#define FAR
#define Z_NULL 0
#define zmemcpy memcpy
#define zmemzero(dest, len) memset(dest, 0, len)
#define Assert(cond, msg)
#define Trace(x)
#define Tracev(x)
#define Tracevv(x)
#define Tracec(c, x)
#define Tracecv(c, x)
#define TRUNCATE_BLOCK
#define check_match(s, start, match, length)

typedef unsigned char  Byte;
typedef unsigned int   uInt;
typedef unsigned long  uLong;
typedef Byte  FAR Bytef;
typedef char  FAR charf;
typedef int   FAR intf;
typedef uInt  FAR uIntf;
typedef uLong FAR uLongf;
typedef void  FAR *voidpf;
typedef unsigned char  uch;
typedef uch FAR uchf;
typedef unsigned short ush;
typedef ush FAR ushf;
typedef unsigned long  ulg;

#define Z_NO_FLUSH      0
#define Z_FINISH        4
#define Z_BINARY   0
#define Z_ASCII    1

/* the game's z_stream: 1.0.4 minus msg and everything after state */
typedef struct z_stream_s {
    Bytef    *next_in;   /* 0x00 */
    uInt     avail_in;   /* 0x04 */
    uLong    total_in;   /* 0x08 */
    Bytef    *next_out;  /* 0x0C */
    uInt     avail_out;  /* 0x10 */
    uLong    total_out;  /* 0x14 */
    struct internal_state FAR *state; /* 0x18 */
} z_stream;
typedef z_stream FAR *z_streamp;

#define STORED_BLOCK 0
#define STATIC_TREES 1
#define DYN_TREES    2
#define MIN_MATCH  3
#define MAX_MATCH  258

#define LENGTH_CODES 29
#define LITERALS  256
#define L_CODES (LITERALS+1+LENGTH_CODES)
#define D_CODES   30
#define BL_CODES  19
#define HEAP_SIZE (2*L_CODES+1)
#define MAX_BITS 15

typedef struct ct_data_s {
    union {
        ush  freq;
        ush  code;
    } fc;
    union {
        ush  dad;
        ush  len;
    } dl;
} FAR ct_data;

#define Freq fc.freq
#define Code fc.code
#define Dad  dl.dad
#define Len  dl.len

typedef struct static_tree_desc_s  static_tree_desc;

typedef struct tree_desc_s {
    ct_data *dyn_tree;
    int     max_code;
    static_tree_desc *stat_desc;
} FAR tree_desc;

typedef ush Pos;
typedef Pos FAR Posf;
typedef ush IPos;      /* 1.0.4: unsigned. The game masks IPos values to 16 bits */

/* the game's deflate_state: 1.0.4 without status, two of noheader /
 * data_type+method / last_flush, strategy, and DEBUG's bits_sent */
typedef struct internal_state {
    z_streamp strm;       /* 0x00 */
    Bytef *pending_buf;   /* 0x04 */
    Bytef *pending_out;   /* 0x08 */
    int   pending;        /* 0x0C */
    int   unk10;          /* 0x10: not read by these functions */
    uInt  w_size;         /* 0x14 */
    uInt  w_bits;
    uInt  w_mask;
    Bytef *window;        /* 0x20 */
    ulg window_size;
    Posf *prev;
    Posf *head;           /* 0x2C */
    uInt  ins_h;
    uInt  hash_size;      /* 0x34 */
    uInt  hash_bits;
    uInt  hash_mask;
    uInt  hash_shift;
    long block_start;     /* 0x44 */
    uInt match_length;
    IPos prev_match;
    int match_available;
    uInt strstart;        /* 0x54 */
    uInt match_start;
    uInt lookahead;       /* 0x5C */
    uInt prev_length;
    uInt max_chain_length;
    uInt max_lazy_match;
    int level;            /* 0x6C */
    uInt good_match;      /* 0x70 */
    int nice_match;       /* 0x74 */
    struct ct_data_s dyn_ltree[HEAP_SIZE];   /* 0x78 */
    struct ct_data_s dyn_dtree[2*D_CODES+1]; /* 0x96C */
    struct ct_data_s bl_tree[2*BL_CODES+1];
    struct tree_desc_s l_desc;
    struct tree_desc_s d_desc;
    struct tree_desc_s bl_desc;
    ush bl_count[MAX_BITS+1];
    int heap[2*L_CODES+1];
    int heap_len;
    int heap_max;
    uch depth[2*L_CODES+1];
    uchf *l_buf;          /* 0x167C */
    uInt  lit_bufsize;
    uInt last_lit;        /* 0x1684 */
    ushf *d_buf;
    ulg opt_len;
    ulg static_len;
    ulg compressed_len;   /* 0x1694 */
    uInt matches;
    int last_eob_len;
    ush bi_buf;           /* 0x16A0 */
    int bi_valid;         /* 0x16A4 */
} FAR deflate_state;

#define put_byte(s, c) {s->pending_buf[s->pending++] = (c);}
#define MIN_LOOKAHEAD (MAX_MATCH+MIN_MATCH+1)
#define MAX_DIST(s)  ((s)->w_size-MIN_LOOKAHEAD)
#define d_code(dist) \
   ((dist) < 256 ? dist_code[dist] : dist_code[256+((dist)>>7)])

#define NIL 0
#define Buf_size (8 * 2*sizeof(char))
#define TOO_FAR 4096

typedef enum {
    need_more,
    block_done,
    finish_started,
    finish_done
} block_state;

typedef struct config_s {
   ush good_length;
   ush max_lazy;
   ush nice_length;
   ush max_chain;
} config;

void _tr_init         OF((deflate_state *s));
int  _tr_tally        OF((deflate_state *s, unsigned dist, unsigned lc));
ulg  _tr_flush_block  OF((deflate_state *s, charf *buf, ulg stored_len, int eof));
void _tr_stored_block OF((deflate_state *s, charf *buf, ulg stored_len, int eof));
local void flush_pending  OF((z_streamp strm));
local void fill_window    OF((deflate_state *s));
local block_state deflate_slow OF((deflate_state *s));
local uInt longest_match  OF((deflate_state *s, IPos cur_match));
local int read_buf        OF((z_streamp strm, charf *buf, unsigned size));
local void tr_static_init OF((void));
local void init_block     OF((deflate_state *s));
local void pqdownheap     OF((deflate_state *s, ct_data *tree, int k));
local void gen_bitlen     OF((deflate_state *s, tree_desc *desc));
local void gen_codes      OF((ct_data *tree, int max_code, ushf *bl_count));
local void build_tree     OF((deflate_state *s, tree_desc *desc));
local void scan_tree      OF((deflate_state *s, ct_data *tree, int max_code));
local void send_tree      OF((deflate_state *s, ct_data *tree, int max_code));
local int  build_bl_tree  OF((deflate_state *s));
local void send_all_trees OF((deflate_state *s, int lcodes, int dcodes, int blcodes));
local void compress_block OF((deflate_state *s, ct_data *ltree, ct_data *dtree));
local unsigned bi_reverse OF((unsigned value, int length));
local void bi_windup      OF((deflate_state *s));
local void copy_block     OF((deflate_state *s, charf *buf, unsigned len, int header));
