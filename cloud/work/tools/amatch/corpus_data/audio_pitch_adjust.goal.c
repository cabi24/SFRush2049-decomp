void audio_pitch_adjust(Node *n)
{
    n->state = 3;
    func_8009211C(n->next, n);
    func_80091FBC(&D_80144C50, n, D_80144C50.next);
    n->next = &D_80144C50;
}