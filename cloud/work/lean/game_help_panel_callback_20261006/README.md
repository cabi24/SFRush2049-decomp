# LEAN RESEARCH: help-text panel callback (8010A53C)

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`. Native extent: 616 bytes / 154 words, frame 64.

The complete typed callback derives its hidden state from the actual flags, chooses one of four corner placements, renders the topic heading, then draws its help lines with native spacing. It retains the actual callback and font-service interfaces.

## Baseline and observed compilation

The original frozen generated seed **does not compile**: it dereferences an integer text-table address. It receives no match score. A minimal control adds only a `void **` cast before that existing dereference; no body behavior repair is otherwise applied to this control.

At IDO 5.3 `-g0 -O3 -mips2 -G 0 -non_shared`, automatic `-Wab,-r4300_mul`:

- Cast-only repaired seed: **148/154 differing words**, three excess, 628-byte function, frame 56.
- Complete typed body standalone: **127/154**, zero excess, 600 bytes/frame 56.
- Same typed body with genuine font context: **127/154**, zero excess, **608 bytes/frame 64**.

The real-context candidate is retained. It restores the native frame naturally, but remains eight bytes short and nonmatching. The target has no unresolved/unverified sites or scorer errors. Context members remain unclaimed and their scores are recorded in `observed.json`.

## Corrected body and real context

Baseline source: frozen `cloud/work/registered-heads/seeds/func_8010A53C/group.c`, cut before its helper definition to discard the generated helper and fake stand-in. The original has byte reads where native loads text pointers, a literal-address case artifact, and premature signed-halfword y truncation. The candidate uses actual typed symbol references, full-width position arithmetic and signed-halfword narrowing only at the text call.

Native layout views are explicit: Blit hidden byte +0x1A; the text-root bank/string pointers at D_8017A4E0+12/+16; unsigned topic-base halfword at bank+0; and D_80114A0C as eleven text pointers per topic (44-byte stride). These express observed offsets, not recovered original type names. The header/string index and help-line count must remain within their actual tables; valid Blit/service pointers are assumed. No fallback outside the native valid runtime domain is invented.

The returning font wrapper is the genuine gfx-lock/font-select/unlock abstraction used by existing source. `slot_context.c` is the real frozen credits-scroll helper, normalized as in PR #215. `countdown_caller.c` is the unchanged actual callback from corrected PR #220, providing real additional font call sites. No dummy caller, unsupported volatile local, dead read or padding storage is introduced. Full font/sound-update IPA context remains unresolved.

## Minimal replay

With project IDO and MIPS tools configured:

```
python3 cloud/work/lean/game_help_panel_callback_20261006/replay.py --repo .
```

The script reads frozen tools, baseline and protected target/manifests into temporary storage, applies the documented one-cast baseline repair, builds the controls and reports metadata. No behavioral or acceptance checks, live-source changes, locks, splice claims or ROM integration are added. Independent verification, acceptance and merging belong to the checker.
