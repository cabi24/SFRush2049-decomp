# BT07 medium packet

Five strict local matching C bodies, **632 B**, plus one complete **NONMATCH**
body, 152 B. No cartridge coverage or promotion is claimed. Independent replay
and aggregate-head CI remain required.

Base `301d9e7552ad4fd7f54a38796db84671e1000d35`; branch
`dot/boot-tail-bt07-medium`; central acknowledged claim `3f7c32b8`.
The coordinator alone edits STATUS, central claims, totals and D10. D10 remains
unchanged. This packet touches only the five matching sources and this folder.

## Results and optimization evidence

| Address | Size | Supported behavior | O2 | O1 control |
|---|---:|---|---|---|
| 800250AC | 68 | Create one-slot queue and insert initial null token | MATCH | MATCH |
| 800251A8 | 188 | Allocate conflict-free token, excluding selected record; skip sentinel | MATCH | 47/47 + 24 extra |
| 80025264 | 72 | Find active record with supplied token | MATCH | 18/18 + 10 extra |
| 80025EB0 | 196 | Initialize stream state, callback, buffer and queue | MATCH | 48/49 + 21 extra |
| 800262BC | 108 | Consume bounded available count and update countdown/state | MATCH | 24/27 + 11 extra |
| 80024FD4 | 152 | Callback accounts for two consumed ranges | NONMATCH 2/38 | 27/38 + 1 extra |

All matched O2 receipts have zero differing/extra words, unresolved or unverified
references, masks and relocation errors. The complete relocated words equal the
protected targets, and object trailing alignment is zero. The unchanged scorer
adds `-Wab,-r4300_mul`. O2 is supported by indexed induction/branch-likely scans,
shared loads, the minimum-value register flow and initializer scheduling. The
queue wrapper is identical under O1 and cannot independently identify the level.

The original pinned IDO toolchain was reused read-only from the already verified
BT07-small setup. Every file hash equaled that prior pinned receipt before work;
`verification.json` records current compiler, target, scorer and source identities.
The three manifest entries pass; all 439 extents/99,120 B match inventory; the
existing getter remains strict MATCH and earns no new credit.

## Native reconstruction and ABI

No reference implementation was imported. Field/function names remain hypotheses.
The partial `StreamState` is 4,648 bytes with directly observed offsets. Its
unknown arrays represent real object storage, never stack padding. The queue is
the existing 24-byte local message-header contract. Repeated native busy loads
and the previous shutdown/polling packet support a volatile busy byte. Typed
callback and buffer pointers refine the earlier byte-only partial view without
changing production headers or old locked sources.

- 250AC is called without inputs at 25C68+0x6C. Static names resolve exactly to
  osCreateMesgQueue at 80006A00 and osJamMesg at 800075E0, with the local header's
  three real arguments. D_800586A8 is the queue; D_800586C0 is its one-message store.
  Its callers ignore the return register, so the wrapper is reconstructed void.
- 251A8 is called at 252AC+0x200 with the actual selected index. It scans two
  records, uses the byte at 4608 and token word at 4644, excludes the selected
  record, skips 0xffffffff when incrementing the counter at D_80058680, and returns
  the token reloaded from the selected record. The caller consumes that return.
- 25264 receives a token in each of 25594, 255F0, 25670 and 259A8; their subsequent
  bounds/error paths consume its signed index/-1 return. The native loop has no
  direct or indirect callees, only the two real field reads per candidate.
- 25EB0 has five real inputs. 252AC+0x1D8 passes the record, aligned input buffer,
  output-buffer pointer, output count and func_8002506C callback; 25C68+0x4C passes
  the record followed by four null/zero inputs. The actual 25F74 consumer loads
  the callback from +400 and calls it with source pointer, destination pointer,
  count and the embedded queue pointer. The same consumer uses +408 as an output
  address and +412 as an unsigned count. bzero at 80008590 consumes pointer and
  byte length 400; osCreateMesgQueue takes record+4524, record+4548 and capacity 1.
  The real halfword->word->halfword zero-assignment chain reproduces the observed
  narrowing and store order. No extra operation or local was introduced.
- 262BC is called by 24FD4 with the record and the sum of two actual unsigned
  range counts. It accepts only state 3, decreases a positive signed countdown,
  transitions to state 4 at/below zero, then returns the unsigned minimum of the
  request and available-minus-consumed while advancing consumed. The state guard
  and native conditional expression recover the genuine common result flow.
- 24FD4 is an indirect callback, not a leaf inferred from absent jal callers.
  2574C+0x11C supplies its address as argument 9 to 1C580, which stores the callback
  at +4 and selected-record context at +20 of its genuine 24-byte registry record.
  1C3CC calls that stored callback at +0x80 and +0xCC with two pointer/count pairs
  and the context as its fifth machine argument. It tests the returned v0.
  Therefore the first and third unused pointer formals are genuine ABI inputs.
  D_8002D480's enable byte is volatile in the best candidate: 25C68 publishes it
  after initialization and 25DC0 clears it during shutdown; the asynchronous
  callback reads it before accounting. The qualifier spelling is reconstructed,
  not a claim to original source. It reproduces the native load/argument-home order.

## Bounded experiments and remaining nonmatch

The first queue wrapper and token search matched immediately. The initial
initializer was 2/49 different; workbench diagnosis identified two store-order
sites with unchanged geometry, and the genuine chained assignment order matched.
The countdown helper initially differed 23/27; diagnosis identified control-flow
geometry. Its real positive-state guard and minimum expression matched on the
next source form. The token allocator was 31/47 different; native indexed scanning
recovered all but two stores, and separating its actual token store from counter
increment matched. Five directed allocator forms were tried.

The callback's first form differed 36/38 at O2 and 28/38 at O1. Workbench identified
real entry/load/frame differences. Its volatile enable read closed every body
word except the stream-pointer save/reload: target stack+28, candidate stack+24.
Fifteen directed controls tested real global views, scope, request-count capture
and source expression choices. None closed the two homes naturally; extra consumed
locals moved the frame from 32 to 40, and unused/padding locals were not attempted.
Two broad alternative controls (a return local and by-value buffer pairs) were
rejected and never submitted; neither has sufficient native source evidence.
The complete best source is `func_80024FD4_NONMATCH.c`, outside the matches folder.
The next hypothesis needs authentic source-lifetime or local-home evidence, not
another formatting or fake-pressure sweep. Numeric experiments are archived without
native words, object files or raw disassembly.

## Reproduction and tests

Run after the unchanged normal setup (or set IDO_DIR to an existing pinned compiler):

```sh
python3 cloud/work/boot_tail/BT07-medium/verify.py
python3 cloud/work/boot_tail/BT07-medium/test_semantics.py
```

The host test compiles a 32-bit C89 layout assertion and links all five real match
translation units into a native-width ASan/UBSan harness. It checks field behavior,
queue arguments, preserved buffer bytes, 7,680 countdown/state/count combinations,
lookup, token conflicts, selected-index exclusion and sentinel wrap. The kernel
cannot execute i386, so runtime semantics use native pointer width; this is
explicitly separate from the fixed-compiler 32-bit strict proof. LeakSanitizer is
disabled under ptrace. Target code and ROM were not executed. Test stubs exist
only in the temporary host harness, never in submitted matching files.
