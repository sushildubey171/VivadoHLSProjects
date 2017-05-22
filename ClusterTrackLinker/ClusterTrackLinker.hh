#ifndef ClusterTrackLinker_hh
#define ClusterTrackLinker_hh

#include "ClusterFinder.hh"

// Track PT is specified in 16-bits
const uint16_t MaxTrackPT = 0xFFFF;
// Barrel calorimeter eta limit is 1.479 and is measured with LSB = 0.0005 - making it up :)
// Temporarily deal with only positive eta - in principle MaxTrackEta should be double this!
const uint16_t MaxTrackEta = int(1.479 / 0.0005);
// Barrel calorimeter eta phi coverage is measured with LSB = 0.0001 - making it up :)
const uint16_t MaxTrackPhi = int(2. * 3.1415927 / 0.0001);

const uint16_t MaxTracksInCard = 20;
const uint16_t MaxNeutralClusters = NCaloLayer1Eta * NCaloLayer1Phi;

bool getClusterTrackLinker(uint16_t smallClusterET[NCaloLayer1Eta][NCaloLayer1Phi],
			   uint16_t peakEta[NCaloLayer1Eta][NCaloLayer1Phi], 
			   uint16_t peakPhi[NCaloLayer1Eta][NCaloLayer1Phi], 
			   uint16_t trackPT[MaxTracksInCard],
			   uint16_t trackEta[MaxTracksInCard],
			   uint16_t trackPhi[MaxTracksInCard], 
			   uint16_t linkedTrackPT[MaxTracksInCard],
			   uint16_t linkedTrackEta[MaxTracksInCard],
			   uint16_t linkedTrackPhi[MaxTracksInCard],
			   uint16_t linkedTrackQuality[MaxTracksInCard],
			   uint16_t neutralClusterET[MaxNeutralClusters],
			   uint16_t neutralClusterEta[MaxNeutralClusters],
			   uint16_t neutralClusterPhi[MaxNeutralClusters]);

#endif
