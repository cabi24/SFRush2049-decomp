"""Source-transformation and control-flow checks; not native-match evidence."""

import hashlib
import importlib.util
import itertools
import json
import math
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[2]
SPEC = importlib.util.spec_from_file_location("prepare_car_select_batch", ROOT / "tools/cloud/prepare_car_select_batch.py")
prep = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(prep)
SOURCE = (ROOT / prep.SOURCE_PATH).read_bytes().decode("utf-8")
VARIANTS = prep.variants(SOURCE)


def compact(text):
    return re.sub(r"\s+", "", text)


def case_groups(source):
    """Independent case split that works after arbitrary complete-group moves."""
    body = source.split(prep.SIGNATURE, 1)[1]
    end = body.rindex("  }\n\n}\n")
    labels = list(re.finditer(r"    case ([0-3]):\n", body[:end]))
    return {int(match[1]): body[match.end():labels[index + 1].start() if index + 1 < len(labels) else end]
            for index, match in enumerate(labels)}


class Statements:
    """Parse only the actual C control skeleton; leave expressions unmodified.

    No stand-in TU or caller is compiled. Unknown expressions/statements fail
    closed in trace() instead of being assigned convenient assumed semantics.
    """

    def __init__(self, source):
        self.source, self.pos = source, 0

    def skip(self):
        while self.pos < len(self.source) and self.source[self.pos].isspace():
            self.pos += 1

    def all(self):
        nodes = []
        self.skip()
        while self.pos < len(self.source) and self.source[self.pos] != "}":
            nodes.append(self.one())
            self.skip()
        return nodes

    def one(self):
        self.skip()
        if self.source[self.pos] == "{":
            self.pos += 1
            nodes = self.all()
            assert self.source[self.pos] == "}"
            self.pos += 1
            return ("block", nodes)
        if self.source.startswith("if (", self.pos):
            self.pos += 3
            start, depth = self.pos + 1, 1
            self.pos += 1
            while depth:
                depth += (self.source[self.pos] == "(") - (self.source[self.pos] == ")")
                self.pos += 1
            expression = self.source[start:self.pos - 1]
            yes = self.one()
            self.skip()
            no = None
            if self.source.startswith("else", self.pos):
                self.pos += 4
                no = self.one()
            return ("if", compact(expression), yes, no)
        end = self.source.index(";", self.pos)
        statement = compact(self.source[self.pos:end])
        self.pos = end + 1
        return ("statement", statement)


GROUPS = case_groups(SOURCE)
ASTS = {name: {case: Statements(body).all() for case, body in case_groups(source).items()}
        for name, source in [("baseline", SOURCE)] + [(key, value["source"]) for key, value in VARIANTS.items()]}
STATEMENTS = [compact(item) for item in re.findall(r"^\s*([^\n;]+);", GROUPS[0], re.MULTILINE)]
ACTION_STATEMENTS = [statement for statement in STATEMENTS if statement != "return"]
FLOAT_CONDITIONS = [compact(item) for item in re.findall(r"if \(([^\n]+)\)\n", GROUPS[1]) if " > " in item]


def trace(nodes, outer=0, inner=0, mode=6, difference=0.0):
    events = []

    def condition(expression):
        predicates = {
            compact(prep.OUTER_GUARD): ("outer_byte_read", outer <= 0),
            compact(prep.OUTER_GUARD.replace("<= 0", "> 0")): ("outer_byte_read", outer > 0),
            compact(prep.INNER_GUARD): ("inner_byte_read", inner != 0),
            compact(prep.INNER_GUARD.replace("!= 0", "== 0")): ("inner_byte_read", inner == 0),
            "gameplay_mode==6": ("gameplay_mode_read", mode == 6),
            "gameplay_mode!=6": ("gameplay_mode_read", mode != 6),
        }
        if expression in predicates:
            event, value = predicates[expression]
            events.append(event)
            return value
        for original in FLOAT_CONDITIONS:
            subtraction, literal = original.rsplit(">", 1)
            if expression in {original, literal + "<" + subtraction}:
                events.append(("unchanged_subtraction", subtraction, literal))
                threshold = float(literal[:-1])
                return difference > threshold if expression == original else threshold < difference
        raise AssertionError("unreviewed expression: " + expression)

    def execute(node):
        kind = node[0]
        if kind == "block":
            return any(execute(child) for child in node[1])
        if kind == "if":
            selected = node[2] if condition(node[1]) else node[3]
            return execute(selected) if selected is not None else False
        statement = node[1]
        if statement in {"return", "break"}:
            return True
        assert statement in ACTION_STATEMENTS + ["temp_v0->pad0EC[0x26C]=2"], statement
        events.append(statement)
        return False

    for node in nodes:
        if execute(node):
            break
    return events


