static Color GetCarsIRColor(Car *car, CarState *m)
{
    Color clr;
    int u6;
    Color sclr2;
    unsigned char rgb[3];
    float d1;
    float d2;
    unsigned int a;
    unsigned int b;
    unsigned int hobj;
    unsigned int c;
    unsigned int d;
    int u7;
    int u8,u9,u10,u11,u12,u13,u14,u15;
    a=(car->pal[0]&0xf800)>>11;
    hobj=(car->pal[1]&0xf800)>>11;
    c=(car->pal[2]&0xf800)>>11;
    d=(car->pal[3]&0xf800)>>11;
    b = ((Color *)D_8011AF90[D_8014978C])[a].b[0]+((Color *)D_8011AF90[D_8014978C])[hobj].b[0]+((Color *)D_8011AF90[D_8014978C])[c].b[0]+((Color *)D_8011AF90[D_8014978C])[d].b[0];
    rgb[0] = b >> 2;
    b = ((Color *)D_8011AF90[D_8014978C])[a].b[1]+((Color *)D_8011AF90[D_8014978C])[hobj].b[1]+((Color *)D_8011AF90[D_8014978C])[c].b[1]+((Color *)D_8011AF90[D_8014978C])[d].b[1];
    rgb[1] = b >> 2;
    b = ((Color *)D_8011AF90[D_8014978C])[a].b[2]+((Color *)D_8011AF90[D_8014978C])[hobj].b[2]+((Color *)D_8011AF90[D_8014978C])[c].b[2]+((Color *)D_8011AF90[D_8014978C])[d].b[2];
    rgb[2] = b >> 2;
    if (m->dist >= 20.0f) {
        d1 = 0.025f * (m->dist - 20.0f);
        if (d1 > 1.0f)
            return ((Color *)D_8011AF90[D_8014978C])[D_8011B468[D_8014978C]];
        if (d1 > 0.0f) {
            sclr2.w = ((Color *)D_8011AF90[D_8014978C])[D_8011B468[D_8014978C]].w;
            d2 = 1.0f - d1;
            c = rgb[0];
            b = sclr2.b[0];
            rgb[0] = c * d2 + b * d1;
            c = rgb[1];
            b = sclr2.b[1];
            rgb[1] = c * d2 + b * d1;
            c = rgb[2];
            b = sclr2.b[2];
            rgb[2] = c * d2 + b * d1;
        }
    }
    clr.b[0] = rgb[0];
    clr.b[1] = rgb[1];
    clr.b[2] = rgb[2];
    clr.b[3] = 255;
    return clr;
}
