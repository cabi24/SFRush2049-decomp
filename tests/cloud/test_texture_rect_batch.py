"""Frozen source/materialization checks, never native-match or ROM evidence.

The small interpreter executes the actual generated C control skeleton and
records SDK arguments. It does not compile a stand-in TU, caller or fake SDK.
The unmodified pinned SDK prefix is separately bound byte-for-byte. Arithmetic
samples are deliberately bounded; this test does not claim whole-C equivalence.
"""
import ast
from datetime import datetime
import hashlib
import importlib.util
import itertools
import json
from pathlib import Path
import random
import re
import subprocess
import tempfile
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[2]
SPEC = importlib.util.spec_from_file_location("prepare_texture_rect_batch", ROOT / "tools/cloud/prepare_texture_rect_batch.py")
prep = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(prep)
SOURCE = (ROOT / prep.SOURCE_PATH).read_bytes().decode("utf-8")
VARIANTS = prep.variants(SOURCE)
IDS = {"C01", "C02", "S01", "S02", "S03", "O01", "O02", "L01", "L02", "L03"}


class Statements:
    """Conservative parser for exactly this source family's C89 statements."""
    def __init__(self, source):
        self.source, self.pos = source, 0

    def skip(self):
        while self.pos < len(self.source) and self.source[self.pos].isspace():
            self.pos += 1

    def token(self, value):
        self.skip()
        if not self.source.startswith(value, self.pos):
            raise AssertionError("expected " + value + " at " + self.source[self.pos:self.pos + 60])
        self.pos += len(value)

    def condition(self):
        self.token("(")
        start, depth = self.pos, 1
        while depth:
            depth += (self.source[self.pos] == "(") - (self.source[self.pos] == ")")
            self.pos += 1
        return self.source[start:self.pos - 1]

    def one(self):
        self.skip()
        if self.source[self.pos] == "{":
            self.pos += 1
            children = []
            self.skip()
            while self.source[self.pos] != "}":
                children.append(self.one())
                self.skip()
            self.pos += 1
            return ("block", children)
        if self.source.startswith("if(", self.pos):
            self.pos += 2
            expression, yes = self.condition(), None
            yes = self.one()
            self.skip()
            no = None
            if self.source.startswith("else", self.pos):
                self.pos += 4
                no = self.one()
            return ("if", expression, yes, no)
        if self.source.startswith("do ", self.pos):
            self.pos += 3
            body = self.one()
            self.token("while")
            if self.condition() != "0":
                raise AssertionError("only a one-shot do-while-zero is reviewed")
            self.token(";")
            return ("once", body)
        end = self.source.index(";", self.pos)
        statement = self.source[self.pos:end].strip()
        self.pos = end + 1
        return ("statement", statement)


def parse(source):
    parser = Statements(source.split(prep.SIGNATURE, 1)[1])
    body = parser.one()
    parser.skip()
    assert parser.pos == len(parser.source)
    return body


# Parse and validate every expression before evaluating it. Unknown C constructs
# fail closed instead of being treated as an assumed-equivalent operation.
EXPRESSION_CACHE = {}
ALLOWED_AST = (ast.Expression, ast.Constant, ast.Name, ast.Load, ast.BinOp, ast.UnaryOp,
               ast.BoolOp, ast.Compare, ast.Add, ast.Sub, ast.LShift, ast.BitAnd,
               ast.Not, ast.USub, ast.And, ast.Or, ast.Lt, ast.Gt)


def expression(text, values):
    if text not in EXPRESSION_CACHE:
        translated = text.replace("&&", " and ").replace("||", " or ").replace("!", " not ").strip()
        tree = ast.parse(translated, mode="eval")
        assert all(isinstance(node, ALLOWED_AST) for node in ast.walk(tree)), text
        EXPRESSION_CACHE[text] = compile(tree, "bounded_source_expression", "eval")
    return eval(EXPRESSION_CACHE[text], {"__builtins__": {}}, values)


def trace(body, arguments, state):
    values = dict(zip(("x", "y", "right", "bottom", "s", "t"), arguments))
    values.update(state)
    packets = []

    def execute(node):
        if node[0] == "block":
            return any(execute(child) for child in node[1])
        if node[0] == "once":
            return execute(node[1])
        if node[0] == "if":
            selected = node[2] if expression(node[1], values) else node[3]
            return execute(selected) if selected is not None else False
        text = node[1]
        if text == "return":
            return True
        if text == "int height,step,offset,texture_edge":
            return False
        if text.startswith("gSPTextureRectangle("):
            arguments = text.removeprefix("gSPTextureRectangle(").removesuffix(")").split(",")
            assert len(arguments) == 10 and arguments.pop(0) == "D_80149438++"
            packets.append(tuple(expression(argument, values) for argument in arguments))
            return False
        match = re.fullmatch(r"(height|step|offset|texture_edge|x|y|right|bottom|s|t)(\+=|=)(.*)", text)
        assert match, text
        name, operation, value = match.groups()
        result = expression(value, values)
        if operation == "+=":
            result += values[name]
        assert -2**31 <= result < 2**31, "samples must avoid signed overflow"
        values[name] = result
        return False

    execute(body)
    assert len(packets) <= 1
    # Macro context proves this is three Gfx packets (24 bytes) per emission.
    return packets, 24 * len(packets)


