#!/bin/sh
# mk.sh DIR [extra parts] -- assemble a stand-in-free group (parts + Input_ProcessGameplayPad) into DIR
D=$(dirname $0); out=$1; shift
mkdir -p $out
sh $D/build.sh $out/group.c ${IPGP:-$D/part_ipgp.c} $D/tail.c "$@"
cat > $out/group.json <<J
{
 "members": ["render_display_list", "func_80099B30", "func_8009C3F8", "camera_update_c", "select_screen_update",
             "particle_system", "track_collision_wall", "func_8009F058", "Input_ProcessGameplayPad"],
 "files": ["group.c"],
 "keep": ["camera_update_c", "select_screen_update", "Input_ProcessGameplayPad"],
 "flags": "-g0 -O3 -mips2 -G 0 -non_shared",
 "unprototyped": [],
 "context": [],
 "claims": []
}
J
