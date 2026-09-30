void func_8008A704(void)
{
  OSMesg m = 0;
  if (!D_8011194C) {
    D_8011194C = (s8) 1;
    osCreateMesgQueue(&D_801497D0, (OSMesg *) &D_801527E4, 1);
    osJamMesg(&D_801497D0, (OSMesg) 0, 0);
  }
  (void) osRecvMesg(&D_801497D0, &m, 1);
}
