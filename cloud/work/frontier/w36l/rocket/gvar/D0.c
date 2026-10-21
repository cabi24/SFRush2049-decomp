static Color GetCarsIRColor(Car *car, CarState *m)
{
    Color clr;
    int u6;
    Color sclr2;
    Color rgb;
    float d1;
    float d2;
    unsigned int a;
    unsigned int b;
    unsigned int hobj;
    Color *colors;
    int u7,u8,u9,u10,u11,u12,u13,u14,u15;
    a=(car->pal[0]&0xf800)>>11;
    hobj=(car->pal[1]&0xf800)>>11;
    colors=(Color *)D_8011AF90[D_8014978C];
    b = colors[a].b[0]+colors[hobj].b[0]+colors[(car->pal[2]&0xf800)>>11].b[0]+colors[(car->pal[3]&0xf800)>>11].b[0];
    rgb.b[0] = b >> 2;
    b = colors[a].b[1]+colors[hobj].b[1]+colors[(car->pal[2]&0xf800)>>11].b[1]+colors[(car->pal[3]&0xf800)>>11].b[1];
    rgb.b[1] = b >> 2;
    b = colors[a].b[2]+colors[hobj].b[2]+colors[(car->pal[2]&0xf800)>>11].b[2]+colors[(car->pal[3]&0xf800)>>11].b[2];
    rgb.b[2] = b >> 2;
    if (m->dist >= 20.0f) {
        d1 = 0.025f * (m->dist - 20.0f);
        if (d1 > 1.0f)
            return ((Color *)D_8011AF90[D_8014978C])[D_8011B468[D_8014978C]];
        if (d1 > 0.0f) {
            sclr2.w = ((Color *)D_8011AF90[D_8014978C])[D_8011B468[D_8014978C]].w;
            d2 = 1.0f - d1;
            a = rgb.b[0];
            b = sclr2.b[0];
            rgb.b[0] = a * d2 + b * d1;
            a = rgb.b[1];
            b = sclr2.b[1];
            rgb.b[1] = a * d2 + b * d1;
            a = rgb.b[2];
            b = sclr2.b[2];
            rgb.b[2] = a * d2 + b * d1;
        }
    }
    clr.b[0] = rgb.b[0];
    clr.b[1] = rgb.b[1];
    clr.b[2] = rgb.b[2];
    clr.b[3] = 255;
    return clr;
}
