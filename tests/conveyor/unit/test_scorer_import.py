def test_scorer_importable():
    from tools.conveyor.jobs._permuter import Scorer

    assert Scorer.PENALTY_INF > 0
    assert hasattr(Scorer, "score")
