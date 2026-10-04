/* Compile-only native layout checks; no submission helpers or runtime bodies. */
#include "nonmatch/func_80010E80.c"
#define OFFSET(type, field) ((unsigned int) &(((type *) 0)->field))
typedef char pointer_is_four[(sizeof(void *) == 4) ? 1 : -1];
typedef char input_size[(sizeof(AudioConfiguration) == 38) ? 1 : -1];
typedef char input_delays[(OFFSET(AudioConfiguration, delays) == 2) ? 1 : -1];
typedef char input_gains[(OFFSET(AudioConfiguration, gains) == 18) ? 1 : -1];
typedef char input_count[(OFFSET(AudioConfiguration, count) == 26) ? 1 : -1];
typedef char input_feedback[(OFFSET(AudioConfiguration, feedback) == 27) ? 1 : -1];
typedef char input_mode[(OFFSET(AudioConfiguration, mode) == 28) ? 1 : -1];
typedef char input_filter[(OFFSET(AudioConfiguration, filter) == 30) ? 1 : -1];
typedef char state_size[(sizeof(AudioDelayState) == 72) ? 1 : -1];
typedef char state_length[(OFFSET(AudioDelayState, length) == 4) ? 1 : -1];
typedef char state_count[(OFFSET(AudioDelayState, count) == 8) ? 1 : -1];
typedef char state_feedback[(OFFSET(AudioDelayState, feedback) == 10) ? 1 : -1];
typedef char state_delays[(OFFSET(AudioDelayState, delays) == 12) ? 1 : -1];
typedef char state_gains[(OFFSET(AudioDelayState, gains) == 44) ? 1 : -1];
typedef char state_filter[(OFFSET(AudioDelayState, filter) == 64) ? 1 : -1];
