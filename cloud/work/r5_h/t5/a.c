void func_8008A704(void)
{
  /*@1: OSMesg m; || void *m; || OSMesg m = 0; */
  if (/*@2: D_8011194C == 0 || !D_8011194C */) {
    /*@3: D_8011194C = 1; || D_8011194C = (s8) 1; */
    /*@4: osCreateMesgQueue(&D_801497D0, &D_801527E4, 1); || osCreateMesgQueue(&D_801497D0, (OSMesg *) &D_801527E4, 1); */
    /*@5: osJamMesg(&D_801497D0, 0, 0); || osJamMesg(&D_801497D0, (OSMesg) 0, 0); */
  }
  /*@6: osRecvMesg(&D_801497D0, &m, 1); || osRecvMesg(&D_801497D0, (OSMesg *) &m, 1); || (void) osRecvMesg(&D_801497D0, &m, 1); */
}
