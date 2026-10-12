# Independent native text-consumer audit

Current repository master 84d8a4827e7fed06365846e013351b580593d921.
This is an input-pointer provenance audit, not an original-capacity or general
memory-safety claim. Native complete target hashes and extents are in the adjacent
JSON file; no native bytes or assembly dumps are included.

The A7B58 temporary text is formatted once and then consumed by the width wrapper
sound_pitch_diff_halved and the renderer wrapper state_utility. Completeness of
that outer address-use census remains the packet author's obligation.

- sound_pitch_diff_halved (800B71A0, 52 B) forwards the string to
  object_manager_update (800B3FA4, 532 B), then computes a horizontal position.
  No direct input-pointer store or persistent escape. The accepted current
  object_manager_update source in codex_sound_channel_extra reads the string
  through an advancing scan pointer; it does not write it. Its other helper
  sound_update_channel refreshes the font globals and is not passed the string.
- state_utility (800B71D4, 396 B) stores its string argument only into its own
  stack parameter home. Optional width branches pass it to the same width
  routine. Its remaining string-consuming call at +0x174 is menu_input_process
  (800B6BEC). Other stores are its stack or independently addressed renderer
  coordinate globals. It does not overwrite the caller's string.
- menu_input_process (800B6BEC, 1280 B) saves the input in its own stack home.
  Its direct stores are own-stack or independently addressed global renderer
  state/queue. On the immediate-render path, it first measures bytes via
  func_800B66B0 at +0x48, then passes the string in private s2 to
  audio_doppler_calc at +0x54. On the queued path it may copy the input to its
  own sp+112 via func_800A473C (+0x84) or func_80092DCC (+0x98), measures via
  B66B0 (+0x1e8), then copies it as source a1 at +0x49c using memcpy. That
  destination comes from the D_80159DD8 queue plus its count, not the input.
  The +0x444 memcpy handles an unrelated four-byte global field.
- func_800B66B0 (152 B) is a leaf byte-count scan. Its only store is the other
  argument to its own sp+4 home. All pointer-derived accesses are byte loads.
- func_800A473C (52 B) and func_80092DCC (96 B) are leaf copy loops: the source
  a1 and its derived cursor are only read. Stores derive solely from destination
  a0, the caller's local sp+112 here. The bounded copy's zero fill likewise
  writes only its destination.
- audio_doppler_calc (800B6788, 1124 B) receives the string in private s2.
  Its byte-order prefix read is +0x170; it selects t2 as input or input+1,
  then replaces s2 with a global X coordinate. Within the glyph loop the only
  input-derived memory accesses are byte loads at +0x208/+0x20c/+0x220.
  Input-derived addresses are not store bases. The t2 cursor is spilled to its
  own sp+0x4c and restored around texture/rectangle calls; those calls receive
  texture addresses/glyph numbers/coordinates, not the input pointer. No input
  pointer is published into a global or queued for later writes.
- memcpy (80007C68, 44 B) is the current tracked eleven-word static byte-copy
  loop. Source a1 is copied to v1 and only read; stores advance the destination
  v0 derived from a0. Twelve following alignment bytes are excluded from its
  helper hash. This current tracked target is separately identified; no new ROM
  authentication or static TU build is claimed.

Conclusion: no additional writer or persistent input-pointer escape was found
on the complete consumer chain. This establishes read-only consumers under the
ordinary backed/disjoint renderer-global and synchronous helper contracts. It
is not a guarantee against arbitrary global aliases into the caller's stack,
asynchronous mutation, a broken helper ABI, invalid strings or corrupted global
state. The writer-bound proof must still bind the formatter, sole outer writer,
six native language labels and full native stack allocation independently.
