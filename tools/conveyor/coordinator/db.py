"""SQLite state store for the conveyor coordinator.

Single-writer (the coordinator process), WAL mode, one file under the data
directory. Schema follows specs/001-matching-pipeline/data-model.md.
"""
import contextlib
import sqlite3
from pathlib import Path

SCHEMA_VERSION = 1

_SCHEMA = """
CREATE TABLE IF NOT EXISTS meta (
    key TEXT PRIMARY KEY,
    value TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS node (
    node_id TEXT PRIMARY KEY,
    hostname TEXT,
    capabilities TEXT NOT NULL DEFAULT '[]',
    cores INTEGER NOT NULL DEFAULT 1,
    agent_version TEXT,
    last_seen TEXT
);

CREATE TABLE IF NOT EXISTS blob (
    sha256 TEXT PRIMARY KEY,
    kind TEXT NOT NULL,
    size_bytes INTEGER NOT NULL,
    created_at TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS work_unit (
    job_id TEXT PRIMARY KEY,
    job_type TEXT NOT NULL,
    target_id TEXT,
    manifest_sha TEXT NOT NULL,
    bundle_sha TEXT,
    toolkit_sha TEXT,
    required_capability TEXT NOT NULL DEFAULT 'x86_64',
    priority INTEGER NOT NULL DEFAULT 100,
    state TEXT NOT NULL DEFAULT 'PENDING',
    attempt INTEGER NOT NULL DEFAULT 0,
    max_attempts INTEGER,
    leased_by TEXT,
    lease_expires TEXT,
    budget TEXT,
    best_score INTEGER,
    best_source_sha TEXT,
    result_sha TEXT,
    result_ok INTEGER,
    ingested_at TEXT,
    created_at TEXT NOT NULL,
    updated_at TEXT NOT NULL
);
CREATE INDEX IF NOT EXISTS idx_work_state_priority
    ON work_unit(state, required_capability, priority, created_at);
CREATE INDEX IF NOT EXISTS idx_work_manifest ON work_unit(manifest_sha);

CREATE TABLE IF NOT EXISTS arcade_candidate (
    candidate_id TEXT PRIMARY KEY,
    name TEXT NOT NULL,
    src_file TEXT NOT NULL,
    body_sha TEXT NOT NULL,
    compile_status TEXT NOT NULL DEFAULT '{}',
    object_shas TEXT NOT NULL DEFAULT '{}'
);

CREATE TABLE IF NOT EXISTS n64_target (
    target_id TEXT PRIMARY KEY,
    address INTEGER,
    population TEXT NOT NULL,
    insn_count INTEGER,
    target_asm_sha TEXT,
    target_o_sha TEXT,
    fingerprint TEXT,
    cluster_id TEXT
);

CREATE TABLE IF NOT EXISTS matrix_entry (
    target_id TEXT NOT NULL,
    candidate_id TEXT NOT NULL,
    flagset TEXT NOT NULL,
    toolkit_sha TEXT NOT NULL,
    score INTEGER NOT NULL,
    PRIMARY KEY (target_id, candidate_id, flagset, toolkit_sha)
);
CREATE INDEX IF NOT EXISTS idx_matrix_target_score ON matrix_entry(target_id, score);

CREATE TABLE IF NOT EXISTS flag_registry (
    translation_unit TEXT PRIMARY KEY,
    pinned_flagset TEXT NOT NULL,
    evidence TEXT NOT NULL DEFAULT '[]',
    source TEXT NOT NULL DEFAULT 'sweep'
);

CREATE TABLE IF NOT EXISTS cluster (
    cluster_id TEXT PRIMARY KEY,
    member_ids TEXT NOT NULL DEFAULT '[]',
    similarity TEXT NOT NULL DEFAULT '{}'
);

CREATE TABLE IF NOT EXISTS function_status (
    target_id TEXT PRIMARY KEY,
    status TEXT NOT NULL DEFAULT 'unmatched',
    best_score INTEGER,
    best_candidate_id TEXT,
    seed_kind TEXT,
    seed_source_sha TEXT,
    flagset TEXT,
    human_flag TEXT,
    override TEXT,
    updated_at TEXT NOT NULL
);
CREATE INDEX IF NOT EXISTS idx_status_status ON function_status(status);

CREATE TABLE IF NOT EXISTS promotion_record (
    promotion_id TEXT PRIMARY KEY,
    target_id TEXT NOT NULL,
    source_sha TEXT NOT NULL,
    search_job_id TEXT,
    build_ok INTEGER,
    sha1_ok INTEGER,
    commit_hash TEXT,
    doc_header_injected INTEGER NOT NULL DEFAULT 0,
    outcome TEXT NOT NULL,
    created_at TEXT NOT NULL
);
CREATE UNIQUE INDEX IF NOT EXISTS idx_one_promotion_per_target
    ON promotion_record(target_id) WHERE outcome = 'promoted';

-- Corpus roots (002): registered local git clones serving as candidate
-- sources alongside the arcade tree. See specs/002-corpus-candidates.
CREATE TABLE IF NOT EXISTS corpus_root (
    origin TEXT PRIMARY KEY,
    path TEXT NOT NULL,
    repo_url TEXT NOT NULL,
    commit_sha TEXT NOT NULL,
    include_dirs TEXT NOT NULL DEFAULT '[]',
    ingested_at TEXT
);
"""

