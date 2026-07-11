"""Bootstrap seeding of flag_registry from the COMPILER_SETTINGS.md pins."""
import pytest

from tools.conveyor.coordinator import db as dbmod
from tools.conveyor.pipeline import flags as flagsmod


@pytest.fixture
def conn(tmp_path):
    c = dbmod.connect(tmp_path / "db.sqlite")
    yield c
    c.close()


def _pins(conn):
    return {r["translation_unit"]: (r["pinned_flagset"], r["source"])
            for r in conn.execute("SELECT * FROM flag_registry")}


def test_seed_confirmed_writes_known_pins(conn):
    n = flagsmod.seed_confirmed(conn)
    assert n == len(flagsmod.CONFIRMED)
    pins = _pins(conn)
    for tu, flagset in flagsmod.CONFIRMED.items():
        assert pins[tu] == (flagset, "confirmed")


def test_seed_confirmed_is_idempotent(conn):
    flagsmod.seed_confirmed(conn)
    assert flagsmod.seed_confirmed(conn) == 0
    assert len(_pins(conn)) == len(flagsmod.CONFIRMED)


def test_seed_confirmed_never_clobbers_a_manual_override(conn):
    tu = next(iter(flagsmod.CONFIRMED))
    conn.execute(
        "INSERT INTO flag_registry (translation_unit, pinned_flagset,"
        " evidence, source) VALUES (?, '-CUSTOM', '[]', 'manual_override')",
        (tu,))
    conn.commit()

    assert flagsmod.seed_confirmed(conn) == len(flagsmod.CONFIRMED) - 1
    flagset, source = _pins(conn)[tu]
    assert (flagset, source) == ("-CUSTOM", "manual_override")
