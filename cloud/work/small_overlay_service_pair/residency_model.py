"""Abstract loader model. Not emulation and not a reachability proof."""
from dataclasses import dataclass
@dataclass(frozen=True)
class State:
    display: bool = False
    large: bool = False
    small: bool = False
    owned: str = 'none'

def init(s):
    return s if s.small else State(s.display,s.large,True,'small')

def enable(s):
    if s.display: return s
    # Native clears the small flag before invoking guarded large load.
    owned = 'none' if s.small else s.owned
    return State(True,True,False,owned if s.large else 'large')

def disable(s):
    if not s.display: return s
    return State(False,False,s.small,'none' if s.large else s.owned)

def transition_reload(s,mode,extra):
    s=disable(s)
    return init(s) if mode in (4,5,6) or extra else s

def tests():
    cold=State()
    small=init(cold)
    assert small == State(False,False,True,'small')
    large=enable(small)
    assert large == State(True,True,False,'large')
    off=disable(large)
    assert off.owned == 'none' and not off.small
    for mode in range(8):
        for extra in (False,True):
            out=transition_reload(large,mode,extra)
            assert (out.owned=='small') == (mode in (4,5,6) or extra)
    # InitMaxPath has no large-image exclusion check: arbitrary call ordering
    # cannot establish mutual exclusion. This is NOT a runtime-reachable trace.
    malformed=init(large)
    assert malformed.large and malformed.small and malformed.owned=='small'
    # Display mode and ownership may disagree under such an unproven ordering.
    assert enable(malformed) == malformed
    print('PASS: bounded loader-model assertions; assumptions remain explicit')
if __name__ == '__main__':tests()
