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