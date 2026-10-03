"""Packet 1 focused metadata tests; standard library only, no compiler or ROM."""
import copy
import csv
import importlib.util
import io
import hashlib
import json
from pathlib import Path
import subprocess
import sys
import unittest

WORK = Path(__file__).resolve().parents[1]
ROOT = WORK.parents[2]


def load(name):
    spec = importlib.util.spec_from_file_location(name, WORK / "scripts" / (name + ".py"))
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


generate = load("generate")
screen = load("screen_opcodes")


class GeneratorTests(unittest.TestCase):
    def setUp(self):
        self.rows = json.loads((ROOT / "specs/015-boot-tail-runtime/inventory.json").read_text())["functions"]
        self.research = json.loads((WORK / "research.json").read_text())
        self.names = generate.symbol_names((ROOT / "symbol_addrs.us.txt").read_text())

    def build(self):
        return generate.build(self.rows, self.research, self.names, {"test": True})

    def test_exact_population_and_status(self):
        status, result = self.build()
        ledger = list(csv.DictReader(io.StringIO(status)))
        self.assertEqual(len(ledger), 412)
        self.assertEqual(sum(int(r["size"]) for r in ledger), 95012)
        self.assertEqual(set(ledger[0]), set(generate.FIELDS))
        expected_open = sum(self.research["function_annotations"].get(r["address"], {}).get("status", "open") == "open"
                            for r in self.rows if r["scope"] == "in_scope")
        self.assertEqual(sum(r["status"] == "open" for r in ledger), expected_open)
        verified = [r for r in ledger if r["status"] == "verified_body"]
        self.assertEqual([(r["address"], r["size"]) for r in verified], [("0x80010A00", "12")])
        self.assertIn("Packet2", verified[0]["note"])
        self.assertEqual(result["summary"]["new_verified_bytes"], 0)
        self.assertEqual(result["summary"]["game_called_in_scope"], 20)

    def test_clusters_exact_disjoint_and_contiguous(self):
        _, result = self.build()
        cs = result["clusters"]
        members = [a for c in cs for a in c["members"]]
        self.assertEqual(len(members), len(set(members)))
        self.assertEqual(set(members), {r["address"] for r in self.rows if r["scope"] == "in_scope"})
        self.assertEqual(sum(c["bytes"] for c in cs), 95012)
        for a, b in zip(cs, cs[1:]):
            self.assertEqual(a["interval"]["end_exclusive"], b["interval"]["start"])
        for c in cs:
            g = c["graph"]
            self.assertEqual(g["total_tail_edges"], g["internal_tail_edges"] + g["cross_cluster_tail_edges"] + g["excluded_tail_edges"])
            if g["total_tail_edges"]:
                self.assertGreaterEqual(g["internal_fraction"], 0.5)

    def test_reproducible(self):
        self.assertEqual(self.build(), self.build())
        result = subprocess.run([sys.executable, str(WORK / "scripts/generate.py"), "--check"], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_duplicate_start_rejected(self):
        self.rows[1]["address"] = self.rows[0]["address"]
        with self.assertRaisesRegex(ValueError, "sorted, unique"):
            self.build()

    def test_byte_drift_rejected(self):
        self.rows[-1]["size"] -= 4
        with self.assertRaisesRegex(ValueError, "byte totals"):
            self.build()

    def test_overlap_rejected(self):
        self.rows[0]["size"] += 16
        with self.assertRaisesRegex(ValueError, "overlapping"):
            self.build()

    def test_gap_and_cut_cohort_rejected(self):
        self.research["cohorts"][1]["start"] = "0x80010454"
        with self.assertRaisesRegex(ValueError, "contiguous"):
            self.build()
        self.research["cohorts"][0]["end"] = "0x80010454"
        with self.assertRaisesRegex(ValueError, "cut"):
            self.build()

    def test_unknown_tail_target_rejected(self):
        self.rows[0]["callees_tail"].append("0xFFFFFFFF")
        with self.assertRaisesRegex(ValueError, "unknown start"):
            self.build()

    def test_reverse_edge_corruption_rejected(self):
        self.rows[0]["callers_tail"].append("0xFFFFFFFF")
        with self.assertRaisesRegex(ValueError, "unknown start"):
            self.build()
        self.rows[0]["callers_tail"] = [self.rows[1]["address"]]
        with self.assertRaisesRegex(ValueError, "reciprocity"):
            self.build()

    def test_structural_annotation_rejected(self):
        for key in ["address", "name", "size", "cluster", "unknown"]:
            with self.subTest(key=key):
                self.research["function_annotations"]["0x8000F8D0"][key] = "bad"
                with self.assertRaisesRegex(ValueError, "structural"):
                    self.build()
                del self.research["function_annotations"]["0x8000F8D0"][key]

    def test_non_c_or_verified_rows_not_recommended(self):
        for state in ["non_c", "verified_body", "claimed", "excluded", "nonmatch"]:
            with self.subTest(state=state):
                self.research["function_annotations"]["0x80010A0C"] = dict(self.research["function_annotations"]["0x80010A00"], status=state)
                _, result = self.build()
                self.assertNotIn("0x80010A0C", [a for c in result["clusters"] for a in c["small_first_candidates"]])

    def test_blockers_not_recommended(self):
        _, result = self.build()
        candidates = [a for c in result["clusters"] for a in c["small_first_candidates"]]
        self.assertFalse(set(candidates) & set(result["matching_blockers"]))
        self.assertNotIn("0x80010A00", candidates)

    def test_derived_verified_totals(self):
        self.research["function_annotations"]["0x80010A00"]["status"] = "open"
        _, result = self.build()
        self.assertEqual(result["summary"]["preexisting_verified_bytes"], 0)
        self.research["function_annotations"]["0x80010A0C"] = dict(self.research["function_annotations"]["0x80010A00"], status="verified_body")
        _, result = self.build()
        self.assertEqual(result["summary"]["new_verified_bytes"], 8)

    def test_verified_status_requires_provenance(self):
        self.research["function_annotations"]["0x80010A0C"] = {"status": "verified_body"}
        with self.assertRaisesRegex(ValueError, "provenance"):
            self.build()

    def test_readme_headline_tracks_live_status(self):
        self.research["function_annotations"]["0x80010A0C"] = dict(self.research["function_annotations"]["0x80010A00"], status="verified_body")
        _, result = self.build()
        rendered = generate.readme(result)
        self.assertIn("%d open functions / %s B" % (result["summary"]["open_functions"], format(result["summary"]["open_bytes"], ",")), rendered)
        self.assertIn("1 newly verified bodies / 8 B", rendered)
        self.assertNotIn("@@", rendered)

    def test_reference_ids_and_revisions(self):
        refs = json.loads((WORK / "references.json").read_text())
        for c in self.research["cohorts"]:
            self.assertTrue(set(c["reference_ids"]).issubset(refs))
        for r in refs.values():
            self.assertEqual(len(r["revision"]), 40)
            self.assertEqual(len(r["paths"]), len(r["links"]))
            self.assertTrue(r["license"])
            self.assertTrue(all(r["revision"] in link for link in r["links"]))

    def test_native_observation_hashes(self):
        evidence = json.loads((WORK / "source_hashes.json").read_text())
        for path, expected in evidence["files"].items():
            self.assertEqual(hashlib.sha256((ROOT / path).read_bytes()).hexdigest(), expected)

    def test_static_labels_are_historical(self):
        self.assertEqual(self.names[0x800075E0], ["osJamMesg"])
        _, result = self.build()
        self.assertTrue(any(s["historical_names"] == ["osJamMesg"] for c in result["clusters"] for s in c["static_call_leads"]))

    def test_main_generator_has_no_target_requirement(self):
        source = (WORK / "scripts/generate.py").read_text()
        self.assertNotIn("asm/us/boot_tail", source)
        self.assertNotIn("subprocess", source)


class OpcodeScreenTests(unittest.TestCase):
    def test_synthetic_instruction_classes(self):
        for word, expected in [(0xBC800000, "cache"), (0x40026000, "mfc0"),
                               (0x40845800, "mtc0"), (0x42000018, "eret"),
                               (0x42000008, "cop0_other"), (0x0000000F, "sync"),
                               (0x03E00008, None), (0x00000000, None)]:
            self.assertEqual(screen.opcode_class(word), expected)

    def test_reproduce_native_metadata_only(self):
        result = screen.audit()
        saved = json.loads((WORK / "opcode_audit.json").read_text())
        self.assertEqual(result, saved)
        self.assertEqual(result["in_scope_positive_functions"], 0)
        self.assertEqual(result["hits"], [{"address": "0x8000FB90", "scope": "stub_unclassified", "offset": "0x0", "instruction_class": "mtc0"}])
        self.assertEqual(len(result["unclassified_calls_at_or_beyond_text_end"]), 9)
        self.assertEqual(len({x["target"] for x in result["unclassified_calls_at_or_beyond_text_end"]}), 4)
        self.assertEqual(len(result["candidates"]), 7)
        self.assertNotIn(".word", json.dumps(result))


if __name__ == "__main__":
    unittest.main()
