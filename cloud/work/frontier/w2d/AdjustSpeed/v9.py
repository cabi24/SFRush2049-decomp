EXTRA="--internal func_80095CF4 --internal func_80095CFC --internal func_800A2674 --internal func_800A2670 --internal func_800A2678 --internal func_800A266C"
B="        if (pfs) {}\n"
V={
 'f1': [(B, B+"        if (D_8011194C) {}\n")],
 'f2': [(B, B+"        if (D_8011194C) {}\n        if (D_8011194C) {}\n")],
 'f3': [("    func_8008A704();\n    request", "    if (D_8011194C) {}\n    func_8008A704();\n    request")],
 'f4': [(B, B+"        if (&D_8011194C) {}\n")],
}
