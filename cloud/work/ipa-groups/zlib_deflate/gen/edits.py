[
 # static_init_done is a fixed game variable, not a function-local static
 # _tr_flush_block: no set_data_type (the game's state has no data_type)
 ("\t /* Check if the file is ascii or binary */\n\tif (s->data_type == Z_UNKNOWN) set_data_type(s);\n", ""),
 # read_buf: no adler32 (the game never writes a zlib header)
 ("    if (!strm->state->noheader) {\n        strm->adler = adler32(strm->adler, strm->next_in, len);\n    }\n", ""),
 # deflate_slow: no strategy field
 ("""            if (s->strategy != Z_HUFFMAN_ONLY) {
                s->match_length = longest_match (s, hash_head);
            }""", """            s->match_length = longest_match (s, hash_head);"""),
 ("(s->strategy == Z_FILTERED ||\n                 (s->match_length == MIN_MATCH &&\n                  s->strstart - s->match_start > TOO_FAR))",
  "(s->match_length == MIN_MATCH &&\n                  s->strstart - s->match_start > TOO_FAR)"),
 # tr_static_init: the game's frame is 8 bytes larger than 1.0.4's; the extra
 # locals are unknown (FAKE padding, unreferenced)
 ("    int n;        /* iterates over tree elements */\n    int bits;     /* bit counter */", "    int pad[2];   /* FAKE: see STATUS.md */\n    int n;        /* iterates over tree elements */\n    int bits;     /* bit counter */"),
 # deflate_slow: one-shot use, so no flush parameter: always Z_FINISH
 ("local block_state deflate_slow(s, flush)\n    deflate_state *s;\n    int flush;\n{", "local block_state deflate_slow(s)\n    deflate_state *s;\n{"),
 ("""            fill_window(s);
            if (s->lookahead < MIN_LOOKAHEAD && flush == Z_NO_FLUSH) {
\t        return need_more;
\t    }
""", """            fill_window(s);
"""),
 ("    Assert (flush != Z_NO_FLUSH, \"no flush?\");\n", ""),
 ("    FLUSH_BLOCK(s, flush == Z_FINISH);\n    return flush == Z_FINISH ? finish_done : block_done;", "    FLUSH_BLOCK(s, 1);\n    return block_done; /* the game returns 1 here */"),
]
