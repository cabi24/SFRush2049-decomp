# Actual export-boundary control

Same complete source and exact compiler flags as the frozen parent packet. Only three context keep entries were removed: entity_hierarchy_update, stat_lap_complete and game_timer_resume. All are real native/source functions, but current direct-call and initialized-address scans found no outside references. The actual E05F0/D5E64 entries and runtime helpers with true outside callers remain kept. No synthetic callers or source operations were added.

This complete replay has no newly exact member. Main remains292/332 differences and the impact handler699/744; all eight member word counts and diagnostic results are identical to the parent baseline. The keep change does not improve this closure. Source hashes are unchanged. Existing protected table placement is still unverified; no coverage is claimed. No additional controls followed.
