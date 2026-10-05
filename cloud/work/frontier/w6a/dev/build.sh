#!/bin/sh
# build.sh OUT.c [extra parts...]: assemble the knot source from the parts
D=$(dirname $0)
out=$1; shift
{
echo '/* flags: -g0 -O3 -mips2 -G 0 -non_shared */'
cat $D/header.txt $D/part_a.h $D/part_b.h
echo 's32 particle_system(Gfx **gp, Node *parent, s32 count, s32 flag, Node *node, s32 view, Palette *pal);'
echo 's32 track_collision_wall(Gfx **gp, Node *parent, Node *node, s32 view, s32 count, s32 more_in, s32 flags6, Palette *pal);'
echo 'f32 func_8009C3F8(f32 input, s32 arg0);'
echo 'void render_display_list(Gfx **gp, Texture *tex, TexRect *rect, TexRect *clip, Palette *palettes);'
echo 'void func_80099B30(Gfx **gfxp, Palette *tex);'
cat $D/part_rdl.c $D/part_asin.c $D/part_ps.c $D/part_tw.c $D/part_f058.c "$@"
} > $out
