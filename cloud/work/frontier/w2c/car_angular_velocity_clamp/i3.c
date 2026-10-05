/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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

#define configuration_table  D_8011E838
extern config configuration_table[10];
#define NIL 0
extern void *audio_dma_sync(voidpf opaque, uInt size);
extern int osRecvMesg(void *mq, void *msg, int flag);
extern int osJamMesg(void *mq, void *msg, int flag);
extern void audio_reverb_update(unsigned int address, int tag);
extern char D_80152770[];

__inline void heap_lock(void)
{
    osRecvMesg(D_80152770, 0, 1);
}
__inline void heap_unlock(void)
{
    osJamMesg(D_80152770, 0, 0);
}
__inline void mem_free(unsigned int address)
{
    heap_lock();
    audio_reverb_update(address, 0);
    heap_unlock();
}
__inline void zcfree(voidpf opaque, voidpf ptr)
{
    mem_free((unsigned int) ptr);
}
#define FREECALL(p) zcfree(0, (voidpf)(p))

extern void func_800AAB3C(deflate_state *s);
extern int car_spawn_at_checkpoint(deflate_state *s);

#define ZALLOC(size) audio_dma_sync(Z_NULL, (size))
#define TRY_FREE(ptr) { if (ptr) FREECALL(ptr); }

uLong car_angular_velocity_clamp(Bytef *next_in, uInt avail_in, Bytef *next_out, uInt avail_out,
                                 int level, int windowBits, int memLevel)
{
    z_stream strm;
    deflate_state *s;
    ushf *overlay;

    strm.next_in = next_in;
    strm.avail_in = avail_in;
    strm.next_out = next_out;
    strm.avail_out = avail_out;

    s = (deflate_state *) ZALLOC(sizeof(deflate_state));
    strm.total_in = 0;
    strm.total_out = 0;
    strm.state = s;
    s->strm = &strm;

    s->w_bits = windowBits;
    s->w_size = 1 << s->w_bits;
    s->w_mask = s->w_size - 1;

    s->hash_bits = memLevel + 7;
    s->hash_size = 1 << s->hash_bits;
    s->hash_mask = s->hash_size - 1;
    s->hash_shift = ((s->hash_bits + MIN_MATCH - 1) / MIN_MATCH);

    s->window = (Bytef *) ZALLOC(s->w_size * 2 * sizeof(Byte));
    s->prev = (Posf *) ZALLOC(s->w_size * sizeof(Pos));
    s->head = (Posf *) ZALLOC(s->hash_size * sizeof(Pos));

    s->lit_bufsize = 1 << (memLevel + 6);

    overlay = (ushf *) ZALLOC(s->lit_bufsize * (sizeof(ush) + 2));
    s->pending_buf = (uchf *) overlay;
    s->d_buf = overlay + s->lit_bufsize / sizeof(ush);
    s->l_buf = s->pending_buf + (1 + sizeof(ush)) * s->lit_bufsize;

    s->level = level;
    s->good_match = configuration_table[level].good_length;
    s->max_lazy_match = configuration_table[level].max_lazy;
    s->nice_match = configuration_table[level].nice_length;
    s->max_chain_length = configuration_table[level].max_chain;

    /* deflateReset */
    s = strm.state;
    s->pending = 0;
    s->pending_out = s->pending_buf;
    func_800AAB3C(s);

    /* lm_init */
    s->window_size = (ulg) 2L * s->w_size;
    s->head[s->hash_size - 1] = NIL;
    zmemzero((charf *) s->head, (unsigned) (s->hash_size - 1) * sizeof(*s->head));
    s->strstart = 0;
    s->block_start = 0L;
    s->lookahead = 0;
    s->match_length = s->prev_length = MIN_MATCH - 1;
    s->match_available = 0;
    s->ins_h = 0;

    car_spawn_at_checkpoint(strm.state);

    /* deflateEnd */
    TRY_FREE(s->pending_buf);
    TRY_FREE(s->head);
    TRY_FREE(s->prev);
    TRY_FREE(s->window);
    TRY_FREE(s);
    return strm.total_out;
}