# Additive migrations for databases created before these columns existed.
_MIGRATIONS = (
    "ALTER TABLE work_unit ADD COLUMN result_ok INTEGER",
    "ALTER TABLE work_unit ADD COLUMN ingested_at TEXT",
    "ALTER TABLE function_status ADD COLUMN seed_kind TEXT",
    "ALTER TABLE function_status ADD COLUMN seed_source_sha TEXT",
)

# Additive column migrations for 002 (corpus candidates), guarded by
# PRAGMA table_info so re-connecting to an already-migrated DB is a no-op.
# arcade_candidate rows created before 002 read as origin 'arcade' via the
# column default; matrix_entry cells from pre-002 toolkits carry NULL
# score_reloc_blind (data-model.md).
_COLUMN_MIGRATIONS = (
    ("arcade_candidate", "origin",
     "ALTER TABLE arcade_candidate ADD COLUMN origin TEXT NOT NULL DEFAULT 'arcade'"),
    ("arcade_candidate", "provenance",
     "ALTER TABLE arcade_candidate ADD COLUMN provenance TEXT NOT NULL DEFAULT '{}'"),
    ("matrix_entry", "score_reloc_blind",
     "ALTER TABLE matrix_entry ADD COLUMN score_reloc_blind INTEGER"),
)


def connect(db_path):
    """Open (creating if needed) the coordinator database."""
    db_path = Path(db_path)
    db_path.parent.mkdir(parents=True, exist_ok=True)
    conn = sqlite3.connect(str(db_path), timeout=30.0, check_same_thread=False)
    conn.row_factory = sqlite3.Row
    conn.execute("PRAGMA journal_mode=WAL")
    conn.execute("PRAGMA synchronous=NORMAL")
    conn.execute("PRAGMA foreign_keys=ON")
    conn.executescript(_SCHEMA)
    for migration in _MIGRATIONS:
        try:
            conn.execute(migration)
        except sqlite3.OperationalError:
            pass  # column already exists
    for table, column, ddl in _COLUMN_MIGRATIONS:
        cols = {r["name"] for r in conn.execute(f"PRAGMA table_info({table})")}
        if column not in cols:
            conn.execute(ddl)
    conn.execute(
        "INSERT OR IGNORE INTO meta(key, value) VALUES ('schema_version', ?)",
        (str(SCHEMA_VERSION),),
    )
    conn.commit()
    return conn


@contextlib.contextmanager
def tx(conn):
    """Transaction scope: commits on success, rolls back on any exception."""
    try:
        yield conn
        conn.commit()
    except BaseException:
        conn.rollback()
        raise


def get_meta(conn, key, default=None):
    row = conn.execute("SELECT value FROM meta WHERE key = ?", (key,)).fetchone()
    return row["value"] if row else default


def set_meta(conn, key, value):
    with tx(conn):
        conn.execute(
            "INSERT INTO meta(key, value) VALUES (?, ?) "
            "ON CONFLICT(key) DO UPDATE SET value = excluded.value",
            (key, value),
        )
