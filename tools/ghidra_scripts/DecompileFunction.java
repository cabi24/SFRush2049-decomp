//Decompile a single function at a specific address
//@category Decompilation
//@author Rush2049 Decomp

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;

public class DecompileFunction extends GhidraScript {

    @Override
    public void run() throws Exception {
        // Get address from script arguments or prompt
        String addrStr = null;

        String[] args = getScriptArgs();
        if (args.length > 0) {
            addrStr = args[0];
        } else {
            addrStr = askString("Address", "Enter function address (e.g., 0x800CA3B4):");
        }

        if (addrStr == null || addrStr.isEmpty()) {
            println("ERROR: No address provided");
            return;
        }

        // Parse address
        Address addr;
        try {
            if (addrStr.startsWith("0x")) {
                addrStr = addrStr.substring(2);
            }
            addr = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(Long.parseLong(addrStr, 16));
        } catch (Exception e) {
            println("ERROR: Invalid address format: " + addrStr);
            return;
        }

        // Find function at address
        FunctionManager funcMgr = currentProgram.getFunctionManager();
        Function func = funcMgr.getFunctionAt(addr);

        if (func == null) {
            // Try to find function containing address
            func = funcMgr.getFunctionContaining(addr);
        }

        if (func == null) {
            println("ERROR: No function found at address " + addr);
            return;
        }

        // Set up decompiler
        DecompInterface decomp = new DecompInterface();
        DecompileOptions options = new DecompileOptions();
        decomp.setOptions(options);

        if (!decomp.openProgram(currentProgram)) {
            println("ERROR: Failed to open program in decompiler");
            return;
        }

        // Decompile
        DecompileResults results = decomp.decompileFunction(func, 60, monitor);

        if (results == null || !results.decompileCompleted()) {
            println("ERROR: Decompilation failed for " + func.getName());
            decomp.dispose();
            return;
        }

        DecompiledFunction decompFunc = results.getDecompiledFunction();
        if (decompFunc == null) {
            println("ERROR: No decompiled output");
            decomp.dispose();
            return;
        }

        // Output
        println("/* Function: " + func.getName() + " */");
        println("/* Address: " + func.getEntryPoint() + " */");
        println("/* Size: " + func.getBody().getNumAddresses() + " bytes */");
        println("");
        println(decompFunc.getC());

        decomp.dispose();
    }
}
