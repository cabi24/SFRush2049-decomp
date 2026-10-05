EXTRA="--internal func_80095CF4 --internal func_80095CFC --internal func_800A2674 --internal func_800A2670 --internal func_800A2678"
F="    file=&D_80144030[port].files[request->index].state;\n"
C="    controller=&D_80144030[port];\n"
V={
 'a': [(C, "    controller=&D_80144030[request->port];\n")],
 'b': [(F, "    file=&D_80144030[request->port].files[request->index].state;\n")],
 'c': [(F+C, C+F)],
 'd': [(F+C, C+"    file=&controller->files[request->index].state;\n")],
 'e': [(F+C, F+"    controller=&D_80144030[node->req->port];\n")],
 'f': [("    port=request->port;\n"+F+C, F.replace('port','request->port')+"    port=request->port;\n"+C)],
}
