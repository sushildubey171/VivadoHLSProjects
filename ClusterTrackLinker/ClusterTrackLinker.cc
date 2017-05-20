#include "ClusterFinder.hh"
#include "ClusterTrackLinker.hh"

bool getClusterTrackLinker(uint16_t clusterET[NCaloLayer1Eta][NCaloLayer1Phi],
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
			   uint16_t neutralClusterPhi[MaxNeutralClusters]) {
  uint16_t clusterEta[MaxNeutralClusters];
  uint16_t clusterPhi[MaxNeutralClusters];
  for(int tEta = 0; tEta < NCaloLayer1Eta; tEta++) {
    for(int tPhi = 0; tPhi < NCaloLayer1Phi; tPhi++) {
      int cluster = tEta * NCaloLayer1Phi + tPhi;
      // Convert cruder calorimeter position to track LSB
      // This can be a LUT - perhaps HLS will take care of this efficiently
      clusterEta[cluster] = (tEta * NCrystalsPerEtaPhi + peakEta[tEta][tPhi]) * MaxTrackEta / NCrystalsInEta;
      clusterPhi[cluster] = (tPhi * NCrystalsPerEtaPhi + peakPhi[tEta][tPhi]) * MaxTrackPhi / NCrystalsInPhi;
      // Initialize neutral clusters
      neutralClusterET[cluster] = clusterET[tEta][tPhi];
      neutralClusterEta[cluster] = clusterEta[cluster];
      neutralClusterPhi[cluster] = clusterPhi[cluster];
    }
  }
  // Double loop over tracks and clusters for linking
  for(int track = 0; track < MaxTracksInCard; track++) {
    linkedTrackPT[track] = trackPT[track];
    linkedTrackEta[track] = trackEta[track];
    linkedTrackPhi[track] = trackPhi[track];
    linkedTrackQuality[track] = 0;
    uint8_t nMatches = 0;
    for(int cluster = 0; cluster < MaxNeutralClusters; cluster++) {
      uint16_t diffEta = clusterEta[cluster] - trackEta[track];
      if(diffEta >= MaxTrackEta) diffEta = trackEta[track] - clusterEta[cluster];
      uint16_t diffPhi = clusterPhi[cluster] - trackPhi[track];
      if(diffPhi >= MaxTrackPhi) diffPhi = trackPhi[track] - clusterPhi[cluster];
      if(diffEta <= 1 && diffPhi <= 2) {
        nMatches++;
        linkedTrackQuality[track] |= 0x0001;
        if(diffEta <= 1 && diffPhi <= 1) {
          linkedTrackQuality[track] |= 0x0002;
        }
        if(diffEta == 0 && diffPhi == 0) {
          linkedTrackQuality[track] |= 0x0004;
        }
        if(neutralClusterET[cluster] > trackPT[track]) {
          neutralClusterET[cluster] -= trackPT[track];
          linkedTrackQuality[track] |= 0x0010;
          // To do: Adjust eta, phi somehow
        }
        else {
          linkedTrackQuality[track] |= 0x0020;
          neutralClusterET[cluster] = 0;
        }
      }
    }
    linkedTrackQuality[track] |= (nMatches << 8);
  }
  return true;
}