class CarSelectBatchTests(unittest.TestCase):
    def test_twenty_unique_complete_sources_and_unchanged_prefix(self):
        prefix = SOURCE.split(prep.SIGNATURE, 1)[0]
        expected = {"C%02d" % n for n in range(1, 6)} | {"G%02d" % n for n in range(1, 5)} | {"X%02d" % n for n in range(1, 10)} | {"D01", "E01"}
        self.assertEqual(set(VARIANTS), expected)
        self.assertEqual(len({value["source"] for value in VARIANTS.values()}), 20)
        self.assertEqual(hashlib.sha256(SOURCE.encode()).hexdigest(), prep.SOURCE_SHA256)
        for candidate_id, value in VARIANTS.items():
            self.assertTrue(value["source"].startswith(prefix + prep.SIGNATURE))
            self.assertEqual(value["sha256"], hashlib.sha256(value["source"].encode()).hexdigest())
            self.assertIn("+++ candidates/" + candidate_id + ".c", value["patch"])
            self.assertTrue(all(value[key] for key in ("edit", "semantic_justification", "native_effect")))
            self.assertEqual(value["source"].count("func_800C4F68(2, temp_a1, 0);"), 1)

    def test_case_permutations_keep_verbatim_bodies_and_fallthrough(self):
        orders = set()
        for index in range(1, 6):
            candidate = VARIANTS["C%02d" % index]["source"]
            self.assertEqual(case_groups(candidate), GROUPS)
            order = tuple(re.findall(r"    case ([0-3]):", candidate))
            self.assertEqual(order.index("3"), order.index("2") + 1)
            orders.add(order)
        self.assertEqual(len(orders), 5)
        self.assertNotIn(("0", "2", "3", "1"), orders)
        self.assertEqual(ASTS["baseline"][2], [])
        self.assertEqual(trace(ASTS["baseline"][3]), [])

    def test_declaration_and_interaction_edits_are_exact(self):
        self.assertEqual(VARIANTS["D01"]["source"].replace(prep.REVERSED_DECLARATIONS, prep.DECLARATIONS, 1), SOURCE)
        parents = ["C%02d" % n for n in range(1, 6)] + ["G%02d" % n for n in range(1, 5)]
        for index, parent in enumerate(parents, 1):
            self.assertEqual(VARIANTS["X%02d" % index]["source"].replace(prep.REVERSED_DECLARATIONS, prep.DECLARATIONS, 1), VARIANTS[parent]["source"])

    def test_guard_equivalence_all_signed_byte_combinations(self):
        # Interprets the generated control skeleton, not a separately invented
        # replacement algorithm. Exact statements are recorded in execution order.
        for outer, inner in itertools.product(range(-128, 128), repeat=2):
            expected = trace(ASTS["baseline"][0], outer, inner)
            for candidate_id in ("G01", "G02", "G04"):
                self.assertEqual(trace(ASTS[candidate_id][0], outer, inner), expected)
        accepted = trace(ASTS["G01"][0], -128, -128)
        self.assertEqual(accepted[0], "outer_byte_read")
        self.assertEqual(accepted[2], "inner_byte_read")
        self.assertEqual(accepted[3:], ACTION_STATEMENTS[1:])
        self.assertEqual(trace(ASTS["G01"][0], 1, 1), ["outer_byte_read"])

    def test_mode_reversal_and_float_comparisons_including_nan(self):
        # Difference is the unchanged f32 subtraction result. Include exact
        # threshold equality and adjacent representable f32 values.
        differences = [-math.inf, -1.0, -0.0, 0.0, 2.25 - 2**-22, 2.25, 2.25 + 2**-22,
                       3.5 - 2**-22, 3.5, 3.5 + 2**-22, math.inf, math.nan]
        for mode, difference in itertools.product((-2**31, -1, 0, 5, 6, 7, 2**31 - 1), differences):
            expected = trace(ASTS["baseline"][1], mode=mode, difference=difference)
            for candidate_id in VARIANTS:
                self.assertEqual(trace(ASTS[candidate_id][1], mode=mode, difference=difference), expected)
        nan_trace = trace(ASTS["E01"][1], difference=math.nan)
        self.assertFalse(any(event == "temp_v0->pad0EC[0x26C]=2" for event in nan_trace))

    def test_changed_baseline_is_rejected_before_output_creation(self):
        for source in (SOURCE + "\n", SOURCE.replace("typedef signed char s8;", "typedef unsigned char s8;"), SOURCE.replace(prep.SIGNATURE, "void car_select_handler(int arg0)\n")):
            with self.assertRaisesRegex(ValueError, "pinned translation unit"):
                prep.variants(source)
        with tempfile.TemporaryDirectory() as temporary:
            bad_source, output = Path(temporary) / "bad.c", Path(temporary) / "plan"
            bad_source.write_text(SOURCE + "\n")
            with self.assertRaises(ValueError):
                prep.materialize(bad_source, output)
            self.assertFalse(output.exists())

    @unittest.skipUnless(shutil.which("cc"), "host C compiler unavailable for syntax-only check")
    def test_complete_sources_are_c89_syntactically_valid(self):
        # This parses the authentic complete TUs without fake callers, linking,
        # object emission, IDO execution or any native-match measurement.
        with tempfile.TemporaryDirectory() as temporary:
            path = Path(temporary) / "source.c"
            for candidate_id, source in [("baseline", SOURCE)] + [(key, value["source"]) for key, value in VARIANTS.items()]:
                path.write_bytes(source.encode())
                result = subprocess.run([shutil.which("cc"), "-std=c89", "-fsyntax-only", str(path)], capture_output=True, text=True, timeout=30)
                self.assertEqual(result.returncode, 0, candidate_id + ": " + result.stderr)

    def test_materialized_plan_hashes_and_no_overwrite(self):
        with tempfile.TemporaryDirectory() as temporary:
            output = Path(temporary) / "plan"
            receipt = prep.materialize(ROOT / prep.SOURCE_PATH, output)
            self.assertEqual(receipt["variants"], 20)
            self.assertEqual((output / "baseline.c").read_bytes(), SOURCE.encode())
            self.assertEqual((output / prep.SOURCE_PATH).read_bytes(), SOURCE.encode())
            self.assertEqual((output / prep.SOURCE_PATH).stat().st_mode & 0o222, 0)
            batch = json.loads((output / "batch.json").read_text())
            experiment = json.loads((output / "experiment.json").read_text())
            self.assertEqual(experiment["parameters"]["variant"], list(VARIANTS))
            self.assertEqual(batch["baseline_sha256"], prep.SOURCE_SHA256)
            self.assertEqual(batch["flags"], prep.FLAGS)
            for candidate_id, prediction in batch["predictions"].items():
                self.assertEqual(hashlib.sha256((output / prediction["source"]).read_bytes()).hexdigest(), prediction["sha256"])
                self.assertEqual((output / "patches" / (candidate_id + ".patch")).read_text(), VARIANTS[candidate_id]["patch"])
            with self.assertRaises(FileExistsError):
                prep.materialize(ROOT / prep.SOURCE_PATH, output)


if __name__ == "__main__":
    unittest.main()
