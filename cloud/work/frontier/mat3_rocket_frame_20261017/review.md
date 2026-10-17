# Independent research review, 2026-10-17

Approved for research-only draft publication after independent replay:

- Strict verification reproduced Rocket 47/671 stack/frame-only differences,
  wrong-alpha control 48/671, Frame 537/820, empty claims and zero new bytes.
- Both host palette tests passed.
- Complete stock/traced objects were identical, hash e273b9a3 (full hash in
  trace_fidelity.json and verification.json).
- Native Frame accesses independently confirmed four halfword samples at
  0x344/0x346/0x348/0x34A, current IR word at 0x34C, and previous IR store at 0x350.
- Context differences and all nonmatch/integration limits remain explicit.

No exact-match, runtime, image, or ROM gate is implied by this review.

## Follow-up guard review

The independent renderer lane reviewed the explicit exception replacements in
verify.py and trace_fidelity.py and replayed both focused pytest cases. Both
passed, including optimized-Python rejection of false claims, unverified
references, and changed source receipts. Source/group files and the compiled
object are unchanged. No blocker to updating the same research-only draft.

The follow-up trace-receipt consistency fix was independently reviewed and both
pytest cases replayed again. The verifier now enforces the trace input and paired
object hashes; optimized-Python negatives reject changes to either binding.
