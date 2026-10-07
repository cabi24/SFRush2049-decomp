"""Focused tooling tests; these do not claim a ROM or source acceptance gate."""
import copy
import importlib.util
import json
import os
from pathlib import Path
import signal
import sys
import tempfile
import time
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.cloud import hypothesis_batch as batch
from tools.cloud import prepare_car_select_batch as prepare


class EditableBodyContextTests(unittest.TestCase):
    BASE = "typedef int T; static T h(T x) { return x; } T f(T x) { return h(x); }"

    def test_named_helper_body_allowed_only_when_declared(self):
        changed = self.BASE.replace("return x;", "return x + 0;")
        self.assertEqual(batch.source_context(self.BASE, ["f", "h"]), batch.source_context(changed, ["f", "h"]))
        self.assertNotEqual(batch.source_context(self.BASE, ["f"]), batch.source_context(changed, ["f"]))

    def test_signature_context_and_other_bodies_remain_frozen(self):
        for changed in (self.BASE.replace("T h(T x)", "T h(T y)"), self.BASE.replace("typedef int", "typedef long"), self.BASE + " T g(void) {return 0;}"):
            self.assertNotEqual(batch.source_context(self.BASE, ["f", "h"]), batch.source_context(changed, ["f", "h"]))

    def test_directives_and_missing_helpers_rejected(self):
        with self.assertRaises(ValueError):
            batch.source_context(self.BASE.replace("return x;", "#define X 0\nreturn x;"), ["f", "h"])
        with self.assertRaises(ValueError):
            batch.source_context(self.BASE, ["f", "missing"])


class ManifestTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        self.plan = self.root / "plan"
        prepare.materialize(ROOT / prepare.SOURCE_PATH, self.plan)
        self.path = self.plan / "batch.json"

    def tearDown(self):
        self.tmp.cleanup()

    def change(self, update, filename="batch.json"):
        path = self.plan / filename
        value = batch.read_json(path)
        update(value)
        batch.write_json(path, value)

    def test_valid_manifest_and_every_candidate(self):
        plan, context, experiment = batch.validate(self.path)
        self.assertEqual(len(plan["predictions"]), 20)
        self.assertEqual(len(experiment.candidates), 20)
        self.assertEqual(context["source"], prepare.SOURCE_PATH)

    def test_invalid_editable_helpers_fail_closed(self):
        for helpers in (["missing"], ["car_select_handler"], ["x", "x"], ["a", "b", "c", "d", "e"], [{}], "x"):
            self.change(lambda value: value.update(editable_helpers=helpers))
            with self.assertRaisesRegex(ValueError, "editable helper"):
                batch.validate(self.path)

    def test_unknown_and_duplicate_json_keys(self):
        self.change(lambda value: value.update(extra=True))
        with self.assertRaisesRegex(ValueError, "unknown"):
            batch.validate(self.path)
        self.path.write_text('{"schema":"a", "schema":"b"}')
        with self.assertRaisesRegex(ValueError, "duplicate JSON"):
            batch.validate(self.path)

    def test_budget_and_flags_fail_closed(self):
        self.change(lambda value: value["limits"].update(jobs=3))
        with self.assertRaisesRegex(ValueError, "budget"):
            batch.validate(self.path)
        self.change(lambda value: value["limits"].update(jobs=2))
        self.change(lambda value: value["flags"].append("-I/tmp"))
        with self.assertRaisesRegex(ValueError, "recipe"):
            batch.validate(self.path)

    def test_twenty_five_variants_supported_with_other_limits_unchanged(self):
        plan = batch.read_json(self.path)
        experiment = batch.read_json(self.plan / "experiment.json")
        template = copy.deepcopy(plan["predictions"]["C01"])
        for number in range(21, 26):
            candidate_id = "X" + str(number)
            prediction = copy.deepcopy(template)
            prediction["source"] = f"candidates/{candidate_id}.c"
            (self.plan / prediction["source"]).write_bytes((self.plan / template["source"]).read_bytes())
            plan["predictions"][candidate_id] = prediction
            experiment["parameters"]["variant"].append(candidate_id)
            experiment["candidates"].append({"source": prediction["source"], "parameters": {"variant": candidate_id}})
        plan["limits"]["variants"] = 25
        batch.write_json(self.path, plan)
        batch.write_json(self.plan / "experiment.json", experiment)
        validated, _, parsed = batch.validate(self.path)
        self.assertEqual(len(validated["predictions"]), 25)
        self.assertEqual(len(parsed.candidates), 25)
        for key, invalid in (("variants", 26), ("jobs", 3), ("compile_seconds", 121),
                             ("score_seconds", 31), ("diagnose_seconds", 31)):
            with self.subTest(limit=key):
                invalid_plan = copy.deepcopy(plan)
                invalid_plan["limits"][key] = invalid
                batch.write_json(self.path, invalid_plan)
                with self.assertRaisesRegex(ValueError, "budget: " + key):
                    batch.validate(self.path)

    def test_changed_candidate_and_context_rejected(self):
        candidate = self.plan / "candidates/C01.c"
        candidate.chmod(0o644)
        candidate.write_text(candidate.read_text() + "\n")
        with self.assertRaisesRegex(ValueError, "source hash changed"):
            batch.validate(self.path)
        self.change(lambda value: value["predictions"]["C01"].update(sha256=batch.file_hash(candidate)))
        with self.assertRaisesRegex(ValueError, "genuine TU context"):
            batch.validate(self.path)

    def test_changed_header_manifest_hash_and_outside_paths_rejected(self):
        self.change(lambda value: value["files"][0].update(sha256="0" * 64), "context.json")
        with self.assertRaisesRegex(ValueError, "context differs"):
            batch.validate(self.path)
        with self.assertRaises(ValueError):
            batch.inside(self.root, "../outside")
        (self.root / "link").symlink_to(self.path)
        with self.assertRaisesRegex(ValueError, "symlink"):
            batch.inside(self.root, "link")

    def test_missing_measurement_and_unknown_signal(self):
        self.change(lambda value: value["predictions"]["C01"]["check"].update(signal_ids=["invented"]))
        with self.assertRaisesRegex(ValueError, "signal"):
            batch.validate(self.path)

    def test_comment_prefixed_and_alternate_preprocessor_tokens_rejected(self):
        for spelling in ('/* hidden */ #include "/tmp/header.h"', '??=include "x"', '%:include "x"', '#inc\\\nlude "x"'):
            self.assertTrue(batch.has_preprocessor(spelling))
        candidate = self.plan / "candidates/C01.c"
        candidate.chmod(0o644)
        head, body, tail = batch.split_function(candidate.read_text(), "car_select_handler")
        candidate.write_text(head + '\n/* hidden */ #include "/tmp/header.h"\n' + body + tail)
        self.change(lambda value: value["predictions"]["C01"].update(sha256=batch.file_hash(candidate)))
        with self.assertRaisesRegex(ValueError, "genuine TU context"):
            batch.validate(self.path)

    def test_effect_checks_cannot_borrow_another_predictions_signal(self):
        measured = [{"id": "other", "status": "PASS"}]
        self.assertEqual(batch.effect_checks({"check": {"signal_ids": []}}, measured)[0]["status"], "UNKNOWN")
        self.assertEqual(batch.effect_checks({"check": {"signal_ids": ["missing"]}}, measured)[0]["status"], "UNKNOWN")
        self.assertEqual(batch.effect_checks({"check": {"signal_ids": ["other"]}}, measured)[0]["status"], "PASS")

    def test_custom_controls_rejected_not_ignored(self):
        self.change(lambda value: value.update(controls=[{"id": "ignored"}]), "experiment.json")
        with self.assertRaisesRegex(ValueError, "custom controls"):
            batch.validate(self.path)

    def test_missing_compiler_blocks_before_any_compile(self):
        with mock.patch.dict(os.environ, {"IDO_DIR": str(self.root / "missing")}):
            with self.assertRaisesRegex(ValueError, "compiler missing"):
                batch.freeze(self.path, self.root / "run", 2)

    def test_experiment_o3_is_explicit_and_calibration_stays_exact_o2(self):
        self.change(lambda value: value.update(flags=batch.O3_FLAGS))
        with self.assertRaisesRegex(ValueError, "recipe"):
            batch.validate(self.path)
        self.change(lambda value: value.update(purpose="experiment", baseline_expectation={"differing": 4, "total": 445, "extra_words": 0}))
        plan, _, _ = batch.validate(self.path)
        self.assertEqual(plan["flags"], batch.O3_FLAGS)
        for replacement in ({"differing": 0, "total": 445, "extra_words": 0},
                            {"differing": True, "total": 445, "extra_words": 0},
                            {"differing": 446, "total": 445, "extra_words": 0}):
            self.change(lambda value: value.update(baseline_expectation=replacement))
            with self.assertRaisesRegex(ValueError, "baseline expectation"):
                batch.validate(self.path)
        self.change(lambda value: value.update(purpose="calibration", flags=batch.FLAGS))
        with self.assertRaisesRegex(ValueError, "cannot override"):
            batch.validate(self.path)


class ArtifactTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
        batch.write_json(self.root / "run-identity.json", {"run_id": "test"})
        self.file = self.root / "input"
        self.file.write_bytes(b"input")
        envelope = {"files": {"input": batch.file_hash(self.file)},
                    "python": {"path": sys.executable, "sha256": batch.file_hash(sys.executable)}}
        self.state = {"envelope": envelope, "envelope_sha256": batch.identity(envelope),
                      "plan": {}, "plan_sha256": batch.identity({})}
        batch.write_json(self.root / "frozen.json", self.state)

    def tearDown(self):
        self.tmp.cleanup()

    def test_receipt_requires_complete_artifact_and_unchanged_context(self):
        obj = self.root / "candidate.o"
        obj.write_bytes(b"full object")
        receipt = self.root / "compile.receipt.json"
        batch.receipt(self.root, receipt, obj)
        self.assertTrue(batch.valid_receipt(self.root, receipt))
        obj.write_bytes(b"corruption")
        self.assertFalse(batch.valid_receipt(self.root, receipt))
        obj.write_bytes(b"full object")
        self.file.write_bytes(b"changed header, symbol, own-data or compiler")
        self.assertFalse(batch.valid_receipt(self.root, receipt))

    def test_interrupted_receipt_and_changed_digest_rejected(self):
        receipt = self.root / "receipt.json"
        receipt.with_suffix(".json.tmp").write_text('{"complete":')
        self.assertFalse(batch.valid_receipt(self.root, receipt))
        self.state["envelope"]["files"]["input"] = "0" * 64
        batch.write_json(self.root / "frozen.json", self.state)
        with self.assertRaisesRegex(ValueError, "identity changed"):
            batch.verify_frozen(self.root)

    def test_malformed_elf_and_missing_symbol_have_distinct_status(self):
        self.state["plan"] = {"targets": "asm/us/blob", "function": "f"}
        self.state["plan_sha256"] = batch.identity(self.state["plan"])
        batch.write_json(self.root / "frozen.json", self.state)
        scorer = mock.Mock()
        scorer._elf.side_effect = SystemExit("bad ELF")
        destination = self.root / "score.json"
        with mock.patch.object(batch, "load_score", return_value=scorer):
            batch.strict_score(self.root, self.file, destination)
            self.assertEqual(batch.read_json(destination)["status"], "invalid_elf")
            scorer._elf.side_effect = None
            scorer.symbols.return_value = {}
            batch.strict_score(self.root, self.file, destination)
            self.assertEqual(batch.read_json(destination)["status"], "missing_symbol")

    def test_missing_or_malformed_score_json_is_failure(self):
        self.state["plan"] = {"limits": {"score_seconds": 1}}
        batch.write_json(self.root / "frozen.json", self.state)
        with mock.patch.object(batch, "stage", return_value={"status": "ok", "seconds": 0}):
            self.assertEqual(batch.score_object(self.root, "A01", self.file)["status"], "strict_comparison_failure")
            (self.root / "scores").mkdir()
            (self.root / "scores/A01.json").write_text('[]')
            self.assertEqual(batch.score_object(self.root, "A01", self.file)["status"], "strict_comparison_failure")

    def test_whole_elf_dedup_does_not_collapse_distinct_relocations(self):
        self.state["jobs"] = 2
        self.state["plan"] = {"predictions": {key: {"sha256": key, "source": key + ".c", "check": {"signal_ids": []}} for key in ("A01", "A02", "A03")}}
        batch.write_json(self.root / "frozen.json", self.state)
        for key, content in (("A01", b"same text|reloc foo"), ("A02", b"same text|reloc bar"), ("A03", b"same text|reloc foo")):
            path = self.root / "compiles" / key
            path.mkdir(parents=True)
            (path / "candidate.o").write_bytes(content)
        with mock.patch.object(batch, "valid_receipt", return_value=True), mock.patch.object(batch, "score_object", return_value={"status": "scored_mismatch"}):
            rows = batch.collect_results(self.root)
        self.assertEqual(rows[0]["elf_group"], rows[2]["elf_group"])
        self.assertNotEqual(rows[0]["elf_group"], rows[1]["elf_group"])
        self.assertEqual(rows[0]["effect_checks"][0]["status"], "UNKNOWN")

    def test_failed_baseline_stops_before_canary_or_batch(self):
        self.state["plan"] = {"baseline": "baseline.c", "limits": {"score_seconds": 1, "compile_seconds": 1}}
        batch.write_json(self.root / "frozen.json", self.state)
        with mock.patch.object(batch, "stage", return_value={"status": "failed"}), mock.patch.object(batch, "score_object") as score:
            with self.assertRaisesRegex(ValueError, "target preparation failed"):
                batch.baseline_controls(self.root)
            score.assert_not_called()

    def test_public_export_allowlist_and_six_row_cap(self):
        secret = "PRIVATE raw assembly sentinel"
        rows = [{"id": f"A{i:02}", "status": "scored_mismatch", "source_sha256": "a" * 64,
                 "strict": {"differing": i, "extra_words": 0, "notes": [secret], "errors": [secret]},
                 "elf_group": i, "effect_checks": [{"status": "UNKNOWN"}], "diagnosis": secret} for i in range(1, 21)]
        summary = {"schema": batch.SCHEMA, "status": "completed", "variants": rows,
                   "baseline": [{"status": "strict_exact_candidate", "strict": {"differing": 0, "extra_words": 0}}], "blocker": secret}
        batch.report(self.root, summary)
        public = (self.root / "public-summary.json").read_text()
        markdown = (self.root / "summary.md").read_text()
        self.assertNotIn(secret, public + markdown)
        self.assertEqual(len(json.loads(public)["variants"]), 20)
        self.assertLessEqual(len([line for line in markdown.splitlines() if line.startswith("| ")]) - 1, 6)


