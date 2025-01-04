#include "graph_analysis.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"

using namespace llvm;

// Plugin Registration
extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION,
        "MultiPassPlugin",
        "v1.0",
        [](PassBuilder &PB) {
            PB.registerAnalysisRegistrationCallback(
                [](FunctionAnalysisManager &FAM) {
                    FAM.registerPass([&] { return LoopAnalysis(); });
                    FAM.registerPass([&] { return DominatorTreeAnalysis(); });
                    FAM.registerPass([&] { return PostDominatorTreeAnalysis(); });
                });
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "graph-analysis") {
                        FPM.addPass(GraphAnalysis());
                        return true;
                    }
                    return false;
                });
        }};
}
