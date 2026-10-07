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


class ArtifactTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name)
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


if __name__ == "__main__":
    unittest.main()