@unittest.skipUnless(os.name == "posix", "process-group contract is POSIX-only")
class ProcessTests(unittest.TestCase):
    def test_timeout_kills_child_and_grandchild(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            pid = root / "child.pid"
            marker = root / "escaped"
            child = f"import time,pathlib; time.sleep(1.5); pathlib.Path({str(marker)!r}).write_text('leak')"
            command = [sys.executable, "-c", f"import subprocess,time,pathlib; p=subprocess.Popen([{sys.executable!r},'-c',{child!r}]); pathlib.Path({str(pid)!r}).write_text(str(p.pid)); time.sleep(10)"]
            result = batch.process(command, root, {"PATH": os.environ["PATH"]}, .3, root / "stage")
            self.assertEqual(result["status"], "timeout")
            time.sleep(1.5)
            self.assertFalse(marker.exists())
            self.assertTrue(pid.exists())


class ExperimentModeTests(unittest.TestCase):
    def controls(self, differing=4, total=445, extra=0):
        strict = {"accepted": differing == 0 and extra == 0, "differing": differing,
                  "total": total, "extra_words": extra, "errors": [], "unresolved": [], "unverified": [], "notes": []}
        row = {"status": "strict_exact_candidate" if strict["accepted"] else "scored_mismatch",
               "strict": strict, "object_sha256": "a" * 64}
        return [copy.deepcopy(row), copy.deepcopy(row)]

    def test_repeatable_explicit_nonmatch_is_not_match(self):
        plan = {"purpose": "experiment", "baseline_expectation": {"differing": 4, "total": 445, "extra_words": 0}}
        controls = self.controls()
        batch.validate_baseline_evidence(plan, controls)
        self.assertFalse(controls[0]["strict"]["accepted"])
        self.assertEqual(controls[0]["status"], "scored_mismatch")
        for field, value in (("differing", 5), ("total", 444), ("extra_words", 1)):
            changed = copy.deepcopy(plan)
            changed["baseline_expectation"][field] = value
            with self.assertRaisesRegex(ValueError, "explicit nonmatch"):
                batch.validate_baseline_evidence(changed, controls)

    def test_repeatable_nonmatch_cannot_relax_calibration(self):
        with self.assertRaisesRegex(ValueError, "declared mode"):
            batch.validate_baseline_evidence({"purpose": "calibration"}, self.controls())
        batch.validate_baseline_evidence({"purpose": "calibration"}, self.controls(0))

    def test_repeatability_checks_whole_elf_and_strict_evidence(self):
        plan = {"purpose": "experiment", "baseline_expectation": {"differing": 4, "total": 445, "extra_words": 0}}
        controls = self.controls()
        controls[1]["object_sha256"] = "b" * 64
        with self.assertRaisesRegex(ValueError, "whole ELF"):
            batch.validate_baseline_evidence(plan, controls)
        for field in ("errors", "unresolved", "unverified"):
            controls = self.controls()
            for row in controls:
                row["strict"][field] = ["blocked"]
            with self.assertRaisesRegex(ValueError, "blocked"):
                batch.validate_baseline_evidence(plan, controls)
        with self.assertRaisesRegex(ValueError, "declared mode"):
            batch.validate_baseline_evidence(plan, self.controls(0))

    def test_frozen_macro_definitions_not_include_closure(self):
        allowed = '#define STEP(x) ((x) + 1)\nvoid f(void) { int i = STEP(1); }\n'
        self.assertTrue(batch.frozen_prefix_macros_only(allowed, "f"))
        for prefix in ('#include "a.h"\n', '/* comment */ # include "a.h"\n', '#inc\\\nlude "a.h"\n', '%:include "a.h"\n', '#pragma optimize\n'):
            self.assertFalse(batch.frozen_prefix_macros_only(prefix + allowed, "f"))
        self.assertFalse(batch.frozen_prefix_macros_only(allowed + '#define OTHER 1\n', "f"))
        self.assertFalse(batch.frozen_prefix_macros_only('void f(void) {\n#define OTHER 1\n}', "f"))


class TimingTests(unittest.TestCase):
    def test_append_only_spans_preserve_utc_monotonic_failure_and_correlation(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            batch.write_json(root / "run-identity.json", {"run_id": "timing-test"})
            with batch.phase(root, "compile", "A01"):
                pass
            before = (root / "events.jsonl").read_bytes()
            with self.assertRaises(ValueError):
                with batch.phase(root, "score", "A01"):
                    raise ValueError("private content must not enter event log")
            self.assertTrue((root / "events.jsonl").read_bytes().startswith(before))
            records = [json.loads(line) for line in (root / "events.jsonl").read_text().splitlines()]
            self.assertEqual(len(records), 4)
            self.assertTrue(all(row["run_id"] == "timing-test" and row["variant"] == "A01" for row in records))
            self.assertTrue(all(row["utc"].endswith("+00:00") for row in records))
            self.assertEqual(records[-1]["status"], "failed")
            self.assertNotIn("private content", (root / "events.jsonl").read_text())
            result = batch.timing_summary(root)
            self.assertEqual(result["open_spans"], [])
            self.assertGreaterEqual(result["stages"]["compile"]["summed_elapsed_seconds"], 0)
            self.assertIsNone(result["cpu_seconds"])
            self.assertIsNone(result["llm_tokens"])

    def test_run_failure_and_cancellation_write_terminal_summary(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            with mock.patch.object(batch, "validate", side_effect=ValueError("invalid plan")):
                result = batch.run(root / "missing-plan", root / "validation-failure")
            self.assertEqual(result["status"], "blocked")
            self.assertTrue((root / "validation-failure/summary.json").exists())
            self.assertEqual(result["timings"]["open_spans"], [])
            plan = {"limits": {"jobs": 2}, "base_commit": "a" * 40, "purpose": "experiment"}
            state = {"plan_sha256": "b" * 64, "envelope_sha256": "c" * 64,
                     "envelope": {"effective_flags": batch.O3_FLAGS}}
            with mock.patch.object(batch, "validate", return_value=(plan, None, None)), mock.patch.object(batch, "_freeze", side_effect=ValueError("missing compiler")):
                result = batch.run(root / "mock-plan", root / "freeze-failure")
            self.assertEqual(result["status"], "blocked")
            self.assertEqual(result["timings"]["open_spans"], [])
            self.assertTrue((root / "freeze-failure/summary.json").exists())
            with mock.patch.object(batch, "validate", return_value=(plan, None, None)), mock.patch.object(batch, "_freeze", return_value=state), mock.patch.object(batch, "baseline_controls", side_effect=KeyboardInterrupt()):
                result = batch.run(root / "mock-plan", root / "cancelled")
            self.assertEqual(result["status"], "cancelled")
            self.assertTrue((root / "cancelled/summary.json").exists())
            self.assertEqual(result["timings"]["open_spans"], [])

    def test_cancel_and_incomplete_child_spans_are_visible(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            batch.write_json(root / "run-identity.json", {"run_id": "timing-test"})
            with self.assertRaises(KeyboardInterrupt):
                with batch.phase(root, "compile", "A01"):
                    raise KeyboardInterrupt()
            batch.event(root, "compiler_process", "start", variant="A02", span_id="interrupted")
            result = batch.timing_summary(root)
            self.assertEqual(result["per_variant"][0]["status"], "cancelled")
            self.assertEqual(result["open_spans"], [{"stage": "compiler_process", "variant": "A02"}])


if __name__ == "__main__":
    unittest.main()


class SelectedFunctionCanaryTests(unittest.TestCase):
    def scorer(self):
        scorer = mock.Mock()
        scorer._elf.return_value = (b"", [{"off": 100, "type": 1, "info": 0}])
        scorer._text_index.return_value = 0
        scorer.symbols.return_value = {"helper": 0, "selected": 8, "after": 20}
        scorer.text_words.return_value = [1, 2, 33, 4, 5, 6]
        scorer.targets.return_value = {"selected": [3, 4, 5, 6]}
        return scorer

    def test_skips_other_helper_and_already_mismatching_prologue(self):
        scorer = self.scorer()
        self.assertEqual(batch.negative_canary_offset(scorer, "dummy", "selected"), 112)

    def test_skips_relocation_words(self):
        import struct
        scorer = self.scorer()
        scorer._elf.return_value = (struct.pack(">II", 12, 4), [
            {"off": 100, "type": 1, "info": 0},
            {"off": 0, "type": 9, "info": 0, "size": 8}])
        self.assertEqual(batch.negative_canary_offset(scorer, "dummy", "selected"), 116)

    def test_does_not_use_matching_next_function(self):
        scorer = self.scorer()
        scorer.text_words.return_value = [1, 2, 33, 44, 55, 6]
        with self.assertRaisesRegex(ValueError, "no matched relocation-free"):
            batch.negative_canary_offset(scorer, "dummy", "selected")
