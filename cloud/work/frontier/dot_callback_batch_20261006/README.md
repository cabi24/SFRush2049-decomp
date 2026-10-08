# Additive main-game callback research batch

## Result

Two complete semantic reconstructions are staged without accepting any native
bytes. Both remain **NONMATCH**:

- EF288 gear-label callback: 788 candidate bytes, 808 native bytes, 179 differing
  words. Native selector input is s2, while this compiled semantic context uses
  s1. Native private-helper composition and the original translation unit are
  not established.
- FA9B4 viewport/effect root: 912 candidate bytes, 924 native bytes, 227/231
  differing positional words. Runtime image B must already be resident. External
  helpers remain bounded contract hooks, not complete callee executions.

The detailed source/ABI/fixture/domain limits in both packet and independent
review READMEs remain controlling. No strict match, original-source identity,
whole-game equivalence, accepted bytes or ROM coverage is claimed. The two
independent fixture reviews reuse the producer interpreters; they are not second
independent CPU implementations.

Base production context is read at commit
`6b2e9e506fe3d2267a710e41c85af5364ccd00c7`. This is a separate, additive,
source-only tree; it does not modify the previously tested 345-file tree or
`rush-sixth-baseline`. It contains only authored C, Python, JSON and Markdown,
plus focused tests. No ROM bytes, target assembly dumps, ELF/native binaries,
raw logs or caches are deliverables.

## Verified results

- Focused toolchain-present suite: **13 passed**.
- IDO absent: **7 passed, 6 skipped**.
- MIPS GNU linker absent: **7 passed, 6 skipped**.
- Gear: full 2,257-case producer replay, independent 7,281-case fixture trace,
  all 202 native words visited, 8 negative controls, 10,000 ASan/UBSan host cases.
- Viewport: both 10,000-case producer receipts, independent 2,548-case repeated-
  callback review, 16 source mutants, native/object eight-byte stack/alignment
  checks, 10,000 ASan/UBSan host cases and independent full-suite UBSan replay.
- 17 additional service contracts, 12 historical image/target identities and
  both wrong-image controls reproduce unchanged.

All 9 staged C files are byte-identical to their source packets. The complete
generated gear proof differs only in the intentional verifier wrapper hash.
All other replayed JSON proof fields remain exactly equal to their originals;
the gear independent receipt retains its additional manually recorded historical
context metadata. The gear reviewed interpreter is preserved as authored: its
selector scratch list differs from the producer snapshot, but the independent
review overrides that hook. Neither file was homogenized.

`bindings.json` pins the actual 24 authored C/Python/group files and 36 selected
native target identities. `verify_packet.py` reloads the unchanged canonical
scorer separately for each image population. It neither hashes nor modifies the
scorer, live manifests, symbols, locks, production source or test files.
`validation.json` records source preservation, exact wrapper identity changes,
semantic equality and focused test/sanitizer summaries. The file list is in
`file-manifest.json`; it is an inventory, not a hash pin on tests.

## Portable reproduction

Use a repository containing the base commit in full history. For a source-only
staging tree, the tool/target root and Git-history root may be separate. Set a
writable absolute TMPDIR outside the staged source tree:

```sh
export TMPDIR=/absolute/writable/workspace/tmp
mkdir -p "$TMPDIR"
export PYTHONDONTWRITEBYTECODE=1
export RUSH_TOOL_ROOT=/absolute/unchanged/tool-and-target-root
export RUSH_REFERENCE_ROOT=/absolute/history-root
# Source rush-recovery-env.sh when using the recovery installation.
# In an integrated repository the two roots default to that repository.
python3 -m pytest /absolute/staged/tree/tests/cloud -q -p no:cacheprovider
```

For the absent-IDO check, point IDO_DIR to a nonexistent directory. For the
absent-linker check, keep pinned IDO and use a PATH without mips-linux-gnu-ld.
Both runs still exercise host/native and independent gear cases. Compiler-
dependent fixtures guard pinned IDO and the MIPS GNU linker before any native
compile, and skip cleanly when either is absent.

