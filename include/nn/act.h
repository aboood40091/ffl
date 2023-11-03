#pragma once

#include <nn/result.h>

struct FFLStoreData;

namespace nn { namespace act {

Result
Initialize(void)
   asm("Initialize__Q2_2nn3actFv");

Result
Finalize(void)
   asm("Finalize__Q2_2nn3actFv");

Result
GetMiiEx(FFLStoreData* mii, uint8_t slot)
   asm("GetMiiEx__Q2_2nn3actFP12FFLStoreDataUc");

Result
GetTransferableIdEx(uint64_t* outId, uint32_t unk1, uint8_t slot)
   asm("GetTransferableIdEx__Q2_2nn3actFPULUiUc");

bool
IsSlotOccupied(uint8_t slot)
   asm("IsSlotOccupied__Q2_2nn3actFUc");

} }
