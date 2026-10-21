/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Wave 36 (w36w): stand-in callers (context, kept) for time_result_display = _TitlePane@optionscr.c (body in
 * gr3_c.c, unchanged). Retail calls it from updateOptionScreen (func_800DB1E0, group frontier_optionscr_buttons)
 * and from the unmatched replay_save_prompt (lane w36q); with no caller in this group umerge would inline or
 * delete it, and its private convention (s0/s1/s2/s4 unsaved) needs callers that save them. */
void time_result_display(void);

void __standin_time_result_display(void)
{
    time_result_display();
}

void __standin_time_result_display2(void)
{
    time_result_display();
}
