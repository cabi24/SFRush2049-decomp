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
    unsigned int fourth;
    Color *colors;
    unsigned int sum;
    int u8,u9,u10,u11,u12,u13,u14,u15;
    a=(car->pal[0]&0xf800)>>11;
    b=(car->pal[1]&0xf800)>>11;
    hobj=(car->pal[2]&0xf800)>>11;
    fourth=(car->pal[3]&0xf800)>>11;
    colors=(Color *)D_8011AF90[D_8014978C];
    sum = colors[a].b[0]+colors[b].b[0]+colors[hobj].b[0]+colors[fourth].b[0];
    rgb.b[0] = sum >> 2;
    sum = colors[a].b[1]+colors[b].b[1]+colors[hobj].b[1]+colors[fourth].b[1];
    rgb.b[1] = sum >> 2;
    sum = colors[a].b[2]+colors[b].b[2]+colors[hobj].b[2]+colors[fourth].b[2];
    rgb.b[2] = sum >> 2;
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

