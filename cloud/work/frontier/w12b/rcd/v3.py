OLD="""                gc->finish_a = 1;
                gc->finish_b = 1;
                gc->flags311 |= 1;
                gc->finish_timer = 0.0f;
                m->finish_a = 1;
                m->finish_b = 1;
                m->flags &= ~8;"""
V={}
V['m_first']="""                m->finish_a = 1;
                m->finish_b = 1;
                m->flags &= ~8;
                gc->finish_a = 1;
                gc->finish_b = 1;
                gc->flags311 |= 1;
                gc->finish_timer = 0.0f;"""
