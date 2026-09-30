void car_collision_init(deflate_state *s, charf *buf, ulg stored_len, int eof)
{
    send_bits(s, (STORED_BLOCK<<1)+eof, 3);  /* send block type */
    s->compressed_len = (s->compressed_len + 3 + 7) & (ulg)~7L;
    s->compressed_len += (stored_len + 4) << 3;

    copy_block(s, buf, (unsigned)stored_len, 1); /* with header */
}