
/* ===========================================================================
 * The game's one-shot compressor (not part of zlib): deflateInit2_,
 * deflateReset and lm_init inlined, one deflate_slow pass, deflateEnd.
 * Hand-written from the assembly.
 */
extern void *audio_dma_sync(voidpf opaque, uInt size);
extern void audio_reverb_update(voidpf opaque, voidpf ptr, int flag);
extern int osRecvMesg(void *mq, void *msg, int flag);
extern int osJamMesg(void *mq, void *msg, int flag);
extern char D_80152770[];

#define ZALLOC(size) audio_dma_sync(Z_NULL, (size))

/* free under the heap lock; -O3 inlines it at each use */
static void zfree_locked(voidpf opaque, voidpf ptr)
{
    osRecvMesg(D_80152770, Z_NULL, 1);
    audio_reverb_update(opaque, ptr, 0);
    osJamMesg(D_80152770, Z_NULL, 0);
}
#define TRY_FREE(ptr) { if (ptr) zfree_locked(opaque, ptr); }

uLong deflate_mem(Bytef *next_in, uInt avail_in, Bytef *next_out, uInt avail_out,
                  int level, int windowBits, int memLevel)
{
    z_stream strm;
    deflate_state *s;
    ushf *overlay;
    voidpf opaque; /* never set: the game passes whatever is in $a0 */

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
    s->hash_shift =  ((s->hash_bits+MIN_MATCH-1)/MIN_MATCH);

    s->window = (Bytef *) ZALLOC(s->w_size * 2*sizeof(Byte));
    s->prev   = (Posf *)  ZALLOC(s->w_size * sizeof(Pos));
    s->head   = (Posf *)  ZALLOC(s->hash_size * sizeof(Pos));

    s->lit_bufsize = 1 << (memLevel + 6);

    overlay = (ushf *) ZALLOC(s->lit_bufsize * (sizeof(ush)+2));
    s->pending_buf = (uchf *) overlay;
    s->d_buf = overlay + s->lit_bufsize/sizeof(ush);
    s->l_buf = s->pending_buf + (1+sizeof(ush))*s->lit_bufsize;

    s->level = level;
    s->good_match       = configuration_table[level].good_length;
    s->max_lazy_match   = configuration_table[level].max_lazy;
    s->nice_match       = configuration_table[level].nice_length;
    s->max_chain_length = configuration_table[level].max_chain;

    /* deflateReset */
    s = strm.state;
    s->pending = 0;
    s->pending_out = s->pending_buf;
    _tr_init(s);

    /* lm_init */
    s->window_size = (ulg)2L*s->w_size;
    CLEAR_HASH(s);
    s->strstart = 0;
    s->block_start = 0L;
    s->lookahead = 0;
    s->match_length = s->prev_length = MIN_MATCH-1;
    s->match_available = 0;
    s->ins_h = 0;

    deflate_slow(strm.state);

    /* deflateEnd */
    TRY_FREE(s->pending_buf);
    TRY_FREE(s->head);
    TRY_FREE(s->prev);
    TRY_FREE(s->window);
    TRY_FREE(s);
    return strm.total_out;
}
