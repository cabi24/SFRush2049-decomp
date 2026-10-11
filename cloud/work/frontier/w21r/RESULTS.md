# w21r: recomp Phase 1 (boot) — tooling lane, worktree branch w21r

Status: done (2026-10-11). Not a matching lane: no src/, asm/, lock or coverage change.

## Result

The recompiled game boots headless on rocky: entrypoint -> threads -> game image -> image A front
end main loop (30 frames/s of display lists, 60 Hz VI, audio tasks, asset inflates; stable 90 s).
With scripted START/A input, image B (race) loads at 23 s and display lists grow to ~14 KB;
stable 120 s. Null renderer: nothing rasterised (RT64 not built on headless rocky).
Recompiled inflate verified: images A/B bit-exact, game image 161761/161768 words (7 data words
already written by the game). Details: tools/recomp/BOOT_LOG.md, plan §9.

## Files (branch w21r)

- tools/recomp/runtime/ (CMakeLists.txt, check_contract.py, src/{main,null_renderer,register_overlays,hooks,shims,trace}.cpp, rush_runtime.h)
- tools/recomp/identity.json (43 recompiler-only libultra renames with evidence; audit vs ultralib VERSION_J)
- tools/recomp/gen_config.py: hooks, identity renames, runtime_contract, rush2049_images.h
- tools/recomp/patches.json: 2 hooks, osStopThread_rx ignored, osInvalICache_full stub removed (now osInvalICache)
- tools/recomp/{README,VERSIONS,BOOT_LOG}.md; tests/recomp/test_runtime_contract.py (17 fixture tests)
- docs/plans/2026-10-12-recomp-port-plan.md §9

## Integration notes

Merge branch w21r as is. Nothing generated is committed. identity.json renames apply to the
recompiler's symbol file only; the inventory names it corrects (e.g. osSendMesg/osJamMesg swapped)
are worth fixing in the project's symbol files separately (owner decision; a misattribution doc
exists for dll_*: docs/SYMBOL_MISATTRIBUTION.md).
