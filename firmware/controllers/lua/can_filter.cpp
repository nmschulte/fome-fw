#include "pch.h"
#include "can_filter.h"
#include "can_hw.h"

static constexpr size_t maxFilterCount = 48;

static size_t filterCount = 0;
static CCM_OPTIONAL CanFilter filters[maxFilterCount];

CanFilter* getFilterForId(CanBusIndex busIndex, int Id) {
	for (size_t i = 0; i < filterCount; i++) {
		auto& filter = filters[i];

		if (filter.accept(Id)) {
			if (filter.Bus == CanBusIndex::Any || filter.Bus == busIndex) {
				return &filter;
			}
		}
	}

	return nullptr;
}

void resetLuaCanRx() {
	// Clear all lua filters - reloading the script will reinit them
	filterCount = 0;
}

void addLuaCanRxFilter(int32_t eid, uint32_t mask, CanBusIndex bus, int callback) {
	if (filterCount >= maxFilterCount) {
		firmwareError(ObdCode::OBD_PCM_Processor_Fault, "Too many Lua CAN RX filters");
	}

	efiPrintf("Added Lua CAN RX filter id 0x%x mask 0x%x with%s custom function", (unsigned int)eid, (unsigned int)mask, (callback == -1 ? "out" : ""));

	filters[filterCount].Id = eid;
	filters[filterCount].Mask = mask;
	filters[filterCount].Bus = bus;
	filters[filterCount].Callback = callback;

	filterCount++;
}
