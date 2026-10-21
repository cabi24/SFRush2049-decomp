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
    unsigned int i0;
    unsigned int i1;
    unsigned int i2;
    unsigned int i3;
    int u8,u9,u10,u11,u12,u13,u14,u15;
    i0=(car->pal[0]&0xf800)>>11;
    i1=(car->pal[1]&0xf800)>>11;
    i2=(car->pal[2]&0xf800)>>11;
    i3=(car->pal[3]&0xf800)>>11;
    b = ((Color *)D_8011AF90[D_8014978C])[i0].b[0]+((Color *)D_8011AF90[D_8014978C])[i1].b[0]+((Color *)D_8011AF90[D_8014978C])[i2].b[0]+((Color *)D_8011AF90[D_8014978C])[i3].b[0];
    rgb[0] = b >> 2;
    b = ((Color *)D_8011AF90[D_8014978C])[i0].b[1]+((Color *)D_8011AF90[D_8014978C])[i1].b[1]+((Color *)D_8011AF90[D_8014978C])[i2].b[1]+((Color *)D_8011AF90[D_8014978C])[i3].b[1];
    rgb[1] = b >> 2;
    b = ((Color *)D_8011AF90[D_8014978C])[i0].b[2]+((Color *)D_8011AF90[D_8014978C])[i1].b[2]+((Color *)D_8011AF90[D_8014978C])[i2].b[2]+((Color *)D_8011AF90[D_8014978C])[i3].b[2];
    rgb[2] = b >> 2;
    if (m->dist >= 20.0f) {
        d1 = 0.025f * (m->dist - 20.0f);
        if (d1 > 1.0f)
            return ((Color *)D_8011AF90[D_8014978C])[D_8011B468[D_8014978C]];
        if (d1 > 0.0f) {
            sclr2.w = ((Color *)D_8011AF90[D_8014978C])[D_8011B468[D_8014978C]].w;
            d2 = 1.0f - d1;
            a = rgb[0];
            b = sclr2.b[0];
            rgb[0] = a * d2 + b * d1;
            a = rgb[1];
            b = sclr2.b[1];
            rgb[1] = a * d2 + b * d1;
            a = rgb[2];
            b = sclr2.b[2];
            rgb[2] = a * d2 + b * d1;
        }
    }
    clr.b[0] = rgb[0];
    clr.b[1] = rgb[1];
    clr.b[2] = rgb[2];
    clr.b[3] = 255;
    return clr;
}
