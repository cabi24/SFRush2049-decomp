/* Alternative expressions for the same observed tail. These are fragments,
 * not separate matching candidates and not extra ROM coverage claims.
 * Include types.h first and use with locals stats/model/sample_count/record.
 */
#if 0
/* Direct native arithmetic expression. Compound assignment also produces
 * exactly this float round trip; do not cast only the distance increment. */
stats->distance = (u32)((f32)stats->distance + model->distance / 528.0f);

/* Equivalent completed-run predicate with the special flag compound;
 * intended only as a source-shape candidate if schedule is the residual. */
if (model->finished != 0) {
    stats->completions++;
    if (model->place == 0) {
        if (D_80152744 >= 2) stats->firsts++;
    } else if (model->place == 1) {
        if (D_80152744 >= 3) stats->seconds++;
    } else if (model->place == 2) {
        if (D_80152744 >= 4) stats->thirds++;
    }
    if (D_80142760 != 0) {
        stats->enabled_completions++;
        if (D_80152734 == D_80144018[player]) {
            stats->enabled_full_runs++;
        }
    }
}
#endif