ASTS = {"baseline": parse(SOURCE), **{name: parse(value["source"]) for name, value in VARIANTS.items()}}


def cases():
    bounds = {"D_8012E60C": 2, "D_8012E668": 3, "D_8012E610": 21, "D_8012E674": 19}
    rectangles = [(4, 5, 10, 12), (2, 3, 2, 3), (0, 0, 22, 20), (4, 5, 3, 12),
                  (4, 5, 10, 4), (0, 4, 8, 9), (4, 0, 8, 9), (4, 5, 25, 12), (4, 5, 10, 25)]
    for low, scale, mode, rectangle in itertools.product(range(16), (0, 0x8000), (0, 1, -1), rectangles):
        yield (*rectangle, 40, 48), {**bounds, "D_8012E608": low | scale | 0x10000, "D_8014A248": mode}
    rng = random.Random(87110)
    for _ in range(256):
        coordinates = tuple(rng.randint(0, 32) for _ in range(4))
        arguments = (*coordinates, rng.randint(40, 80), rng.randint(40, 80))
        yield arguments, {**bounds, "D_8012E608": rng.getrandbits(20), "D_8014A248": rng.choice((0, 1, -1))}


class TextureRectBatchTests(unittest.TestCase):
    def test_ten_unique_sources_and_frozen_context(self):
        self.assertEqual(set(VARIANTS), IDS)
        self.assertEqual(len({value["source"] for value in VARIANTS.values()}), 10)
        prefix, baseline_body = SOURCE.split(prep.SIGNATURE, 1)
        before_stretch = baseline_body.split(prep.SETUP, 1)[0]
        for candidate_id, value in VARIANTS.items():
            source = value["source"]
            self.assertTrue(source.startswith(prefix + prep.SIGNATURE + before_stretch))
            self.assertEqual(source.count("gSPTextureRectangle("), 9)  # definition plus eight calls
            self.assertEqual(source.count("int height,step,offset,texture_edge;"), 1)
            self.assertEqual(source.count("void func_80087110("), 1)
            self.assertNotIn("#", source.split(prep.SIGNATURE, 1)[1])
            self.assertEqual(value["sha256"], hashlib.sha256(source.encode()).hexdigest())
            self.assertIn("+++ candidates/" + candidate_id + ".c", value["patch"])
            self.assertTrue(all(value[key] for key in ("edit", "semantic_justification", "native_effect")))

    def test_actual_pinned_repository_baseline_and_control_hashes(self):
        pinned = subprocess.check_output(["git", "show", prep.BASE_COMMIT + ":" + prep.SOURCE_PATH], cwd=ROOT)
        self.assertEqual(pinned, SOURCE.encode())
        self.assertEqual(hashlib.sha256(pinned).hexdigest(), prep.SOURCE_SHA256)
        for candidate_id, control in prep.HISTORICAL.items():
            path = "cloud/work/texture_rect_compiler_boundary/" + control["name"] + ".c"
            pinned_control = subprocess.check_output(["git", "show", prep.BASE_COMMIT + ":" + path], cwd=ROOT)
            self.assertEqual(pinned_control, VARIANTS[candidate_id]["source"].encode())
            self.assertEqual(hashlib.sha256(pinned_control).hexdigest(), control["sha256"])

    def test_exact_step_boundary_pair(self):
        outside = VARIANTS["S02"]["source"]
        inside = VARIANTS["S03"]["source"]
        self.assertEqual(outside.replace(prep.EARLY_STEP + "        do {height=bottom-y;\n",
                                         "        do {\n" + prep.EARLY_STEP + "        height=bottom-y;\n", 1), inside)
        for candidate_id in ("S01", "S02", "S03"):
            source = VARIANTS[candidate_id]["source"]
            self.assertEqual(source.count("step=512;"), 1)
            self.assertEqual(source.count("step=1024;"), 1)

    def test_coordinate_carrier_changes_only_terminal_stretched_uses(self):
        for candidate_id, parent in (("L01", "C01"), ("L02", "C02")):
            child = VARIANTS[candidate_id]["source"]
            restored = child.replace("texture_edge=texture_edge-x", "s=texture_edge-x").replace(",0,texture_edge<<5,", ",0,s<<5,")
            self.assertEqual(restored, VARIANTS[parent]["source"])
            self.assertEqual(child.count("texture_edge=texture_edge-x"), 2)

    def test_bounded_actual_source_control_and_packet_argument_equivalence(self):
        count, rejected, emitted = 0, 0, 0
        for arguments, state in cases():
            expected = trace(ASTS["baseline"], arguments, state)
            rejected += not expected[0]
            emitted += bool(expected[0])
            for candidate_id in VARIANTS:
                self.assertEqual(trace(ASTS[candidate_id], arguments, state), expected, (candidate_id, arguments, state))
            count += 1
        self.assertEqual(count, 1120)
        self.assertGreater(rejected, 0)
        self.assertGreater(emitted, 0)

    def test_source_trace_detects_real_semantic_mutations(self):
        mutants = [
            VARIANTS["S02"]["source"].replace("step=512;", "step=513;", 1),
            VARIANTS["O01"]["source"].replace("offset=16;", "offset=15;", 1),
            VARIANTS["L02"]["source"].replace("texture_edge=texture_edge-x", "texture_edge=texture_edge+x", 1),
            VARIANTS["L03"]["source"].replace("if(D_8012E608&4) {texture_edge", "if(D_8012E608&8) {texture_edge", 1),
        ]
        for mutant in mutants:
            body = parse(mutant)
            self.assertTrue(any(trace(body, arguments, state) != trace(ASTS["baseline"], arguments, state)
                                for arguments, state in cases()))

    def test_changed_baseline_and_anchors_fail_closed(self):
        for source in (SOURCE + "\n", SOURCE.replace("int x,int y", "short x,int y"), SOURCE.replace("#define _SHIFTL", "#define SHIFT")):
            with self.assertRaisesRegex(ValueError, "pinned translation unit"):
                prep.variants(source)
        with self.assertRaises(ValueError):
            prep.replace_once("abc abc", "abc", "def")
        with tempfile.TemporaryDirectory() as temporary:
            bad, output = Path(temporary) / "bad.c", Path(temporary) / "plan"
            bad.write_text(SOURCE + "\n")
            with self.assertRaises(ValueError):
                prep.materialize(bad, output)
            self.assertFalse(output.exists())

    def test_materialized_experiment_receipts_and_no_overwrite(self):
        with tempfile.TemporaryDirectory() as temporary:
            output = Path(temporary) / "plan"
            receipt = prep.materialize(ROOT / prep.SOURCE_PATH, output)
            self.assertEqual(receipt["variants"], 10)
            self.assertEqual(receipt["historical_controls"], 2)
            self.assertEqual((output / "baseline.c").read_bytes(), SOURCE.encode())
            self.assertEqual((output / prep.SOURCE_PATH).read_bytes(), SOURCE.encode())
            self.assertEqual((output / "baseline.c").stat().st_mode & 0o222, 0)
            batch = json.loads((output / "batch.json").read_text())
            review = json.loads((output / "review.json").read_text())
            experiment = json.loads((output / "experiment.json").read_text())
            self.assertEqual(batch["purpose"], "experiment")
            self.assertEqual(batch["flags"], prep.FLAGS)
            self.assertEqual(batch["baseline_expectation"], {"differing": 4, "total": 445, "extra_words": 0})
            self.assertEqual(batch["limits"]["variants"], 10)
            self.assertEqual(batch["targets"], "asm/us/blob")
            self.assertEqual(experiment["controls"], [])  # historical controls are budgeted source candidates
            self.assertEqual(experiment["parameters"]["variant"], list(VARIANTS))
            self.assertEqual(review["status"], "source_reviewed_uncompiled")
            self.assertEqual(review["claims"], [])
            for candidate_id, prediction in batch["predictions"].items():
                self.assertEqual(hashlib.sha256((output / prediction["source"]).read_bytes()).hexdigest(), prediction["sha256"])
                self.assertEqual((output / "patches" / (candidate_id + ".patch")).read_text(), VARIANTS[candidate_id]["patch"])
            events = [json.loads(line) for line in (output / "materialization-events.jsonl").read_text().splitlines()]
            self.assertEqual([event["event"] for event in events], ["start", "end"])
            self.assertEqual([event["status"] for event in events], ["running", "completed"])
            self.assertEqual({event["materialization_id"] for event in events}, {receipt["materialization_id"]})
            self.assertLessEqual(datetime.fromisoformat(events[0]["utc"]), datetime.fromisoformat(events[1]["utc"]))
            self.assertAlmostEqual(events[1]["monotonic_seconds"] - events[0]["monotonic_seconds"], events[1]["duration_seconds"])
            self.assertGreaterEqual(events[1]["duration_seconds"], 0)
            with self.assertRaises(FileExistsError):
                prep.materialize(ROOT / prep.SOURCE_PATH, output)


    def test_partial_materialization_appends_failure_after_start(self):
        with tempfile.TemporaryDirectory() as temporary:
            output = Path(temporary) / "plan"
            with mock.patch.object(prep, "_materialize_files", side_effect=OSError("injected write failure")):
                with self.assertRaisesRegex(OSError, "injected write failure"):
                    prep.materialize(ROOT / prep.SOURCE_PATH, output)
            self.assertFalse((output / "batch.json").exists())
            events = [json.loads(line) for line in (output / "materialization-events.jsonl").read_text().splitlines()]
            self.assertEqual([event["event"] for event in events], ["start", "end"])
            self.assertEqual([event["status"] for event in events], ["running", "failed"])
            self.assertEqual(events[0]["materialization_id"], events[1]["materialization_id"])
            self.assertEqual(events[1]["error_type"], "OSError")
            self.assertGreaterEqual(events[1]["duration_seconds"], 0)


if __name__ == "__main__":
    unittest.main()
