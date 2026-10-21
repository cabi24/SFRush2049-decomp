"""w36n anchor table: N64 function -> GC function pairs stated by earlier, verified work.

Each entry: (n64 id, gc key, kind, evidence). kind:
  applied   name applied from GC evidence by w29n/w30n/w32n (docs/external/gc/names.md; renames.json)
  matched   N64 function is locked C ported from the GC function (header/receipt says so)
  stated    a lane proved/stated the identity with evidence (cited)
Libc/libm/inflate seeds pair same-named static functions (correlate.py SEED_NAMES).
"""

SEEDS = ['sprintf', 'strlen', 'strchr', 'memcpy', 'memset', 'memchr', 'sinf', 'cosf', 'modf',
         'huft_build', 'inflate_block', 'inflate_dynamic', 'inflate_fixed', 'inflate_stored', 'strcpy',
         'strcmp', 'strcat', 'strncpy', 'qsort', 'rand', 'srand', 'atoi', 'abs', 'fabs']

# names.md / renames.json (accepted, GC-sourced rows). n64 name -> (gc key, evidence)
APPLIED = {
    'ShowCountdown': ('ShowCountdown', 'names.md w29n: matched w28c, corr 5.35, str CNTDWN%d'),
    'PrintScores': ('_PrintScores', 'names.md w29n: matched w28a, corr 6.51'),
    'SetHighScoreState': ('_SetHighScoreState', 'names.md w29n: matched w28a, corr 4.4'),
    'animLapTime': ('animLapTime', 'names.md w29n: matched w28c (GC locals close the frame)'),
    'CreatePlayer': ('CreatePlayer', 'names.md w29n: call-for-call + homes = GC locals (w28b)'),
    'SaveServer': ('SaveServer', "names.md w29n: structure + save_settings' (0,0) call (w28b)"),
    'SetupWaveModel': ('_SetupWaveModel', 'names.md w29n: FourCC chunk tags, homes = GC locals, corr 9.48'),
    'PS_CreateFile': ('PS_CreateFile', "names.md w29n: CreatePlayer's (pak,file,'P',0x810,0x3544,'NRUE') call"),
    'PS_WriteFile': ('PS_WriteFile', 'names.md w29n: call in CreatePlayer; locked'),
    'PS_FlushFile': ('PS_FlushFile', 'names.md w29n: call order in CreatePlayer; locked'),
    'PS_FlushPak': ('PS_FlushPak', 'names.md w29n: call order in CreatePlayer; locked'),
    'FindGhosts': ('_FindGhosts', 'names.md w29n: matched w29a'),
    'drawMain': ('drawMain', 'names.md w29n: matched w29a (GC locals/loop)'),
    'model_iteration': ('model_iteration', 'names.md w29n: matched w29c from arcade mdrive.c'),
    'animElapsedTimerText': ('animElapsedTimerText', 'names.md w29n: matched w29c (GC locals)'),
    'animPlace': ('animPlace', 'names.md w29n: w29c draft, GC locals'),
    'animControls': ('animControls', 'names.md w30n: corr 3.76, str CONTROLS_SM'),
    'LoadTrack': ('LoadTrack', 'names.md w30n: corr 12.96 (track-name table x19, SkyInit/InitSmoke)'),
    'highscore_InitScreen': ('_InitScreen@highscorescr.c', 'names.md w32n: matched w31d from GC _InitScreen'),
    'DrawName': ('DrawName', 'names.md w32n: w31a draft EQUAL in the 5480 unit'),
    'DrawGhost': ('DrawGhost', 'names.md w32n: w31a draft 17/601; only caller highscore_animText state 4'),
    'DrawUnlocked': ('DrawUnlocked', 'names.md w32n: corr 3.8; draft EQUIVALENT'),
    'highscore_animText': ('animText@highscorescr.c', 'names.md w32n: draft EQUIVALENT; calls DrawName/DrawGhost/DrawUnlocked'),
    'animResults': ('animResults', 'names.md w32n: corr 2.8 (str "%s\\n%s"); w31a draft'),
    'SaveGhost': ('SaveGhost', 'names.md w32n: matched w31c'),
    'CompressGhost': ('CompressGhost', 'names.md w32n: ghost-save chain before SaveGhost; draft 9/190'),
    'drawBattleScore': ('drawBattleScore', 'names.md w32n: matched w30a'),
    'animWrongWay': ('animWrongWay', 'names.md w32n: matched w30b'),
    'StartCone': ('_StartCone', 'names.md w32n: matched w30b'),
    'options_animText': ('animText@optionscr.c', 'names.md w32n: matched w31i'),
    'options_animButtonText': ('animButtonText@optionscr.c', 'names.md w32n: matched w31i'),
    'options_printOption': ('printOption@optionscr.c', 'names.md w32n: matched w31i'),
    'audio_animText': ('animText@audioscr.c', 'names.md w32n: draft EQUIVALENT 1,273 calls'),
    'audio_animButtonText': ('animButtonText@audioscr.c', 'names.md w32n: same unit as audio_animText'),
    'controls_animText': ('animText@controlscr.c', 'names.md w32n: draft EQUIVALENT 1,949 calls'),
    'controls_animButtonText': ('animButtonText@controlscr.c', 'names.md w32n: same unit as controls_animText'),
    'GhostStart': ('GhostStart', 'names.md w32n: matched w32c from GC GhostStart'),
}

# identities stated in matched sources / lane RESULTS (filled by harvest; see RESULTS.md)
STATED = []
