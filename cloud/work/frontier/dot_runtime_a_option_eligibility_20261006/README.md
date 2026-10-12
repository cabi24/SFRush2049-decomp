# Image-A option eligibility group

Installer-compatible extension of the existing group of the same name.

- `func_803ADDA8`: 1,048 fresh exact bytes, 262/262 words
- `func_803AE1C0`: 860 preserved exact bytes, 215/215 words
- `func_8038F744`: 88 preserved exact bytes, 22/22 words

All three are members and claims. The real callback `func_803AE1C0` is the sole
kept root. Its original 48-byte buffer and both installed helper bodies are
unchanged. This packet changes no production source, lock or target artifact.
Only the independent checker can install it and establish new ROM coverage.

```sh
python3 cloud/work/frontier/dot_runtime_a_option_eligibility_20261006/verify.py --score
python3 cloud/work/frontier/dot_runtime_a_option_eligibility_20261006/verify.py --deep
python3 cloud/work/frontier/dot_runtime_a_option_eligibility_20261006/trace.py
python3 -m pytest tests/cloud/test_runtime_a_position_callback.py -q
```

The score wrapper calls ordinary `score.py group` without `--claims`, after
binding authenticated current-tree image-A data in memory. Direct scoring
without those data reports the five float-literal references as unverified.
No ROM bytes, assembly dumps or compiled objects are published here.

The receipt binds source files and normalized target words, not mutable tools
or lock files. See [RESULTS.md](RESULTS.md) for trace, stack, buffer-bound,
ABI, GNU-link and limitation evidence. `--deep` includes negative controls,
GNU relinking, a 640-case host harness and IDO layout assertions; the single
pytest test runs only the bounded input and stock-group verification.
