#include "../interface/ModuleDataProducer.h"

// Static member declarations
constexpr std::array<std::pair<int, int>, 12> ModuleDataProducer::phaseZeroHalfModuleList;

/////////////
// Phase-0 //
/////////////

ModuleData ModuleDataProducer::getPhaseZeroOfflineModuleData(const uint32_t& rawId, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& federrors)
{
	ModuleData mod;
	mod.rawid = rawId;
	int subdetId = DetId(mod.rawid).subdetId();
	if(subdetId == PixelSubdetector::PixelBarrel)
	{
		mod.det    = 0;
		mod.layer  = trackerTopology -> pxbLayer(rawId);
		mod.ladder = trackerTopology -> pxbLadder(rawId);
		mod.module = trackerTopology -> pxbModule(rawId);
		mod.half   = isPhaseZeroModuleHalf(mod.layer, mod.ladder);
		mod.outer  = (mod.ladder + mod.layer + 1) % 2;
	}
	if(subdetId == PixelSubdetector::PixelEndcap)
	{
		mod.det    = 1;
		mod.side   = trackerTopology -> pxfSide(rawId);
		mod.disk   = trackerTopology -> pxfDisk(rawId);   // Increasing abs(z)
		mod.blade  = trackerTopology -> pxfBlade(rawId);  // Increasing phi
		mod.panel  = trackerTopology -> pxfPanel(rawId);  // Forward: 1; Backward 2
		mod.module = trackerTopology -> pxfModule(rawId); // Increasing R
	}
	mod.shl = getPhaseZeroShellNum(mod.det, mod.ladder, mod.module, mod.disk, mod.blade);
	// FED error
	mod.federr = 0;
	auto foundError = federrors.find(mod.rawid);
	if(foundError != federrors.end())
	{
		mod.federr = foundError -> second;
	}
	return mod;
}

ModuleData ModuleDataProducer::getPhaseZeroOnlineModuleData(const uint32_t& rawId, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& federrors)
{
	ModuleData modOff;
	modOff = getPhaseZeroOfflineModuleData(rawId, trackerTopology, federrors);
	return convertPhaseZeroOfflineOnline(modOff);
}

ModuleData ModuleDataProducer::convertPhaseZeroOfflineOnline(const ModuleData& modOff)
{
	ModuleData modOn;
	modOn.rawid  = modOff.rawid;
	modOn.det    = modOff.det;
	if(modOff.det == 0)
	{
		modOn.layer  = modOff.layer;
		modOn.module = convertPhaseZeroOfflineModuleToOnlineModuleCoordinate(modOff.module);
		modOn.ladder = convertPhaseZeroOfflineLadderToOnlineLadderCoordinate(modOff.layer, modOff.ladder);
		modOn.half   = modOff.half; 
		modOn.outer  = modOff.outer; 
	}
	if(modOff.det == 1)
	{
		modOn.side   = modOff.side;
		modOn.disk   = convertPhaseZeroOfflineDiskToOnlineDiskCoordinate(modOff.side, modOff.disk);
		modOn.blade  = convertPhaseZeroOfflineBladeToOnlineBladeCoordinate(modOff.blade);
		modOn.panel  = modOff.panel;
		modOn.module = modOff.module;
	}
	modOn.shl    = modOff.shl;
	modOn.federr = modOff.federr;
	return modOn;
}

/////////////////////
// Phase-0 Utility //
/////////////////////

int ModuleDataProducer::isPhaseZeroModuleHalf(const int& layer, const int& ladder)
{
	const auto& halves = ModuleDataProducer::phaseZeroHalfModuleList;
	auto found = std::find_if(halves.begin(), halves.end(), [&] (std::pair<int, int> layer_ladder_pair)
	{
		return layer == layer_ladder_pair.first && ladder == layer_ladder_pair.second;
	} );
	if(found == halves.end())
	{
		return 0;
	}
	return 1;
}

int ModuleDataProducer::getPhaseZeroShellNum(const int& det, const int& ladder, const int& module, const int& disk, const int& blade)
{
	if(det == 0) return ((module > 0) ? 0 : 2) + ((ladder > 0) ? 0 : 1);
	if(det == 1) return ((disk   > 0) ? 0 : 2) + ((blade  > 0) ? 0 : 1);
	return -1;
}

int ModuleDataProducer::convertPhaseZeroOfflineModuleToOnlineModuleCoordinate(const int& module)
{
	if(0 < module && module <= 4) return module - 5;
	if(4 < module && module <= 8) return module - 4;
	return NOVAL_I;
}

int ModuleDataProducer::convertPhaseZeroOfflineLadderToOnlineLadderCoordinate(const int& layer, const int& ladder)
{
	int converted = NOVAL_I;
	switch(layer)
	{
		case 1:
			if(0 <  ladder && ladder <= 5)  converted = 6  - ladder;
			if(5 <  ladder && ladder <= 15) converted = 5  - ladder;
			if(15 < ladder && ladder <= 20) converted = 26 - ladder;
			break;
		case 2:
			if(0  < ladder && ladder <= 8 ) converted = 9 -  ladder;
			if(8  < ladder && ladder <= 24) converted = 8 -  ladder;
			if(24 < ladder && ladder <= 32) converted = 41 - ladder;
			break;
		case 3:
			if(0  < ladder && ladder <= 11) converted = 12 - ladder;
			if(11 < ladder && ladder <= 33) converted = 11 - ladder;
			if(34 < ladder && ladder <= 44) converted = 56 - ladder;
			break;
	}
	return converted;
}

