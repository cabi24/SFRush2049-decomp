/* flags: -g0 -O2 -mips2 -G 0 -non_shared */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct Z
{
  u8 *S;
} Z;
typedef struct Y
{
  u8 pad[44];
  Z *z;
} Y;
typedef struct X
{
  Y *y;
} X;
typedef struct R
{
  u8 pad[72];
  X *x;
} R;
extern s16 active_player_count;
extern R input_rec0[];
extern s16 D_801164C0;
extern s16 D_801164C2;
extern s16 D_801164C4;
extern s8 D_80156994;
extern s8 D_80150F7C[];
extern u8 D_80150E30[][13];
extern u8 D_80150DD8[][19];
extern u8 D_80150E88[][4];
extern u8 D_80150EB8[][8];
extern u8 D_80150ED8[][9];
extern u8 D_80150F00[][5];
extern u8 D_80150F40[][6];
s32 func_800B78A4(u32, u8);
void net_state_validate(void)
{
  s16 i;
  s16 j;
  s32 k;
  u8 *S;
  u8 *p;
  u8 *q;
  s32 cnt;
  s32 n;
  s32 a;
  s32 new_var2;
  s32 b;
  s32 c;
  s32 d;
  u8 *new_var;
  s32 v0;
  s32 v1;
  s32 one = 1;
  if (D_801164C0 == one)
  {
    for (i = 0; i < active_player_count; i++)
    {
      for (j = 0; j < 13; j++)
      {
        D_80150E30[i][j] = one;
      }

    }

  }
  else
  {
    for (i = 0; i < active_player_count; i++)
    {
      a = 0;
      b = 0;
      c = 0;
      d = 0;
      if (input_rec0[i].x->y->z != 0)
      {
        for (j = 0; j < 13; j++)
        {
          if (j < 6)
          {
            D_80150E30[i][j] = one;
          }
          else
          {
            D_80150E30[i][j] = 0;
          }
        }

        for (k = 0; k != 576; k += 96)
        {
          p = (input_rec0[i].x->y->z->S + k) + 140;
          a += func_800B78A4((*((u16 *) (p + 92))) & 0xff, 16);
          b += func_800B78A4((*((u16 *) (p + 92))) & 0xff00, 16);
        }

        for (k = 0; k != 256; k += 64)
        {
          p = (input_rec0[i].x->y->z->S + k) + 1292;
          c += func_800B78A4((*((u16 *) (p + 60))) & 0xff, 16);
          d += func_800B78A4((*((u16 *) (p + 60))) & 0xff00, 16);
        }

        if (a >= 48)
        {
          D_80150E30[i][6] = one;
        }
        if (b >= 24)
        {
          D_80150E30[i][7] = one;
        }
        if (b >= 36)
        {
          D_80150E30[i][8] = one;
        }
        if (c >= 32)
        {
          D_80150E30[i][9] = one;
        }
        if (d >= 16)
        {
          D_80150E30[i][10] = one;
        }
        if (d >= 24)
        {
          D_80150E30[i][11] = one;
        }
        if ((((a >= 48) && (b >= 48)) && (c >= 32)) && (d >= 32))
        {
          D_80150E30[i][12] = one;
        }
      }
    }

  }
  if (D_801164C4 == one)
  {
    for (i = 0; i < active_player_count; i++)
    {
      if (input_rec0[i].x->y->z != 0)
      {
        for (j = 0; j < 8; j++)
        {
          if ((j == 6) || (j == 7))
          {
            D_80150EB8[i][j] = 0;
          }
          else
          {
            D_80150EB8[i][j] = one;
          }
        }

        for (j = 0; j < 9; j++)
        {
          D_80150ED8[i][j] = one;
        }

        for (j = 0; j < 5; j++)
        {
          D_80150F00[i][j] = one;
        }

        for (j = 0; j < 6; j++)
        {
          D_80150F40[i][j] = one;
        }

      }
    }

  }
  else
  {
    for (i = 0; i < active_player_count; i++)
    {
      v0 = 0;
      if (input_rec0[i].x->y->z != 0)
      {
        for (n = 0; n < 12; n++)
        {
          v0 += *((s32 *) ((input_rec0[i].x->y->z->S + 228) + (n * 96)));
        }

        v0 = v0 / 10;
        D_80150EB8[i][0] = one;
        D_80150EB8[i][1] = one;
        new_var2 = v0;
        D_80150EB8[i][2] = new_var2 >= 200;
        D_80150ED8[i][3] = new_var2 >= 250;
        D_80150EB8[i][3] = new_var2 >= 200;
        D_80150EB8[i][4] = new_var2 >= 500;
        D_80150EB8[i][5] = new_var2 >= 500;
        D_80150EB8[i][6] = 0;
        D_80150EB8[i][7] = 0;
        D_80150ED8[i][4] = new_var2 >= 500;
        D_80150ED8[i][0] = one;
        D_80150ED8[i][1] = one;
        D_80150ED8[i][2] = one;
        D_80150ED8[i][5] = new_var2 >= 800;
        D_80150ED8[i][7] = new_var2 >= 1600;
        D_80150ED8[i][8] = new_var2 >= 2000;
        D_80150F00[i][1] = new_var2 >= 300;
        D_80150F00[i][3] = new_var2 >= 100;
        D_80150F00[i][4] = new_var2 >= 600;
        D_80150ED8[i][6] = new_var2 >= 1200;
        D_80150F00[i][0] = one;
        D_80150F00[i][2] = new_var2 >= 1200;
        D_80150F40[i][0] = one;
        D_80150F40[i][1] = one;
        D_80150F40[i][2] = one;
        D_80150F40[i][3] = new_var2 >= 150;
        D_80150F40[i][4] = new_var2 >= 400;
        D_80150F40[i][5] = new_var2 >= 700;
      }
    }

  }
  if (D_801164C2 == one)
  {
    for (i = 0; i < active_player_count; i++)
    {
      for (j = 0; j < 19; j++)
      {
        D_80150DD8[i][j] = one;
      }

    }

  }
  else
  {
    for (i = 0; i < active_player_count; i++)
    {
      v0 = 0;
      for (j = 0; j < 19; j++)
      {
        D_80150DD8[i][j] = 0;
      }

      D_80150DD8[i][0] = one;
      D_80150DD8[i][1] = one;
      D_80150DD8[i][2] = one;
      D_80150DD8[i][3] = one;
      D_80150DD8[i][14] = one;
      D_80150DD8[i][6] = one;
      D_80150DD8[i][7] = one;
      D_80150DD8[i][8] = one;
      D_80150DD8[i][9] = one;
      if (input_rec0[i].x->y->z != 0)
      {
        S = input_rec0[i].x->y->z->S;
        q = S + 1292;
        v1 = 0;
        for (n = 0; n < 4; n++)
        {
          v1 += *((s32 *) ((q + 12) + (n * 64)));
        }

        q = S + 1548;
        for (k = 0; k < 8; k++)
        {
          v0 += *((u16 *) ((q + 8) + (k * 12)));
        }

        D_80150DD8[i][15] = v1 >= 100000;
        D_80150DD8[i][16] = v1 >= 250000;
        D_80150DD8[i][17] = v1 >= 500000;
        D_80150DD8[i][18] = v1 >= 1000000;
        D_80150DD8[i][10] = v0 >= 100;
        D_80150DD8[i][11] = v0 >= 250;
        D_80150DD8[i][12] = v0 >= 500;
        D_80150DD8[i][13] = v0 >= 1000;
      }
    }

  }
  if (D_801164C2 == one)
  {
    for (i = 0; i < active_player_count; i++)
    {
      for (j = 0; j < 4; j++)
      {
        D_80150E88[i][j] = one;
      }

    }

    if (D_80156994 == 0)
    {
      D_80150E88[i][2] = 0;
    }
  }
  else
  {
    for (i = 0; i < active_player_count; i++)
    {
      if (input_rec0[i].x->y->z != 0)
      {
        S = input_rec0[i].x->y->z->S;
        for (j = 0; j < 4; j++)
        {
          D_80150E88[i][j] = 0;
        }

        D_80150E88[i][0] = one;
        new_var = S;
        if ((((*((u16 *) (new_var + 1674))) || (*((u16 *) (new_var + 1676)))) || (*((u16 *) (new_var + 1678)))) || D_80150F7C[1])
        {
          D_80150E88[i][1] = one;
          D_80150DD8[i][4] = one;
        }
        if (D_80156994 == 0)
        {
          if ((((*((u16 *) (new_var + 1702))) || (*((u16 *) (S + 1704)))) || (*((u16 *) (S + 1706)))) || D_80150F7C[3])
          {
            D_80150E88[i][3] = one;
          }
        }
        else
        {
          if ((((*((u16 *) (S + 1702))) || (*((u16 *) (S + 1704)))) || (*((u16 *) (S + 1706)))) || D_80150F7C[2])
          {
            D_80150E88[i][2] = one;
            D_80150DD8[i][5] = one;
          }
          if ((((*((u16 *) (S + 1730))) || (*((u16 *) (S + 1732)))) || (*((u16 *) (S + 1734)))) || D_80150F7C[3])
          {
            D_80150E88[i][3] = one;
          }
        }
      }
    }

  }
}
