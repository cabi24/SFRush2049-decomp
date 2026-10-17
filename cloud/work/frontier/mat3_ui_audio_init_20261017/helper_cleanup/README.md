# Supporting MBOX helper cleanup

This evidence does not replace the 48-body audio baseline or change its claims.
All three native helpers were already locked. New accepted bytes remain zero.

The complete MBOX_HideObject and MBOX_ShowObject bodies use their actual
GetChild, GetNext, ObjectFlags and SetObjectFlags boundaries. They reproduce
all 93 and 101 native words without the older reconstructions' compiled-out
negative-index diagnostic. The genuine ZOID_SetTexture body reproduces all
26 native words without the older unreachable block. Before-source hashes and
strict results are retained. Moving all four accessor definitions ahead of the
visibility routines preserves strict equality; the result does not depend on
that definition order.

The later mbox and zoidtexture TUs are recorded Matching. Original DWARF
corroborates three int visibility parameters, unsigned-int flags and int tmpidx
locals; the flag getter takes signed short, whereas its setter takes int.
GetChild/GetNext return int from signed-short fields. The texture setter takes
signed short, texture pointer and int, with the sole object-pointer local.
This is saved source and original type evidence, not a fresh later-platform
compiler result or proof of historical N64 spelling. The duplicated mode-zero
arm is retained from the saved source; its unreachable syntax cannot be
recovered from eliminated native code.

The N64 object stride is 0x44, with flags at zero, child at 0x16, next at 0x18
and four texture slots at 0x20..0x2c. Opaque gaps preserve the actual native
record offsets; they add no automatic storage. This view differs from the
later platform. The four source accessors emit eight-byte inlining stubs;
there is no authenticated separate native symbol identity for those stubs.
All seven emitted symbols are inventoried, and all three known native bodies
are explicit members. No dummy caller, register annotation or extra local is
added.

Source evidence SHA256s: mbox.c
8cbe7ecbc8b1ff1a373c73aeef46b3f6e80a8e6b01ad520858970084d238690e;
zoidtexture.c
3c6c46b38ab700bf405e32bbbce3c5fabcb5ea7ed3e47799b2c293b883cebe43.
Original visibility DIEs are 0x8871d/0x887f2; texture setter is 0x1375cc.

The supporting proof independently rebuilds each locked before-source, the
natural group, a definition-order control, five source negatives and the real
audio graph before/after addition. All 48 prior receipts remain identical;
the expanded graph has 51 known members with the same six failures. It also
pins source, proof, target, comparison tools and old source files. Explicit
exceptions remain active under Python -O. Stock and observation-only traced
objects are identical. No native words, raw assembly, object or trace dump is
included. This is neither canonical installation nor complete-image proof.

Five negatives change the show mask, hide flag, child field, flag getter width
and texture slot. All reject. The getter-width control in particular shows
why the authentic signed-short read versus int write boundary matters.

A separate original-six-local updateButtons investigation uses the same
helpers. It preserves D6914's remaining partial result and frame gap. Native
texture lookup and setter sites are out-of-line calls; the later lookup's
48-byte stack record is not evidence for adding 48 bytes to the updater.
