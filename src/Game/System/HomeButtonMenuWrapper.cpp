#include "Game/System/HomeButtonMenuWrapper.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <revolution/rso.h>

void (*HBMCreateRSO)(const HBMDataInfo*);
void (*HBMInitRSO)(void);
void (*HBMCalcRSO)(const HBMControllerData*);
void (*HBMDrawRSO)(void);
HBMSelectBtnNum (*HBMGetSelectBtnNumRSO)(void);
void (*HBMSetAdjustFlagRSO)(int);
void (*HBMStartBlackOutRSO)(void);

static RSOExportFuncTable exp_tbl[] = {{"HBMCreateRSO", (u32*)&HBMCreateRSO},
                                       {"HBMInitRSO", (u32*)&HBMInitRSO},
                                       {"HBMCalcRSO", (u32*)&HBMCalcRSO},
                                       {"HBMDrawRSO", (u32*)&HBMDrawRSO},
                                       {"HBMGetSelectBtnNumRSO", (u32*)&HBMGetSelectBtnNumRSO},
                                       {"HBMSetAdjustFlagRSO", (u32*)&HBMSetAdjustFlagRSO},
                                       {"HBMStartBlackOutRSO", (u32*)&HBMStartBlackOutRSO}};

typedef void (*ProloguePtr)(BOOL);

// Archipelago: void RSO::setupRsoHomeButtonMenu() {
// Archipelago:     u32 i;
// Archipelago:     RSOObjectHeader* rsoPtr;
// Archipelago:     RSOExportFuncTable* pTbl;
// Archipelago:     const RSOObjectHeader* symbolTable = reinterpret_cast< const RSOObjectHeader* >(MR::receiveFile("/ModuleData/product.sel"));
// Archipelago:     int jumpCodeSize;
// Archipelago:     void* bss;
// Archipelago:     void* jumps;

// Archipelago:     RSOListInit((void*)symbolTable);
// Archipelago:     if (symbolTable != nullptr) {
// Archipelago:         jumpCodeSize = RSOGetJumpCodeSize(symbolTable);

// Archipelago:         if (jumpCodeSize == 0) {
// Archipelago:             jumps = nullptr;
// Archipelago:         } else {
// Archipelago:             jumps = new (MR::getStationedHeapGDDR3(), 0) char[jumpCodeSize];
// Archipelago:             RSOMakeJumpCode(symbolTable, jumps);
// Archipelago:         }
// Archipelago:         rsoPtr = reinterpret_cast< RSOObjectHeader* >(MR::receiveFile("/ModuleData/HomeButtonMenuWrapperRSO.rso"));
// Archipelago:         if (rsoPtr->mBssSize) {
// Archipelago:             bss = new (MR::getStationedHeapGDDR3(), 0) char[rsoPtr->mBssSize];
// Archipelago:         }

// Archipelago:         RSOLinkList((void*)rsoPtr, bss);

// Archipelago:         if (rsoPtr != nullptr) {
// Archipelago:             RSOLinkJump(rsoPtr, symbolTable, jumps);
// Archipelago:             reinterpret_cast< ProloguePtr >(rsoPtr->mProlog)(RSOIsImportSymbolResolvedAll(rsoPtr));
// Archipelago:             for (i = 0; i < ARRAY_SIZE(exp_tbl); i++) {
// Archipelago:                 pTbl = &exp_tbl[i];
// Archipelago:                 RSOFindExportSymbolAddr(rsoPtr, pTbl->symbol_name);
// Archipelago:                 *(pTbl->symbol_ptr) = (u32)RSOFindExportSymbolAddr(rsoPtr, pTbl->symbol_name);
// Archipelago:             }
// Archipelago:         }
// Archipelago:     }
// Archipelago: }

// Archipelago: void RSO::HBMCreate(const HBMDataInfo* pHBInfo) {
// Archipelago:     (*HBMCreateRSO)(pHBInfo);
// Archipelago: }

// Archipelago: void RSO::HBMInit() {
// Archipelago:     (*HBMInitRSO)();
// Archipelago: }

// Archipelago: void RSO::HBMCalc(const HBMControllerData* pController) {
// Archipelago:     (*HBMCalcRSO)(pController);
// Archipelago: }

// Archipelago: void RSO::HBMDraw() {
// Archipelago:     (*HBMDrawRSO)();
// Archipelago: }

// Archipelago: HBMSelectBtnNum RSO::HBMGetSelectBtnNum() {
// Archipelago:     return (*HBMGetSelectBtnNumRSO)();
// Archipelago: }

// Archipelago: void RSO::HBMSetAdjustFlag(int flag) {
// Archipelago:     (*HBMSetAdjustFlagRSO)(flag);
// Archipelago: }

// Archipelago: void RSO::HBMStartBlackOut() {
// Archipelago:     (*HBMStartBlackOutRSO)();
// Archipelago: }

void _unresolved(void) {
}
