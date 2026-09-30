"""group_search executor: permute one function of an IDO -O3 call group.

About 40% of the game was compiled as whole-program IPA groups, so a function
cannot be searched alone: its registers depend on its callers and callees.
This job runs the permuter on the group's source, mutating only the target
function, but compiles every candidate as the whole group (cc -j, then uld -kp
keep.txt, usplit, umerge, uopt, ugen, as1 -r4300_mul — the stages of
blob_group.builder_script) and scores just the target's slice, linked at its
image address (see groupdump).

Manifest:
    {
      "job_type": "group_search",
      "toolkit_sha": "...",
      "target_id": "func_800C9590",     # the function being permuted
      "group": "controller_poll",
      "seed_file": "base.c",            # under inputs/: the file holding the target
      "seed_name": "group.c",           # its name inside the group
      "extra_files": ["other.c"],       # under inputs/, compiled with it
      "keep": ["fn", ...],              # uld -kp list
      "compile_flags": "-g0 -O3 -mips2 -G 0 -non_shared",
      "spec_file": "spec.json",         # groupdump spec (see groupdump.py)
      "budget": {"wall_seconds": 14400, "iterations": null}
    }
"""
import base64
import gzip
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

_HERE = Path(__file__).resolve().parent
if str(_HERE) not in sys.path:
    sys.path.insert(0, str(_HERE))

import groupdump  # noqa: E402
import permuter_search  # noqa: E402
import _permuter  # noqa: E402


# --- __inline: pycparser (the permuter's parser) rejects it, IDO -O3 needs it ---

_INLINE_DECL = re.compile(r"__inline\b[ \t]+([^();{}]*?)\b([A-Za-z_]\w*)[ \t]*\(")
_INLINE_TOKEN = re.compile(r"__inline\b[ \t]+")


def strip_inline(source):
    """(source without `__inline`, {function: 'static' | ''}): the marker is
    removed from every declaration that carries it (definitions and prototypes;
    line numbers are unchanged) and remembered by function name. 'static' means
    the declaration line starts with `static`, so the marker follows it."""
    names = {}
    for line in source.split("\n"):
        m = _INLINE_DECL.search(line)
        if m:
            names[m.group(2)] = "static" if line.startswith("static") else ""
    return _INLINE_TOKEN.sub("", source), names


def _inline_pattern(name, kind):
    """Regex for a line-start declaration of `name` (calls are indented or
    have no type in front, so they do not match)."""
    lead = r"static[ \t]+" if kind == "static" else r"(?!static\b)"
    return re.compile(r"^(" + lead + r")(?!__inline)(?=[^;=()\n]*[^\w]" + re.escape(name) +
                      r"[ \t]*\()", re.M)


def restore_inline(source, names):
    """Inverse of strip_inline for source the permuter has rewritten."""
    for name, kind in names.items():
        if kind == "static":
            source = _inline_pattern(name, kind).sub(
                lambda m: m.group(1) + "__inline ", source)
        else:
            source = _inline_pattern(name, kind).sub("__inline ", source)
    return source


def inline_sed(names):
    """POSIX sed arguments that do restore_inline's job (compile.sh runs them
    on every candidate before the group is built). '' when nothing to restore."""
    if not names:
        return ""
    # lines that already carry the marker are left alone (b = end of script);
    # static declarations are done first so the plain-declaration rules below
    # only ever see lines that did not start with `static`
    parts = ["-e '/__inline/b'"]
    for name, kind in sorted(names.items()):
        if kind == "static":
            parts.append(f"-e '/^static[[:space:]].*[^A-Za-z0-9_]{name}[[:space:]]*(/ "
                         f"s/^static[[:space:]][[:space:]]*/static __inline /'")
    parts.append("-e '/^static[^A-Za-z0-9_]/b'")
    for name, kind in sorted(names.items()):
        if kind != "static":
            parts.append(f"-e '/^[A-Za-z_][^;=(]*[^A-Za-z0-9_]{name}[[:space:]]*(/ "
                         f"s/^/__inline /'")
    return " ".join(parts)


class _RestoringProgress:
    """Progress sink that puts `__inline` back into best_source checkpoints."""

    def __init__(self, progress, names):
        self._progress, self._names = progress, names

    def update(self, **kw):
        if kw.get("best_source") and self._names:
            text = gzip.decompress(base64.b64decode(kw["best_source"])).decode()
            kw["best_source"] = base64.b64encode(gzip.compress(
                restore_inline(text, self._names).encode())).decode()
        return self._progress.update(**kw)

    def __getattr__(self, name):
        return getattr(self._progress, name)


def _restore_file(path, names):
    """A copy of `path` with `__inline` restored, in a directory that outlives
    the job's scratch dir."""
    out = Path(tempfile.mkdtemp(prefix="grpbest-")) / "best.c"
    out.write_text(restore_inline(Path(path).read_text(), names))
    return str(out)


