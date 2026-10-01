# Vendored copy of n64-decomp-workbench

- source: https://github.com/akratch/n64-decomp-workbench
- commit: 3f58a68db5d4cf343c76b8361dfc1786c501e521
- commit date: 2026-09-09T09:14:41+02:00
- subject: Record the measured fix for the address-scoring defect: mask the union
- synced: 2026-10-01T13:47:51Z
- licence: CC0-1.0 (LICENSE.md)

Kept: the `decomp_workbench` package, markdown docs (no images or history), `examples/fixtures`,
README, LICENSE, pyproject. Not kept: tests, research archive, case studies, release tooling.
Update with `python3 tools/vendor_workbench.py sync` and review the diff. Do not edit these files
by hand: local findings go in `cloud/PLAYBOOK.md` and `docs/`.

Run without installing: `PYTHONPATH=third_party/n64-decomp-workbench/src python3 -m decomp_workbench ...`
(Python 3.10+; real object comparison needs a GNU MIPS objdump).
