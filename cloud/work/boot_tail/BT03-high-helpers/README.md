# BT03-high: six list/context and mixed-argument helpers

Six first-form strict O2 matches, **584 B / 146 words**. Independent paired
review and exact aggregate-head CI remain required. No cartridge coverage or
promotion is claimed.

- Branch `dot/boot-tail-bt03-high-helpers`, exact source base master
  `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Central activation `07997e6a`: `17540+104`, `18B3C+80`, `1D084+108`,
  `1D3F0+112`, `1D460+108`, `1D578+72`, all addresses prefixed `0x800`.
- Every target remains inside BT03-high and outside C11. Prior packets are
  frozen; no central status, claims, generated files or D10 is modified here.
  This branch has no source dependency on preceding matching drafts.
- Fresh protected manifests, all 439 starts/sizes totaling 99,120 B and the
  preexisting getter replay pass using unchanged pinned IDO. No protected input
  or compiler/scorer is edited.

## Native contracts

| Function | Bytes | Whole native operation |
|---|---:|---|
| `80017540` | 104 | Walk the current context list, save each next pointer before validating/removing the entry |
| `80018B3C` | 80 | If context is active, snapshot its active word, reset split state, then call two global-state helpers |
| `8001D084` | 108 | Unlink a real state node, clear upper flag bits and stop its valid identifier |
| `8001D3F0` | 112 | Forward nine genuine inputs to a ten-input constructor, deriving its extra tagged identifier |
| `8001D460` | 108 | Forward ten genuine inputs to the same constructor |
| `8001D578` | 72 | Walk a global state list while preserving next before calling its remover |

`17540` reads context+3964 as a pointer to entries with next/previous at +0/+4
and an identifier at +8. The real validator `201D0` returns -1 for invalid IDs;
`17470` mutates list links. Saving next before either call is essential behavior,
not a keeper. `18B3C` uses context words +3944/+3948/+3952/+3956; the repeated
context-pointer loads after stores are retained. `18A30` and `1897C` use globals
and require no incoming argument.

`1D084` and `1D578` share a minimum state-node prefix: aligned next/previous
pointers, flags at +8, and identifier at +52. The unlink operation updates the
appropriate neighbor or head `D_8004FD50`, retains only low16 flag bits, and calls
`1B8C4` only for a non-sentinel identifier. The walker saves next before the real
`1D4CC` call because that operation can unlink or otherwise mutate the current
node. No callee implementation is supplied in any candidate.

Unknown object-byte ranges are genuine unmodeled storage, not stack padding.
The layouts concern the N64 32-bit pointer ABI; wider-host pointer offsets are
not asserted. No sizeof-based array traversal depends on a guessed full node
extent. Original type/function names and complete layouts remain unproved.

## Genuine mixed float/integer O32 signatures

Both wrappers have three leading pointers, followed by two float values and a
word flag value. The first pointer is optional state storage; the next two
point to three-word records read by actual callee `8001D1F4`. They remain opaque
const pointers because a full record type is not needed by the wrapper.

The fourth float arrives in a3 under O32, not an extra floating-register input.
Native transfers a3 to f12 and back without arithmetic. The fifth float is at
incoming sp+16; its LWC1/SWC1 forwarding establishes that actual slot. The sixth
word is at incoming sp+20. This is ordinary O32, not IPA or a fabricated frame.

- `1D3F0` then takes an unsigned halfword at incoming sp+24 and byte values at
  sp+28/+32. It sends the halfword as argument7 and `identifier | 0x80000000` as
  argument8, followed by the two bytes as arguments9/10.
- `1D460` instead has two halfwords at incoming sp+24/+28 and two bytes at
  sp+32/+36, forwarding all ten arguments directly.

The callee's complete native entry reads every one of these slots: first state
pointer, two readable record pointers, two floating values, the flags word,
halfword identifier, full tagged word, and two bytes. It stores the floating
values in state and uses the bytes in floating arithmetic. Its return is a word
identifier or -1. Both wrappers return -1 if the runtime-enable byte is zero;
otherwise they forward the exact return word. The real ten-argument outgoing
area explains the 48-byte frame. No formal or stack slot was invented to fit it.
The unclaimed callee is declared only; none of its float constants or body is
copied, and no local-rodata proof is needed for these parameter-only wrappers.

## Flags, limits and replay

All six initial natural sources strictly match at
`-g0 -O2 -mips2 -G 0 -non_shared`; the scorer adds `-Wab,-r4300_mul` unchanged.
Their O1 controls fail:

- `17540`: 25/26 differing words
- `18B3C`: 20/20 plus two nonzero extras
- `1D084`: 26/27 plus six extras
- `1D3F0`: 28/28 plus four extras
- `1D460`: 27/27 plus four extras
- `1D578`: 16/18 plus one extra

No refinement, near-match diagnosis or flag sweep was needed. The selected
flags reproduce every argument home, frame, callee-save restoration, list reload,
floating move and complete native return path. All 146 accepted words are fully
relocated, with zero differing/extra words, unresolved symbols, unverified
relocations or errors. No fake argument, padding local, dummy call, keeper,
assembly or protected-tool change is used.

```sh
python3 cloud/work/boot_tail/BT03-high-helpers/verify.py
```

`verification.json` binds twelve final flag/control rows to exact source hashes;
`initial_controls.json` retains the seed results. `status_delta.csv` is the
central sole writer's six-row integration input. Only six submission files and
this packet directory change. No target, layout, symbol, lock, runtime image,
farm, forbidden helper, ROM/raw instruction dump, object, credential or production
gate is changed or published. Independent review and exact-head CI precede
checker-owned merging.

Independent paired review passed at source commit
`fd2eb5f1418013394dc9faf7706770d0cddda066`, tree
`8ca4328186c87f10b993cb5163a0620a3cc4e57b`. The reviewer independently reproduced
all twelve rows and source hashes and audited the complete bodies and genuine
callee contracts. `independent_review.json` records six strict matches / 584 bytes.