def pipeline(toolkit, flags, files, workdir="."):
    """The build stages, as shell. Kept identical to blob_group.builder_script
    (a test compares the two); only the toolkit path differs."""
    units = " ".join(f[:-2] + ".u" if f.endswith(".c") else f for f in files)
    opt = "-O3"
    T = f"{toolkit}/ido"
    return (
        f'T="{T}"; cd {workdir}; '
        f"$T/cc -j {flags} {' '.join(files)} >cc.log 2>&1 || {{ cat cc.log >&2; exit 1; }}; "
        f"$T/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 "
        f"-no_AutoGnum -kp keep.txt {units} -ko linked; "
        f"$T/usplit -mips2 -o split -t st linked; "
        f"$T/umerge -Olimit 5000 -mips2 -EB -g0 {opt} split -o merged -t st >/dev/null; "
        f"$T/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 {opt} merged opt -t st optlog >/dev/null; "
        f"$T/ugen -G 0 -mips2 -EB -g0 {opt} opt -o gen -t st -temp ugtmp >/dev/null; "
        f"$T/as1 -elf -G 0 -p0 -mips2 -EB -g0 {opt} -r4300_mul -Olimit 5000 gen -o group.o -t st >/dev/null"
    )


def write_compile_sh(perm_dir, toolkit, manifest, static_dir, inline_names=None):
    """compile.sh IN.c -o OUT.o: build the group with IN.c as the target file.
    IN.c has no `__inline` (the permuter cannot parse it); the functions in
    `inline_names` get it back before the compiler sees the copy."""
    seed = manifest["seed_name"]
    sed = inline_sed(inline_names or {})
    if sed:
        put = f'sed {sed} "$SRC" > "$W/{seed}" || exit 1\n'
    else:
        put = f'cp "$SRC" "$W/{seed}" || exit 1\n'
    files = [manifest["seed_name"]] + list(manifest.get("extra_files", []))
    script = perm_dir / "compile.sh"
    script.write_text(
        "#!/bin/sh\n"
        "set -e\n"
        'SRC="$1"; OUT="$3"\n'
        'W=$(mktemp -d "${TMPDIR:-/tmp}/grp.XXXXXX") || exit 1\n'
        'trap \'rm -rf "$W"\' EXIT\n'
        f'cp -R "{static_dir}"/. "$W"/ || exit 1\n'
        + put
        + pipeline(toolkit, manifest["compile_flags"], files, workdir='"$W"') + "\n"
        'cp "$W/group.o" "$OUT"\n'
    )
    script.chmod(0o755)


def run(job_dir, manifest, progress):
    job_dir = Path(job_dir)
    inputs = job_dir / "inputs"
    toolkit = permuter_search._toolkit()
    budget = manifest["budget"] or {}
    wall_budget = budget.get("wall_seconds") or 4 * 3600
    cores = int(os.environ.get("CONVEYOR_CORES", "1"))

    perm_dir = Path(tempfile.mkdtemp(prefix="grpjob-"))
    static_dir = perm_dir / "static"
    try:
        static_dir.mkdir()
        # the permuter's parser rejects __inline: strip it here, compile.sh
        # puts it back on exactly those functions, results get it restored
        stripped, inline_names = strip_inline((inputs / manifest["seed_file"]).read_text())
        (perm_dir / "base.c").write_text(stripped)
        progress = _RestoringProgress(progress, inline_names)
        for name in manifest.get("extra_files", []):
            shutil.copy(inputs / name, static_dir / name)
        (static_dir / "keep.txt").write_text("".join(f"{k}\n" for k in manifest["keep"]))

        spec = json.loads((inputs / manifest["spec_file"]).read_text())
        spec_path = perm_dir / "spec.json"
        shutil.copy(inputs / manifest["spec_file"], spec_path)
        target_o = perm_dir / "target.o"
        target_o.write_bytes(groupdump.stub_bytes(bytes.fromhex(spec["retail"])))

        (perm_dir / "function.txt").write_text(manifest["target_id"] + "\n")
        write_compile_sh(perm_dir, toolkit, manifest, static_dir, inline_names)
        (perm_dir / "settings.toml").write_text(
            f'objdump_command = "{sys.executable} {_HERE / "groupdump.py"} {spec_path}"\n')

        base_score = None
        with tempfile.NamedTemporaryFile(suffix=".o") as base_o:
            proc = subprocess.run(
                [str(perm_dir / "compile.sh"), str(perm_dir / "base.c"), "-o", base_o.name],
                capture_output=True, text=True, timeout=600)
            if proc.returncode == 0:
                scorer = _permuter.Scorer(
                    target_o=str(target_o), stack_differences=True, algorithm="difflib",
                    debug_mode=False, ign_branch_targets=False,
                    objdump_command=f"{sys.executable} {_HERE / 'groupdump.py'} {spec_path}")
                base_score = scorer.score(base_o.name)[0]
            error = (proc.stderr or proc.stdout)[-600:]
        if base_score is None:
            return {"target_id": manifest["target_id"], "group": manifest["group"],
                    "final_best_score": None, "base_score": None,
                    "error": "group does not build: " + " ".join(error.split())}, {}
        progress.update(best_score=base_score,
                        best_source=permuter_search._gz_b64(perm_dir / "base.c"))
        if base_score == 0:
            payload = {"target_id": manifest["target_id"], "group": manifest["group"],
                       "final_best_score": 0, "base_score": 0, "wall_seconds_used": 0}
            return payload, {"best.c": _restore_file(perm_dir / "base.c", inline_names)}

        payload, artifacts = permuter_search.drive(
            perm_dir, manifest, progress, base_score, wall_budget, cores,
            extra_args=("--stack-diffs",))
        payload["group"] = manifest["group"]
        if inline_names and "best.c" in artifacts:
            artifacts["best.c"] = _restore_file(artifacts["best.c"], inline_names)
        return payload, artifacts
    finally:
        shutil.rmtree(perm_dir, ignore_errors=True)
