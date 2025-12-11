//Decompile all functions in the program and output C code
//@category Decompilation
//@author Rush2049 Decomp

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.address.*;
import ghidra.util.task.TaskMonitor;

public class DecompileAllFunctions extends GhidraScript {

    @Override
    public void run() throws Exception {
        DecompInterface decomp = new DecompInterface();

        // Configure decompiler options
        DecompileOptions options = new DecompileOptions();
        decomp.setOptions(options);

        if (!decomp.openProgram(currentProgram)) {
            println("ERROR: Failed to open program in decompiler");
            return;
        }

        FunctionManager funcMgr = currentProgram.getFunctionManager();
        int totalFuncs = funcMgr.getFunctionCount();
        int processed = 0;
        int failed = 0;

        println("/* ========================================== */");
        println("/* Ghidra Decompilation Output                */");
        println("/* Program: " + currentProgram.getName() + " */");
        println("/* Functions: " + totalFuncs + "                  */");
        println("/* ========================================== */");
        println("");

        // Iterate all functions
        FunctionIterator funcs = funcMgr.getFunctions(true);
        while (funcs.hasNext() && !monitor.isCancelled()) {
            Function func = funcs.next();
            processed++;

            // Skip thunks and external functions
            if (func.isThunk() || func.isExternal()) {
                continue;
            }

            Address addr = func.getEntryPoint();
            String name = func.getName();

            // Decompile with 60 second timeout
            DecompileResults results = decomp.decompileFunction(func, 60, monitor);

            if (results == null || !results.decompileCompleted()) {
                println("/* ERROR: Failed to decompile " + name + " @ " + addr + " */");
                failed++;
                continue;
            }

            DecompiledFunction decompFunc = results.getDecompiledFunction();
            if (decompFunc == null) {
                println("/* ERROR: No decompiled output for " + name + " @ " + addr + " */");
                failed++;
                continue;
            }

            // Output the C code
            println("/* ------------------------------------------ */");
            println("/* " + name + " @ " + addr + " */");
            println("/* ------------------------------------------ */");
            println(decompFunc.getC());
            println("");

            // Progress update every 50 functions
            if (processed % 50 == 0) {
                printerr("Progress: " + processed + "/" + totalFuncs + " functions");
            }
        }

        decomp.dispose();

        println("/* ========================================== */");
        println("/* Decompilation complete                     */");
        println("/* Processed: " + processed + " functions         */");
        println("/* Failed: " + failed + " functions              */");
        println("/* ========================================== */");
    }
}
