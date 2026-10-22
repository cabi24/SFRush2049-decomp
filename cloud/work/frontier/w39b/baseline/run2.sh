#!/bin/sh
# Baseline batch 2 (prior bests with their helper flags). Serial, tag w39b.
cd /home/cburnes/projects/rush2049-decomp
export TAG=w39b SCR=rush2049/scratch/frontier/w39b
T=cloud/work/frontier/tools/trace
F=cloud/work/frontier
run() { n=$1; f=$2; shift 2; echo "## $n $f UARGS=$*"; UARGS="$*" sh $T/us.sh $n $F/$f 2>&1 | tail -1; }
CT="--internal ct_CenterText --internal ct_RightText --internal ct_font_set --internal ct_gfx_lock --internal ct_gfx_unlock --internal ct_SetFont --internal controls_animButtonText"
VS="--internal vs_CenterText --internal vs_RightText --internal vs_font_set --internal vs_drawColorBars --internal vs_gfx_lock --internal vs_gfx_unlock --internal vs_SetFont --internal vs_SetJust --internal func_8010EA08"
run controls_animText w36k/anim/best.c $CT
run controls_animButtonText w36k/anim/v/b3.c $CT
run func_8010F218 w33b/videoscr/best.c $VS
run func_8010EA08 w33b/videoscr/best.c $VS
run display_list_flush w36p/display_list_flush/best.c --internal InitBattlePoints
run pak_delete_file_retry w36v/pak_delete_file_retry/best.c --internal func_800A1E94 --internal paksys_queue_init --internal paksys_lock --internal paksys_unlock --internal pdf_heap_lock --internal pdf_heap_unlock --internal pdf_buffer_free --internal pdf_free_blocks --internal pdf_release
run track_render_process w37a/groups/w37a_paksys/group.c --internal func_800A1E94 --internal no_catchup --internal paksys_queue_init --internal paksys_lock --internal paksys_unlock --internal pfs_get_node --internal func_800A3640 --internal func_800A3724 --internal _FileChanged --internal _StopRumble
