# F5 validation — 2026-09-29

`tools/cloud/setup.sh` checks the downloaded IDO 5.3 v1.2 archive against
SHA-256 `ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506`
before extracting it. A checksum failure terminates setup without reporting
the compiler ready. Existing installations retain their previous behavior.

Validation:

- `bash -n tools/cloud/setup.sh`: passed.
- `test_cloud_setup.py`: 2 passed. Both malformed download bytes and a valid
  but incorrect compiler archive fail the real checksum check before `tar`
  runs. The tests also use an installation path containing spaces.
- A fresh installation in an isolated watchman2 directory downloaded the
  release, printed `ido.tgz: OK`, and completed successfully. That newly
  installed compiler produced `MATCH` for `sound_handles_clear` and all three
  `resource_slot_clear` group members.
- The Pi Conveyor suite excluding `node_required` tests completed with
  **352 passed, 131 skipped, 5 deselected, 1 pre-existing failure** in
  `test_closure.py::test_populate_keeps_suffix_row_conflicted_against_discovered_extent`.
