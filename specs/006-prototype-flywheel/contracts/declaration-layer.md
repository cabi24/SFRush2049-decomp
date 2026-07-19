# Contract: Generated Declaration Layer

Consumers: `pipeline/protos.py` (new), `autodecomp._context()`.

## Generation (normative)

1. **Scope**: every extracted target that is derivable (has an object, not
   `extent_conflict`/`no_disasm`) plus every `func_<addr>`/named callee
   referenced from derived extracted asm whose address resolves to a known
   target (either population).
2. **Signature source order** per callee:
   (a) its own m2c definition line (the `_extract_fn` isolation: text from
   return type through the closing parenthesis of the parameter list),
   captured during the population m2c pass;
   (b) fallback `s32 <name>();` when (a) is unavailable (m2c crash,
   partial decomp, or the callee is a known target address without a
   derivable body).
   There is no call-site inference and no conflict resolution — one
   callee, one definition.
3. **Two passes, exactly**: pass 1 runs the population m2c pass with the
   hand context only and captures signatures; pass 2 re-runs with the
   pass-1 layer included and captures again; the artifact is pass 2's
   result. Same inputs ⇒ byte-identical artifact (sorted by symbol name,
   stamp separated from content so content stability is checkable).
4. **Precedence filter**: a name is OMITTED (recorded with reason) when it
   is already declared by the preprocessed hand-curated context chain or
   is a static/library target whose real declaration exists there. The
   combined context MUST produce zero redefinition/conflict errors across
   the full derivable population (SC-002 gate: population compile probe
   reports no `redeclar`/`conflicting types` class errors).
5. **Consumption**: `_context()` includes `build/m2c_protos.h` LAST, only
   if present; its content sha joins the derived-asm/seed cache keys the
   same way `game_types.h`'s does (context sha), so layer changes
   regenerate downstream artifacts automatically.
6. **Static isolation**: the 005 SC-005 body-identity guard MUST stay
   green with the layer active. (Generated declarations are game-code
   names; the guard test proves none of them perturb a static body.)

## Acceptance oracle

- Double `generate` → `m2c_protos.h` byte-identical; stamp differs only in
  timestamp.
- `func_<addr>`-shaped blockers for known-target callees: 289 → 0 in the
  next population histogram.
- Spot oracle: a target blocked on `math_utility` (26 blocked at baseline)
  compiles with the layer active and no other change.
- `declarations` + `omitted` covers every referenced known-target callee;
  no name appears in both.
