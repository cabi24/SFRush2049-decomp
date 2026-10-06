# Image B player aim easing: lean research

Target: `B:func_8038F568`, `[0x8038F568,0x8038F938)`, 976 bytes.
Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

## Observed result and reproduction

Standalone IDO 5.3 O3: **199/244 words differ**, zero extra words,
unresolved symbols, unverified relocations or errors with authenticated data.
The first complete reconstruction differed in 239/244 words. Ordinary
unsuffixed zero literals for the initial aim values and forward-depth test
change the zero-constant lifetime and improve the score. Integer-zero and
continue-guard alternatives were worse. This remains a broad NONMATCH with
allocation, addressing and schedule differences.

```
python3 cloud/work/lean/runtime_b_player_aim_20261006/reproduce.py
```

Actual flags: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
The helper authenticates the frozen asset/image B in memory and supplies
required own-data to the unchanged scorer. The three literals at
B:0x80394E8C..0x80394E94 are binary32 0.01. No native dump is included.

## Source and assumptions

Walks eligible opposing players, offsets the source's aim origin using its
kind/model tables, and transforms relative positions into the source frame.
The selection cone is half-depth horizontally and full-depth vertically.
The least forward depth within 2,000 wins, with equal-depth later candidates
allowed to replace earlier ones. Two real atan-like calls produce desired
angles; each stored angle moves by 0.01 toward its desired value. When no
candidate is selected, the desired values remain zero.

Native views preserve 952-byte players, 2,056-byte vehicles, player UV at
+44, and pitch/yaw fields at +944/+948. Kind-table stride is 156 bytes and
model vectors are 12 bytes. Helpers remain external with existing signatures.
Original names, declarations and whole-function arcade ancestry are unknown;
unknown record fields preserve actual storage.

Valid inputs require nonnegative backed player counts, valid source/player
indices, valid team/kind/model table entries and finite geometry. The tables
are external, so this is not a proof that every signed-byte kind or unsigned
model value is valid. No clamp or overshoot correction was invented for the
native fixed-step angle changes. No padding/pressure variables, fake callers,
artificial volatile or assembly. Only compile/scoring was performed.
Observed scores are not accepted coverage; checker owns further validation,
acceptance and integration.