`RUSH_VIEWPORT_OBJECT` may select an existing canonical O3 object for the viewport
checks. Without it, guarded tests reproduce the frozen source through the
unchanged canonical scorer, including mandatory R4300 backend flags. These are
replays of frozen baselines, not additional tuning or flag experiments. Raw-cc
substitution, protected tool changes, live acceptance assertions and mutable
context hashes are not used.

Review output defaults to external TMPDIR work directories. Optional
`RUSH_GEAR_REVIEW_WORK`, `RUSH_VIEWPORT_REVIEW_WORK`, `RUSH_REVIEW_OUTPUT` and
`REVIEW_HOST_LIBRARY` select separate work/output/sanitized-library locations.
The viewport review requires an existing object and never compiles a target.
Its host builder now verifies archived C bytes instead of rewriting them.

These are focused checks only. Before any future publication, independently
confirm authorization, current-master integration, and the full requested
`tests/conveyor tests/cloud -m "not node_required"` present/absent-toolchain
matrix. No external writes, protected edits, publication or CI watching were
performed in this staging lane. Publication remains paused.

## Exact path-wrapper changes

The viewport focused test was relocated from its packet-local test filename to
`tests/cloud/test_viewport_effect_fa9b4.py`. Its sole existing-code change is:

```diff
-HERE=Path(__file__).resolve().parent
+ROOT=Path(__file__).resolve().parents[2]
+HERE=ROOT/'cloud/work/frontier/dot_viewport_effect_fa9b4_20261006'
```

The newly added binding/review focused test is new authored test coverage and
has no receipt hash. All other changes to existing Python source are below.
They select repo-relative or explicitly configured tool/history/packet roots,
keep generated work outside the source packet, and verify existing frozen C
rather than rewriting it. No machine, callback, fixture, mutation, native
comparison or reconstructed C semantics changed.

