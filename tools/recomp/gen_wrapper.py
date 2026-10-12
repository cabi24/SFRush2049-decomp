#!/usr/bin/env python3
"""Generate one ROM-free, big-endian hard-float o32 replacement wrapper.

This deliberately parses a small, explicit prototype subset, not arbitrary C.
See README.md: 'o32 replacement wrappers' for the host-target contract.
"""
from __future__ import annotations

import argparse
from dataclasses import dataclass
from pathlib import Path
import re
import sys


class PrototypeError(ValueError):
    """The supplied prototype has no supported, unambiguous o32 mapping."""


@dataclass(frozen=True)
class CType:
    host: str
    bits: int
    kind: str  # signed, unsigned, float, pointer, void

    @property
    def words(self):
        return max(1, self.bits // 32)


@dataclass(frozen=True)
class Prototype:
    name: str
    result: CType
    args: tuple[CType, ...]


@dataclass(frozen=True)
class Location:
    slot: int
    fpr: int | None = None


TYPES = {"void": CType("void", 0, "void")}
for _bits in (8, 16, 32, 64):
    for _short, _kind, _prefix in (("s", "signed", "int"), ("u", "unsigned", "uint")):
        _type = CType(f"{_prefix}{_bits}_t", _bits, _kind)
        TYPES[f"{_short}{_bits}"] = TYPES[_type.host] = _type
for _names, _key in (
    (("signed char",), "s8"), (("unsigned char",), "u8"),
    (("short", "short int", "signed short", "signed short int"), "s16"),
    (("unsigned short", "unsigned short int"), "u16"),
    (("int", "signed", "signed int", "long", "long int", "signed long", "signed long int"), "s32"),
    (("unsigned", "unsigned int", "unsigned long", "unsigned long int"), "u32"),
    (("long long", "long long int", "signed long long", "signed long long int"), "s64"),
    (("unsigned long long", "unsigned long long int"), "u64"),
):
    for _name in _names:
        TYPES[_name] = TYPES[_key]
TYPES["f32"] = TYPES["float"] = CType("float", 32, "float")
TYPES["f64"] = TYPES["double"] = CType("double", 64, "float")
QUALIFIERS = {"const", "volatile", "restrict"}
KEYWORDS = set((
    "alignas alignof asm auto bool break case catch char class const constexpr "
    "continue decltype default delete do double else enum explicit export extern "
    "false float for friend goto if inline int long mutable namespace new noexcept "
    "nullptr operator private protected public register reinterpret_cast return "
    "short signed sizeof static static_assert static_cast struct switch template "
    "this thread_local throw true try typedef typeid typename union unsigned using "
    "virtual void volatile wchar_t while _Bool _Complex _Atomic restrict"
).split())
IDENT = r"[A-Za-z_][A-Za-z_0-9]*"


def identifier(name):
    if not re.fullmatch(IDENT, name) or name in KEYWORDS:
        raise PrototypeError(f"unsupported C/C++ identifier: {name!r}")
    return name


def parse_type(text, guest_pointers=False):
    tokens = text.replace("*", " * ").split()
    if not tokens:
        raise PrototypeError("missing type")
    if "*" in tokens:
        if not guest_pointers:
            raise PrototypeError("pointer prototypes require --guest-pointers and a guest-address host adapter")
        first = tokens.index("*")
        base = [t for t in tokens[:first] if t not in QUALIFIERS]
        tail = tokens[first:]
        # Pointee layout is deliberately opaque. Only a simple named type or
        # struct/union/enum tag is accepted; arrays/function pointers are not.
        valid_base = (" ".join(base) in TYPES or base == ["char"] or
                      (len(base) == 1 and re.fullmatch(IDENT, base[0]) and base[0] not in KEYWORDS) or
                      (len(base) == 2 and base[0] in {"struct", "union", "enum"}
                       and re.fullmatch(IDENT, base[1]) and base[1] not in KEYWORDS))
        if not valid_base or any(t != "*" and t not in QUALIFIERS for t in tail):
            raise PrototypeError(f"unsupported pointer declarator: {text!r}")
        return CType("rush_addr", 32, "pointer")
    normal = " ".join(t for t in tokens if t not in QUALIFIERS)
    if normal not in TYPES:
        raise PrototypeError(f"unsupported type: {text!r}; use an explicit fixed-width scalar or guest-address adapter")
    return TYPES[normal]


def parse_parameter(text, guest_pointers):
    try:
        return parse_type(text, guest_pointers)
    except PrototypeError as original:
        # A final identifier may be a parameter name, never part of a type
        # spelling. Refuse reserved names instead of generating invalid C++.
        match = re.fullmatch(rf"(.+?)(?:\s+|(?<=\*))({IDENT})", text)
        if not match or match[2] in KEYWORDS or match[2] in QUALIFIERS:
            raise original
        identifier(match[2])
        return parse_type(match[1], guest_pointers)


def parse_prototype(text, guest_pointers=False):
    # Reject comments/directives/declarator extensions rather than stripping
    # possibly meaningful syntax. Inputs are one declaration, not a C header.
    match = re.fullmatch(r"\s*([^();]+)\(([^()]*)\)\s*;?\s*", text)
    if not match or re.search(r"[^A-Za-z_0-9\s*(),;]", text):
        raise PrototypeError("expected one plain function prototype; no arrays, varargs, function pointers, comments or attributes")
    prefix = match[1].strip()
    prefix = re.sub(r"^extern\s+", "", prefix)
    split = re.fullmatch(rf"(.+?)(?:\s+|(?<=\*))({IDENT})", prefix)
    if not split:
        raise PrototypeError("expected a return type and function name")
    result = parse_type(split[1], guest_pointers)
    name = identifier(split[2])
    arg_text = match[2].strip()
    if not arg_text:
        raise PrototypeError("unprototyped () is unsupported; use (void)")
    args = () if arg_text == "void" else tuple(
        parse_parameter(t.strip(), guest_pointers) for t in arg_text.split(","))
    if any(t.kind == "void" for t in args):
        raise PrototypeError("void is only valid as the complete (void) parameter list")
    return Prototype(name, result, args)


def argument_locations(prototype):
    slot = 0
    leading_floats = True
    locations = []
    for index, arg in enumerate(prototype.args):
        if arg.words == 2:
            slot = (slot + 1) & ~1
        fpr = 12 + 2 * index if leading_floats and arg.kind == "float" and index < 2 else None
        locations.append(Location(slot, fpr))
        leading_floats = leading_floats and arg.kind == "float"
        slot += arg.words
    return tuple(locations)


def word_expression(slot):
    if slot < 4:
        return f"static_cast<uint32_t>(ctx->r{slot + 4})"
    return f"rush_o32_stack_word(&memory, ctx->r29, {slot})"


def value_expression(arg, loc):
    if loc.fpr is not None:
        return f"ctx->f{loc.fpr}.{'fl' if arg.bits == 32 else 'd'}"
    if arg.kind == "pointer" and loc.slot < 4:
        return f"rush_o32_pointer(ctx->r{loc.slot + 4})"
    raw = word_expression(loc.slot)
    if arg.words == 2:
        raw = f"((static_cast<uint64_t>({raw}) << 32) | {word_expression(loc.slot + 1)})"
    if arg.kind == "signed":
        return f"rush_o32_bits<{arg.host}>(static_cast<uint{arg.bits}_t>({raw}))"
    if arg.kind == "float":
        return f"rush_o32_bits<{arg.host}>({raw})"
    return f"static_cast<{arg.host}>({raw})"


def generate(text, *, target=None, wrapper=None, guest_pointers=False,
             with_rdram=False, rdram_size=0x800000):
    prototype = parse_prototype(text, guest_pointers)
    target = identifier(target or f"rush_host_{prototype.name}")
    wrapper = identifier(wrapper or prototype.name)
    if target == wrapper:
        raise PrototypeError("host target must differ from wrapper symbol")
    if not isinstance(rdram_size, int) or not 4 <= rdram_size <= 0x20000000 or rdram_size % 4:
        raise PrototypeError("rdram size must be a multiple of four in [4, 0x20000000]")
    locations = argument_locations(prototype)
    adapter = with_rdram or guest_pointers
    parameters = (["const rush_rdram* memory"] if adapter else []) + [
        f"{arg.host} arg{i}" for i, arg in enumerate(prototype.args)]
    lines = [
        "// Generated by tools/recomp/gen_wrapper.py; big-endian hard-float o32.",
        "// ROM-free prototype adapter. Do not use native pointers for guest memory.",
        '#include "o32_abi.h"', "",
        f'extern "C" {prototype.result.host} {target}({", ".join(parameters) or "void"});', "",
        f'extern "C" void {wrapper}(uint8_t* rdram, recomp_context* ctx) {{',
        "    if (ctx == nullptr) std::abort();",
        f"    const rush_rdram memory{{rdram, 0x{rdram_size:x}}};",
        "    (void)memory;",
    ]
    for i, (arg, loc) in enumerate(zip(prototype.args, locations)):
        source = f"f{loc.fpr}" if loc.fpr is not None else f"o32 word slot {loc.slot}"
        lines += [f"    // Argument {i}: {source}.",
                  f"    const {arg.host} arg{i} = {value_expression(arg, loc)};"]
    call_args = (["&memory"] if adapter else []) + [f"arg{i}" for i in range(len(prototype.args))]
    call = f'{target}({", ".join(call_args)})'
    result = prototype.result
    if result.kind == "void":
        lines.append(f"    {call};")
    else:
        lines.append(f"    const {result.host} result = {call};")
        if result.kind == "float":
            lines.append(f"    ctx->f0.{'fl' if result.bits == 32 else 'd'} = result;")
        elif result.bits == 64:
            lines += ["    const uint64_t bits = static_cast<uint64_t>(result);",
                      "    ctx->r2 = rush_o32_extend(static_cast<uint32_t>(bits >> 32));",
                      "    ctx->r3 = rush_o32_extend(static_cast<uint32_t>(bits));"]
        else:
            lines.append("    ctx->r2 = rush_o32_extend(static_cast<uint32_t>(result));")
    lines += ["}", ""]
    return "\n".join(lines)


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    inputs = parser.add_mutually_exclusive_group(required=True)
    inputs.add_argument("--prototype", help="one quoted C function prototype")
    inputs.add_argument("--prototype-file", type=Path, help="UTF-8 file containing exactly one prototype")
    parser.add_argument("--target", help="host C symbol (default: rush_host_NAME)")
    parser.add_argument("--wrapper", help="runtime wrapper symbol (default: prototype NAME)")
    parser.add_argument("--guest-pointers", action="store_true", help="map pointers to rush_addr and pass memory first")
    parser.add_argument("--with-rdram", action="store_true", help="pass a const rush_rdram* first even for scalar prototypes")
    parser.add_argument("--rdram-size", type=lambda s: int(s, 0), default=0x800000,
                        help="actual valid RDRAM span in bytes, default 0x800000; must match allocation")
    parser.add_argument("--out", type=Path, help="C++ output file (default: stdout)")
    args = parser.parse_args(argv)
    try:
        text = args.prototype if args.prototype is not None else args.prototype_file.read_text(encoding="utf-8")
        output = generate(text, target=args.target, wrapper=args.wrapper,
                          guest_pointers=args.guest_pointers, with_rdram=args.with_rdram,
                          rdram_size=args.rdram_size)
        if args.out is None:
            sys.stdout.write(output)
        else:
            args.out.parent.mkdir(parents=True, exist_ok=True)
            args.out.write_text(output, encoding="utf-8")
    except (PrototypeError, OSError, UnicodeError) as exc:
        print(f"gen_wrapper: {exc}", file=sys.stderr)
        return 2
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
