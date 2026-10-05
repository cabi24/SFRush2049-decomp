/* Synthetic host harness; the two callbacks model only this caller's contracts. */
#include <assert.h>
#include <string.h>
#ifndef CANDIDATE
#define CANDIDATE "../../../matches/stat_race_update.c"
#endif
#include CANDIDATE

static Blit *current;
static TexDef replacement;
static int rename_present, renames, updates;
static char name[] = "synthetic";

void func_800EF5B0(Blit *blt, char *requested, s32 preserve)
{
    assert(blt == current && requested == name && preserve == 1);
    ++renames;
    blt->Info = rename_present ? &replacement : 0;
}

void Input_ApplyPadConfig(Blit *blt)
{
    assert(blt == current);
    ++updates;
}

void run_case(int present, int info_present, int width, int index,
              int hsize, int vsize, int flip, int replacement_present,
              int replacement_width, int *out)
{
    Blit blt;
    TexDef original;
    memset(&blt, 0, sizeof(blt));
    memset(&original, 0, sizeof(original));
    memset(&replacement, 0, sizeof(replacement));
    original.Width = width;
    replacement.Width = replacement_width;
    rename_present = replacement_present;
    renames = updates = 0;
    current = &blt;
    blt.Name = name;
    blt.Info = info_present ? &original : 0;
    blt.Width = 111; blt.Height = 222;
    blt.Alpha = 17; blt.Flip = flip; blt.Hide = -7; blt.Init = 19;
    blt.Top = -30000; blt.Bot = 1234; blt.Left = -5678; blt.Right = 30000;
    stat_race_update(present ? &blt : 0, index, hsize, vsize);
    out[0] = blt.Top; out[1] = blt.Bot; out[2] = blt.Left; out[3] = blt.Right;
    out[4] = blt.Width; out[5] = blt.Height;
    out[6] = blt.Alpha; out[7] = blt.Flip; out[8] = blt.Hide; out[9] = blt.Init;
    out[10] = renames; out[11] = updates;
    out[12] = blt.Info == 0 ? 0 : (blt.Info == &original ? 1 : 2);
}
