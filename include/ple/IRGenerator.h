#pragma once
#include "CalcBaseVisitor.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <memory>
#include <map>
#include <string>

class IRGenerator : public CalcBaseVisitor {
    std::unique_ptr<llvm::LLVMContext> TheContext;
    std::unique_ptr<llvm::Module> TheModule;
    std::unique_ptr<llvm::IRBuilder<>> Builder;

    llvm::Value *LastValue = nullptr;
public:
    IRGenerator(const std::string &ModuleName);

    virtual std::any visitProg(CalcParser::ProgContext *ctx) override;
    virtual std::any visitExpr(CalcParser::ExprContext *ctx) override;
    virtual std::any visitTerm(CalcParser::TermContext *ctx) override;
    virtual std::any visitFactor(CalcParser::FactorContext *ctx) override;

    llvm::Module *getModule(void) { return TheModule.get(); }
    void printIR(void);
};