```diff
--- original/cloud/work/frontier/dot_gear_label_ef288_20261006/verify.py
+++ staged/cloud/work/frontier/dot_gear_label_ef288_20261006/verify.py
@@ -19 +19,2 @@
-sys.path.insert(0,str(ROOT))
+TOOL_ROOT=Path(os.environ.get('RUSH_TOOL_ROOT',ROOT)).resolve()
+sys.path.insert(0,str(TOOL_ROOT))
@@ -125 +126 @@
-    repo=Path(os.environ.get('RUSH_GIT_REPO',ROOT))
+    repo=Path(os.environ.get('RUSH_REFERENCE_ROOT',os.environ.get('RUSH_GIT_REPO',ROOT)))
--- original/cloud/work/frontier/dot_gear_label_ef288_20261006/review/audit.py
+++ staged/cloud/work/frontier/dot_gear_label_ef288_20261006/review/audit.py
@@ -7 +7 @@
-import argparse,ctypes,hashlib,json,pathlib,random,sys
+import argparse,ctypes,hashlib,json,os,pathlib,random,sys,tempfile
@@ -9,0 +10,4 @@
+WORK=pathlib.Path(os.environ.get('RUSH_GEAR_REVIEW_WORK',pathlib.Path(tempfile.gettempdir())/'gear-source-review')).resolve()
+WORK.mkdir(parents=True,exist_ok=True)
+OUTPUT=pathlib.Path(os.environ.get('RUSH_REVIEW_OUTPUT',WORK)).resolve()
+OUTPUT.mkdir(parents=True,exist_ok=True)
@@ -108 +112 @@
- lib=ctypes.CDLL(str(P/'tmp/host_audit.so'));lib.run_audit.argtypes=[ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_int)];lib.run_audit.restype=ctypes.c_int
+ lib=ctypes.CDLL(str(WORK/'host_audit.so'));lib.run_audit.argtypes=[ctypes.POINTER(ctypes.c_int),ctypes.POINTER(ctypes.c_int)];lib.run_audit.restype=ctypes.c_int
@@ -119 +123 @@
- (P/'review.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
+ (OUTPUT/'review.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
--- original/cloud/work/frontier/dot_gear_label_ef288_20261006/review/negative.py
+++ staged/cloud/work/frontier/dot_gear_label_ef288_20261006/review/negative.py
@@ -22,2 +22,2 @@
- d=P/'tmp'/name;d.mkdir(exist_ok=True);(d/'candidate.c').write_text(wrong);(d/'host_audit.c').write_text(fixture)
- env=dict(os.environ,TMPDIR=str(P/'tmp'))
+ d=audit.WORK/name;d.mkdir(exist_ok=True);(d/'candidate.c').write_text(wrong);(d/'host_audit.c').write_text(fixture)
+ env=dict(os.environ,TMPDIR=str(audit.WORK))
@@ -41 +41 @@
-(P/'negative_controls.json').write_text(json.dumps(results,indent=2)+'\n');print(json.dumps(results,indent=2))
+(audit.OUTPUT/'negative_controls.json').write_text(json.dumps(results,indent=2)+'\n');print(json.dumps(results,indent=2))
--- original/cloud/work/frontier/dot_viewport_effect_fa9b4_20261006/review/build_review_host.py
+++ staged/cloud/work/frontier/dot_viewport_effect_fa9b4_20261006/review/build_review_host.py
@@ -2,3 +2,5 @@
-import re,subprocess,os,hashlib
-HERE=Path(__file__).resolve().parent; ROOT=HERE.parent
-PACKET=Path(os.environ.get('RUSH_PACKET_ROOT',str(ROOT/'viewport-effect-root/packet'))).resolve()
+import re,subprocess,os,hashlib,tempfile
+HERE=Path(__file__).resolve().parent
+WORK=Path(os.environ.get('RUSH_VIEWPORT_REVIEW_WORK',Path(tempfile.gettempdir())/'viewport-source-review')).resolve()
+WORK.mkdir(parents=True,exist_ok=True)
+PACKET=Path(os.environ.get('RUSH_PACKET_ROOT',str(HERE.parent))).resolve()
@@ -7 +9 @@
-(HERE/'frozen_candidate.c').write_bytes(source)
+assert (HERE/'frozen_candidate.c').read_bytes()==source
@@ -11 +13 @@
-(HERE/'host_review.c').write_text(host)
+assert (HERE/'host_review.c').read_text()==host
@@ -13 +15 @@
- subprocess.run(['gcc','-std=c99','-O2','-Wall','-Wextra','-Werror','-fPIC','-shared',f'-DCANDIDATE_FILE="{src}"',str(HERE/'host_review.c'),'-o',str(HERE/'tmp'/name)],check=True,env=dict(os.environ,TMPDIR=str(HERE/'tmp')))
+ subprocess.run(['gcc','-std=c99','-O2','-Wall','-Wextra','-Werror','-fPIC','-shared',f'-DCANDIDATE_FILE="{src}"',str(HERE/'host_review.c'),'-o',str(WORK/name)],check=True,env=dict(os.environ,TMPDIR=str(WORK)))
--- original/cloud/work/frontier/dot_viewport_effect_fa9b4_20261006/review/mutations.py
+++ staged/cloud/work/frontier/dot_viewport_effect_fa9b4_20261006/review/mutations.py
@@ -28,3 +28,3 @@
- src=r.HERE/'tmp'/('mutant-'+str(index)+'.c');src.write_text(mutant)
- lib=r.HERE/'tmp'/('mutant-'+str(index)+'.so')
- subprocess.run(['gcc','-std=c99','-O2','-fPIC','-shared',f'-DCANDIDATE_FILE="{src}"',str(r.HERE/'host_review.c'),'-o',str(lib)],check=True,env=dict(os.environ,TMPDIR=str(r.HERE/'tmp')))
+ src=r.WORK/('mutant-'+str(index)+'.c');src.write_text(mutant)
+ lib=r.WORK/('mutant-'+str(index)+'.so')
+ subprocess.run(['gcc','-std=c99','-O2','-fPIC','-shared',f'-DCANDIDATE_FILE="{src}"',str(r.HERE/'host_review.c'),'-o',str(lib)],check=True,env=dict(os.environ,TMPDIR=str(r.WORK)))
@@ -41 +41 @@
-(r.HERE/'mutations.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
+(r.OUTPUT/'mutations.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
--- original/cloud/work/frontier/dot_viewport_effect_fa9b4_20261006/review/review.py
+++ staged/cloud/work/frontier/dot_viewport_effect_fa9b4_20261006/review/review.py
@@ -7 +7 @@
-import ctypes as C, hashlib, importlib.util, json, os, random, struct, sys
+import ctypes as C, hashlib, importlib.util, json, os, random, struct, sys, tempfile
@@ -9,4 +9,8 @@
-HERE=Path(__file__).resolve().parent; ROOT=HERE.parent
-TOOL=Path(os.environ.get('RUSH_TOOL_ROOT',str(ROOT/'viewport-effect-root'))).resolve()
-PACKET=Path(os.environ.get('RUSH_PACKET_ROOT',str(TOOL/'packet'))).resolve()
-REFERENCE=Path(os.environ.get('RUSH_REFERENCE_ROOT',str(ROOT/'rush-recovery'))).resolve()
+HERE=Path(__file__).resolve().parent; ROOT=HERE.parents[4]
+WORK=Path(os.environ.get('RUSH_VIEWPORT_REVIEW_WORK',Path(tempfile.gettempdir())/'viewport-source-review')).resolve()
+WORK.mkdir(parents=True,exist_ok=True)
+OUTPUT=Path(os.environ.get('RUSH_REVIEW_OUTPUT',WORK)).resolve()
+OUTPUT.mkdir(parents=True,exist_ok=True)
+TOOL=Path(os.environ.get('RUSH_TOOL_ROOT',str(ROOT))).resolve()
+PACKET=Path(os.environ.get('RUSH_PACKET_ROOT',str(HERE.parent))).resolve()
+REFERENCE=Path(os.environ.get('RUSH_REFERENCE_ROOT',str(TOOL))).resolve()
@@ -15 +19 @@
-obj=Path(os.environ.get('RUSH_VIEWPORT_OBJECT',str(TOOL/'tmp/candidate.o'))).resolve();raw=score.text_words(obj);compiled,masks,unresolved,unverified,errors=score.relocate(obj,raw,0,len(raw)*4,score.image_symbols())
+obj=Path(os.environ.get('RUSH_VIEWPORT_OBJECT',str(WORK/'candidate.o'))).resolve();raw=score.text_words(obj);compiled,masks,unresolved,unverified,errors=score.relocate(obj,raw,0,len(raw)*4,score.image_symbols())
@@ -145 +149 @@
- host=Host(Path(os.environ.get('REVIEW_HOST_LIBRARY',str(HERE/'tmp/review.so'))));ids=set();coverage=set();branches=set();multi=0;n=0;actions=0
+ host=Host(Path(os.environ.get('REVIEW_HOST_LIBRARY',str(WORK/'review.so'))));ids=set();coverage=set();branches=set();multi=0;n=0;actions=0
@@ -162 +166 @@
- (HERE/'review.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
+ (OUTPUT/'review.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
--- original/cloud/work/frontier/dot_viewport_effect_fa9b4_20261006/review/static_review.py
+++ staged/cloud/work/frontier/dot_viewport_effect_fa9b4_20261006/review/static_review.py
@@ -29 +29 @@
-(r.HERE/'static-review.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
+(r.OUTPUT/'static-review.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
```
