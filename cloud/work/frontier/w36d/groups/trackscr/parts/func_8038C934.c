void func_8038C934(void)
{
    static TileSize fogTileSize;
    TileSize *tile = &fogTileSize;
    s32 s;
    s32 speed;
    speed = D_80142726 ? D_80142726 * 5 : 1;
    s = tile->uls + speed;
    if (s < 0) s += 2048;
    if (s >= 2048) s -= 2048;
    tile->uls = s;
    tile->ult = 0;
}
