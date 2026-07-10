#!/usr/bin/env python3
"""Keep symbol_addrs.us.txt splat-consumable (004 extraction remediation).

splat parses every space-separated token after `//` on a definition line as a
`key:value` attribute: tokens with >1 colon are fatal, `type:data` is not a
valid type, and duplicate addresses/names abort the split. This tool
normalizes in place, deterministically:

- trailing comments: valid splat attributes are kept; `type:data` is dropped
  (untyped is correct for plain data); every other colon-bearing token has its
  colons rewritten to '.' so provenance prose (arcade:file:func etc.) survives
  as prose;
- later duplicate addresses/names are commented out (`// dup-addr(...)` /
  `// dup-name(...)`), first definition wins — file order is the tiebreak;
- full-line comments and blank lines pass through untouched.

Idempotent: running twice changes nothing. Our own tools (conveyor targets,
matrix) parse only `NAME = 0xADDR;` heads and ignore commented lines, so they
are unaffected.
"""
import re
import sys
from pathlib import Path

DEF_RE = re.compile(r"^(\s*)([A-Za-z_][A-Za-z0-9_]*)\s*=\s*(0x[0-9A-Fa-f]+)\s*;(.*)$")
VALID_ATTR_KEYS = {"type", "size", "rom", "segment", "name_end", "dead",
                   "ignore", "extract", "allow_duplicated", "defined",
                   "force_migration", "force_not_migrated", "allow_addend",
                   "can_reference", "can_be_referenced", "filename"}
VALID_TYPES = {"func", "jtbl", "jtbl_label", "label",
               "u8", "s8", "u16", "s16", "u32", "s32", "u64", "s64",
               "f32", "f64", "asciz", "char", "char*", "String"}


def clean_comment(comment):
    out = []
    for tok in comment.split():
        if ":" in tok:
            key, _, val = tok.partition(":")
            if key == "type" and val not in VALID_TYPES:
                continue  # e.g. type:data — not a splat type; untyped is fine
            if key in VALID_ATTR_KEYS and ":" not in val:
                out.append(tok)
                continue
            tok = tok.replace(":", ".")
        out.append(tok)
    return out


def sanitize(text):
    seen_addr, seen_name = {}, {}
    lines, n_comment_fixed, n_dups = [], 0, 0
    for line in text.splitlines():
        m = DEF_RE.match(line)
        if not m:
            lines.append(line)
            continue
        indent, name, addr_s, rest = m.groups()
        addr = int(addr_s, 16)
        rest = rest.strip()
        comment_tokens = []
        if rest.startswith("//"):
            cleaned = clean_comment(rest[2:])
            if " ".join(cleaned) != rest[2:].strip():
                n_comment_fixed += 1
            comment_tokens = cleaned
        elif rest:
            comment_tokens = clean_comment(rest.lstrip("/ "))
        newline = f"{indent}{name} = 0x{addr:08X};"
        if comment_tokens:
            newline += " // " + " ".join(comment_tokens)
        if addr in seen_addr:
            lines.append(f"// dup-addr(kept {seen_addr[addr]}): {newline.strip()}")
            n_dups += 1
            continue
        if name in seen_name:
            lines.append(f"// dup-name(kept 0x{seen_name[name]:08X}): {newline.strip()}")
            n_dups += 1
            continue
        seen_addr[addr] = name
        seen_name[name] = addr
        lines.append(newline)
    return "\n".join(lines) + "\n", n_comment_fixed, n_dups, len(seen_name)


def main():
    path = Path(sys.argv[1] if len(sys.argv) > 1 else "symbol_addrs.us.txt")
    new, fixed, dups, kept = sanitize(path.read_text())
    path.write_text(new)
    print(f"{path}: {kept} definitions kept, {fixed} comments normalized, "
          f"{dups} duplicates commented out")


if __name__ == "__main__":
    main()
