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
