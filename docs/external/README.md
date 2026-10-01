# External reference documents

Third-party notes that help with matching, kept as local reference copies. The copies are **not committed**
(`docs/external/files/` is git-ignored): a source without a licence is read and linked, not republished.
Cloud agents, which see only GitHub, use the URLs below.

| Document | Source | What it is |
|---|---|---|
| `files/snowboardkids-DECOMPILATION_LEARNINGS.md` | [cdlewis/snowboardkids-decomp](https://github.com/cdlewis/snowboardkids-decomp/blob/main/DECOMPILATION_LEARNINGS.md) | Generic IDO 5.3 behaviour and matching patterns: how `uopt` colours registers (measured with an instrumented build), stack-frame arithmetic and the `-g3` trick, loop unrolling rules, struct/global access, workflow. No licence declared upstream. |

Read it before working a register-allocation or frame-size residual. Caveats: it was measured at `-O2 -mips1`
(we use `-mips2` and `-O3` whole-program groups), and parts are specific to that project (GBI macros, its build
tooling). Check a claim on our pipeline before relying on it; matching items go into `cloud/PLAYBOOK.md`.

## Vendored: n64-decomp-workbench (CC0)

[akratch/n64-decomp-workbench](https://github.com/akratch/n64-decomp-workbench) is public domain (CC0-1.0), so it
**is** committed, under `third_party/n64-decomp-workbench/` (package, markdown docs, fixtures; pinned commit in its
`UPSTREAM.md`). It diagnoses a late-stage mismatch (`schedule` / `allocation` / `frame` / `structure` / `constant` /
`phase-shift`), splits registers into the allocator's coloured pool and ugen's temp ring, and names the lever.
Its field guide (`docs/field-guide.md`) and IDO 5.3 compiler laws (`docs/compiler-laws/ido-5.3.md`) are the best
reference we have for register-allocation and frame-size residuals.

```bash
python3 tools/workbench.py diagnose TARGET.o CANDIDATE.o --function FN --objdump mips-linux-gnu-objdump
python3 tools/workbench.py guide                 # topics; `guide 15` = one lever; `guide laws ido53 L64`
python3 tools/vendor_workbench.py status         # pinned commit vs upstream (also logged daily)
python3 tools/vendor_workbench.py sync           # update the vendored copy; review the diff
```

Run `diagnose` on a near-miss **before** hand-editing: it says whether the residual is scheduling, frame size or the
temp ring and which lever to try. Needs Python 3.10+ and a GNU MIPS objdump (on the nodes use the toolkit's
`bin/objdump` with `LD_LIBRARY_PATH` set to the toolkit's `lib`). Its ownership labels are heuristic and most of its
measurements come from other games: check a lever on our flags before relying on it.

## Keeping the copy current

```bash
python3 tools/external_docs.py fetch      # download (or refresh) every document in SOURCES.json
python3 tools/external_docs.py status     # when each copy was fetched and which upstream commit it is
```

A fresh clone has no copy until `fetch` runs. On the Pi a systemd timer (`external-docs.timer`) runs `fetch`
daily. When a document changes, the previous copy is kept as `<name>.prev` and the change is logged
(`journalctl -u external-docs`). A failed download never replaces a good copy.

To add a source, add an entry to `SOURCES.json` (name, file, raw URL, upstream repo and path, licence note) and
list it in the table above.
