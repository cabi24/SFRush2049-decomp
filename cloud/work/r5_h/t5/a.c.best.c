void func_8008A704(void)
{
  OSMesg m;
  if (D_8011194C == 0) {
    D_8011194C = 1;
    osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
    osJamMesg(&D_801497D0, 0, 0);
  }
  osRecvMesg(&D_801497D0, &m, 1);
}
