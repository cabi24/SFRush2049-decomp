# w24s - static integration of w24b's results (worktree lane)

Branch `w24s` (worktree `.claude/worktrees/w24s`, from wave24 1dde4ccb). Not pushed. Static numbers only.
Status: in progress. Base: static 604/668, 135,732 B (84.45%).

Builder gate repo: `watchman2:~/rush2049/scratch/frontier/w24s/repo` (copy of `~/rush2049/repo` without `.git`;
promote_batch with `CONVEYOR_BUILDER_REPO=<that> CONVEYOR_BUILDER_JOBS=4`).
DB backups (Pi scratchpad): `conveyor.db.w24s.bak` (+ `-wal`/`-shm`, `cp -p`), `conveyor.db.w24s.bak2` (sqlite backup API).
