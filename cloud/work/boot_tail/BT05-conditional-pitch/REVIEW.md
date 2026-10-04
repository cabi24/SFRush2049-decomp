# Independent review

PASS-CONDITIONAL-RESEARCH-ONLY. Review binds source commit
`b781d68416c0c4e23ca44c7c160e8f8648b380ca`, tree
`27e08355f0e0cb6fb9506a9766f1ac8c67d090b0`, and retained source SHA256
`a7204076bf3562cf6c391ed3886cbe544391eba99a2c74940501c61964a2ee29`.

The independent decoder-facing caller reviewer inspected the whole protected
native handler and its known caller window, the actual retained source,
conditional contract, address token, test-domain guard and synthetic fixtures.
Fresh replay independently reproduced both NONMATCH scores, exact ELF extents,
full relocation results, 98,816 validated calls and eight rejected fixtures.

The review changes no source, test, verifier, preflight or prior receipt. Its
full findings and file hashes are in `independent_review.json`. The pending
review status inside the earlier recovery timestamp is historical and is
resolved by this subsequent artifact.

No blocker to retaining conditional research was found. Native containing
storage and actual producer-domain constraints remain unresolved; binary
matching credit is zero and unconditional runtime safety is not established.
