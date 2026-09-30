void func_800E7038(void)
{
  if (D_80111954 == 0)
  {
    player_state_set(-1, 1);
    player_mode_set(-1, 1);
  }
  D_80111954 = 1;
  D_80149AFE = 0x46;
  D_80149AFF = 0x46;
  D_80149AFC = 0x46;
  D_80149AFD = 0x46;
  D_80149AFA = 0x46;
  D_80149AFB = 0x46;
  D_80149AF8 = 0x46;
  D_80149AF9 = 0x46;
  if (D_80111968 == 0)
  {
    D_80111968 = 1;
    osCreateMesgQueue((OSMesgQueue *) (&D_801497A8), (void **) (&D_80152730), 1);
    osJamMesg((OSMesgQueue *) (&D_801497A8), (void *) 0, 0);
  }
  if (D_80111950 == 0x80)
  {
    do
    {
    }
    while (D_80111950 == 0x80);
  }
  controller_poll();
}
