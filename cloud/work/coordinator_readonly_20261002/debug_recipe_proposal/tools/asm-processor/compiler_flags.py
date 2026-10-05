"""Select the placeholder model without changing the real compiler recipe."""


def processor_flags(compile_args):
    recognized = {"-g3", "-g", "-O0", "-O1", "-O2", "-framepointer", "-KPIC"}
    flags = [arg for arg in compile_args if arg in recognized]
    debug = next((arg for arg in reversed(compile_args)
                  if arg in {"-g", "-g0", "-g1", "-g2", "-g3"}), None)
    opt = next((arg for arg in reversed(compile_args)
                if arg in {"-O0", "-O1", "-O2", "-O3"}), None)
    # IDO's debug O1 placeholders have four return/epilogue words, as in
    # the existing -g model. Passing -O1 to the processor leaves extra
    # returns between the real assembly slots. The compiler still gets
    # the original -g1/-g2 -O1 arguments below in build.py.
    if debug in {"-g1", "-g2"} and opt == "-O1":
        flags = ["-g"] + [arg for arg in flags
                          if arg in {"-framepointer", "-KPIC"}]
    if "-mips2" not in compile_args:
        flags.append("-mips1")
    return flags
