#include "llvm/Transforms/Utils/AssumeBundleBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Local.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Pass.h"
#include "llvm/Analysis/PostDominators.h"
#include "llvm/Support/JSON.h"

#include <unordered_set>

using namespace llvm;

namespace {

static std::string blockName(BasicBlock *BB) {
    std::string block_address;
    raw_string_ostream string_stream(block_address);
    BB->printAsOperand(string_stream, false);

    return string_stream.str();
}

std::string instructionToString(const llvm::Instruction &I) {
    std::string Str;
    llvm::raw_string_ostream RSO(Str);
    I.print(RSO);
    std::string instr = RSO.str();
    return instr.substr(instr.find_first_not_of(" \t"));
}

struct GraphAnalysis : PassInfoMixin<GraphAnalysis> {
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &FAM) {
        auto &DT = FAM.getResult<DominatorTreeAnalysis>(F);
        auto &PDT = FAM.getResult<PostDominatorTreeAnalysis>(F);

        std::unordered_map<BasicBlock *, std::unordered_set<BasicBlock *>> dt;
        std::unordered_map<BasicBlock *, std::unordered_set<BasicBlock *>> df;
        std::unordered_map<BasicBlock *, std::unordered_set<BasicBlock *>> postDF;
        std::unordered_map<BasicBlock *, std::unordered_set<BasicBlock *>> cfg;
        std::unordered_map<BasicBlock *, std::string> block_labels;

        for (BasicBlock &BB : F) {
            cfg.insert({&BB, {}});
            dt.insert({&BB, {}});
        }

        for (BasicBlock &BB : F) {
            errs() << "Basic Block: " << BB << " " << pred_size(&BB) << "\n";

            BB.printAsOperand(errs(), false);
            if (pred_size(&BB) < 1)
                continue;
            for (BasicBlock *Pred : predecessors(&BB)) {
                cfg[Pred].insert(&BB);
            }
            BasicBlock *IDom = DT.getNode(&BB)->getIDom()->getBlock();
            dt[IDom].insert(&BB);
        }

        for (BasicBlock &BB : F) {
            if (pred_size(&BB) < 2)
                continue;
            for (BasicBlock *Pred : predecessors(&BB)) {
                BasicBlock *Runner = Pred;
                while (Runner && Runner != DT.getNode(&BB)->getIDom()->getBlock()) {
                    df[Runner].insert(&BB);
                    Runner = DT.getNode(Runner)->getIDom()->getBlock();
                }
            }
        }

        for (BasicBlock &BB : F) {
            if (succ_size(&BB) < 2)
                continue;
            for (BasicBlock *Succ : successors(&BB)) {
                BasicBlock *Runner = Succ;
                while (Runner && Runner != PDT.getNode(&BB)->getIDom()->getBlock()) {
                    postDF[Runner].insert(&BB);
                    Runner = PDT.getNode(Runner)->getIDom()->getBlock();
                }
            }
        }

        for (BasicBlock &BB : F) {
            std::string block_label;
            for (Instruction &I : BB) {
                block_label += instructionToString(I);
                block_label += "\\l";
            }
            block_labels.insert({&BB, block_label});
        }

        json::Object cfgResult;
        json::Array DominatorTree;
        json::Array Objects;
        for (BasicBlock &BB : F) {
            json::Array PredsCFG;
            json::Array PredsDT;
            json::Array PredsDF;
            json::Array PredsPostDF;
            std::string blockName = ::blockName(&BB);
            for (auto *Pred : cfg[&BB]) {
                PredsCFG.push_back(::blockName(Pred));
            }
            for (auto *Pred : dt[&BB]) {
                PredsDT.push_back(::blockName(Pred));
            }
            for (auto *Pred : df[&BB]) {
                PredsDF.push_back(::blockName(Pred));
            }
            for (auto *Pred : postDF[&BB]) {
                PredsPostDF.push_back(::blockName(Pred));
            }
            json::Object cfgNode = json::Object{{"name", blockName}, {"edge_cfg", std::move(PredsCFG)}, {"edge_dt", std::move(PredsDT)}, {"dominator_frontier", std::move(PredsDF)}, {"post_dominator_frontier", std::move(PredsPostDF)}, {"label", block_labels[&BB]}};
            Objects.push_back(std::move(cfgNode));
        }
        cfgResult["cfg"] = std::move(Objects);

        std::error_code EC;
        raw_fd_ostream File("output/cfg.json", EC);
        if (EC) {
            errs() << "Could not open file: " << EC.message() << "\n";
            return PreservedAnalyses::all();
        }
        errs() << "Writing to cfg.json\n";
        
        File << json::Value(std::move(cfgResult)) << "\n";
        File.close();

        return PreservedAnalyses::all();
    }
};
} // namespace