int ModuleDataProducer::convertPhaseZeroOfflineDiskToOnlineDiskCoordinate(const int& side, const int& disk)
{
	return disk * (side * 2 - 3);
}

int ModuleDataProducer::convertPhaseZeroOfflineBladeToOnlineBladeCoordinate(const int& blade)
{
	int converted = NOVAL_I;
	if(0 < blade && blade <= 6 ) converted = 7  - blade;
	if(6 < blade && blade <= 18) converted = 6  - blade;
	if(8 < blade && blade <= 24) converted = 31 - blade;
	return converted;
}

/////////////
// Phase-I //
/////////////

ModuleData ModuleDataProducer::getPhaseOneOfflineModuleData(const uint32_t& rawId, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& federrors)
{
	ModuleData mod;
	mod.rawid = rawId;
	int subdetId = DetId(mod.rawid).subdetId();
	if(subdetId == PixelSubdetector::PixelBarrel)
	{
		mod.det    = 0;
		mod.layer  = trackerTopology -> pxbLayer(rawId);
		mod.ladder = trackerTopology -> pxbLadder(rawId);
		mod.module = trackerTopology -> pxbModule(rawId);
	}
	if(subdetId == PixelSubdetector::PixelEndcap)
	{
		mod.det    = 1;
		mod.side   = trackerTopology -> pxfSide(rawId);
		mod.disk   = trackerTopology -> pxfDisk(rawId);   // Increasing abs(z): 1-3
		mod.blade  = trackerTopology -> pxfBlade(rawId);  // Inner blades: 1-22, outer blades: 23-56
		mod.panel  = trackerTopology -> pxfPanel(rawId);  // Forward: 1; Backward 2
		mod.module = trackerTopology -> pxfModule(rawId); // Always 1
		mod.ring   = 1 + (22 < mod.blade);                 // Inner: 1, Outer: 2
	}
	// FED error
	mod.federr = 0;
	auto foundError = federrors.find(mod.rawid);
	if(foundError != federrors.end())
	{
		mod.federr = foundError -> second;
	}
	return mod;
}

ModuleData ModuleDataProducer::getPhaseOneOnlineModuleData(const uint32_t& rawId, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& federrors)
{
	ModuleData modOff;
	modOff = getPhaseOneOfflineModuleData(rawId, trackerTopology, federrors);
	return convertPhaseOneOfflineOnline(modOff);
}

ModuleData ModuleDataProducer::convertPhaseOneOfflineOnline(const ModuleData& modOff)
{
	ModuleData modOn;
	modOn.rawid  = modOff.rawid;
	modOn.det    = modOff.det;
	if(modOff.det == 0)
	{
		modOn.layer  = modOff.layer;
		modOn.module = convertPhaseOneOfflineModuleToOnlineModuleCoordinate(modOff.module);
		modOn.ladder = convertPhaseOneOfflineLadderToOnlineLadderCoordinate(modOff.layer, modOff.ladder);
	}
	if(modOff.det == 1)
	{
		modOn.side   = modOff.side;
		modOn.disk   = convertPhaseOneOfflineDiskToOnlineDiskCoordinate(modOff.side, modOff.disk);    // Increasing abs(z): 1-3
		modOn.blade  = convertPhaseOneOfflineBladeToOnlineBladeCoordinate(modOff.blade); // Inner blades: (-11)-(-1), 1-11, outer blades: (-17)-(1), 1-17
		modOn.panel  = modOff.panel;  // Forward: 1; Backward 2
		modOn.module = modOff.module; // Always 1
		modOn.ring   = modOff.ring;   // Inner: 1, Outer: 2
	}
	modOn.federr = modOff.federr;
	return modOn;
}

/////////////////////
// Phase-I utility //
/////////////////////

int ModuleDataProducer::convertPhaseOneOfflineModuleToOnlineModuleCoordinate(const int& module)
{
	int converted = NOVAL_I;
	if(0 < module && module < 5) converted = module - 5;
	if(4 < module && module < 9) converted = module - 4;
	return converted;
}

int ModuleDataProducer::convertPhaseOneOfflineLadderToOnlineLadderCoordinate(const int& layer, const int& ladder)
{
	static const std::vector<int> layerQuarterSizes = {NOVAL_I, 3, 7, 11, 16};
	const int& quarterSize = layerQuarterSizes[layer];
	int converted = NOVAL_I;
	if(0               < ladder && ladder <= quarterSize)     converted = quarterSize - ladder + 1;
	if(quarterSize     < ladder && ladder <= quarterSize * 3) converted = quarterSize - ladder;
	if(quarterSize * 3 < ladder && ladder <= quarterSize * 4) converted = quarterSize * 5 - ladder;
	return converted;
}

int ModuleDataProducer::convertPhaseOneOfflineDiskToOnlineDiskCoordinate(const int& side, const int& disk)
{
	int converted = NOVAL_I;
	if(side == 2) converted =  disk;
	if(side == 1) converted = -disk;
	return converted;
}

int ModuleDataProducer::convertPhaseOneOfflineBladeToOnlineBladeCoordinate(const int& blade)
{
	int converted = NOVAL_I;
	if(1  <= blade && blade < 6)  converted = 6  - blade; // 5 on 1st quarter
	if(6  <= blade && blade < 17) converted = 5  - blade; // 11 on 2nd half
	if(17 <= blade && blade < 23) converted = 28 - blade; // 6 on 4th quarter
	if(23 <= blade && blade < 31) converted = 31 - blade; // 8 on 1st quarter
	if(31 <= blade && blade < 48) converted = 30 - blade; // 17 on 2nd half
	if(48 <= blade && blade < 57) converted = 65 - blade; // 9 on 4th quarter
	return converted;
}