#ifndef MODULE_DATA_PRODUCER_H
#define MODULE_DATA_PRODUCER_H

#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"

#include <algorithm>

#include "../interface/ModuleData.h"

class ModuleDataProducer
{
	private:
		ModuleDataProducer() = delete;
		// Phase-0: Layer + ladder pairs
		static constexpr std::array<std::pair<int, int>, 12> phaseZeroHalfModuleList =
		{{
			std::pair<int, int>(1, 5),
			std::pair<int, int>(1, 6),
			std::pair<int, int>(1, 15),
			std::pair<int, int>(1, 16),
			std::pair<int, int>(2, 8),
			std::pair<int, int>(2, 9),
			std::pair<int, int>(2, 24),
			std::pair<int, int>(2, 25),
			std::pair<int, int>(3, 11),
			std::pair<int, int>(3, 12),
			std::pair<int, int>(3, 33),
			std::pair<int, int>(3, 34)
		}};

	public:
		~ModuleDataProducer();

		/////////////
		// Phase-0 //
		/////////////

		static ModuleData getPhaseZeroOfflineModuleData(const uint32_t& rawId, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& federrors);
		static ModuleData getPhaseZeroOnlineModuleData (const uint32_t& rawId, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& federrors);
		static ModuleData convertPhaseZeroOfflineOnline(const ModuleData& modOff);
	private:
		static int isPhaseZeroModuleHalf(const int& layer, const int& ladder);
		static int getPhaseZeroShellNum(const int& det, const int& ladder, const int& module, const int& disk, const int& blade);
		static int convertPhaseZeroOfflineModuleToOnlineModuleCoordinate(const int& module);
		static int convertPhaseZeroOfflineLadderToOnlineLadderCoordinate(const int& layer, const int& ladder);
		static int convertPhaseZeroOfflineDiskToOnlineDiskCoordinate(const int& side, const int& disk);
		static int convertPhaseZeroOfflineBladeToOnlineBladeCoordinate(const int& blade);

		/////////////
		// Phase-I //
		/////////////
		
		static ModuleData getPhaseOneOfflineModuleData(const uint32_t& rawId, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& federrors);
		static ModuleData getPhaseOneOnlineModuleData (const uint32_t& rawId, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& federrors);
		static ModuleData convertPhaseOneOfflineOnline(const ModuleData& modOff);
	private:
		static int convertPhaseOneOfflineModuleToOnlineModuleCoordinate(const int& module);
		static int convertPhaseOneOfflineLadderToOnlineLadderCoordinate(const int& layer, const int& ladder);
		static int convertPhaseOneOfflineDiskToOnlineDiskCoordinate(const int& side, const int& disk);
		static int convertPhaseOneOfflineBladeToOnlineBladeCoordinate(const int& blade);
};

#endif