#include "ClusterFinder.hh"
#include "ClusterTrackLinker.hh"

#include <iostream>
using namespace std;

int main(int argc, char **argv) {

  uint16_t crystals[NCaloLayer1Eta][NCaloLayer1Phi][NCrystalsPerEtaPhi][NCrystalsPerEtaPhi];
  for(int tEta = 0; tEta < NCaloLayer1Eta; tEta++) {
    for(int tPhi = 0; tPhi < NCaloLayer1Phi; tPhi++) {
      for(int cEta = 0; cEta < NCrystalsPerEtaPhi; cEta++) {
        for(int cPhi = 0; cPhi < NCrystalsPerEtaPhi; cPhi++) {
          crystals[tEta][tPhi][cEta][cPhi] = 0;
        }
      }
    }
  }

  double objects[10] = {11, 22, 33, 44, 55, 66, 77, 88, 99, 111};
  double totalET = 0;
  uint16_t trackPT[5] = {0};  // With LSB = 0.1
  uint16_t trackEta[5] = {0}; // With LSB = 0.0005 (only positive and < 1.479)
  uint16_t trackPhi[5] = {0}; // With LSB = 0.0001 (only positive and < 2Pi)
  cout << "Generated objects: " << endl;
  cout << "tEta\ttPhi\tcEta\tcPhi\tobjectET" << endl;
  for(int object = 0; object < 10; object++) {
    // Crude simulation of dispersal of object ET for fun around some location
    int objectET = objects[object];
    int tEta = objectET % NCaloLayer1Eta;
    int tPhi = objectET % NCaloLayer1Phi;
    int cEta = (tPhi * objectET) % NCrystalsPerEtaPhi;
    int cPhi = (tEta * objectET) % NCrystalsPerEtaPhi;
    // Print information
    cout << tEta
         << "\t" << tPhi
         << "\t" << cEta
         << "\t" << cPhi
         << "\t" << objects[object] << endl;
    for(int dEta = -1; dEta <= 1; dEta++) {
      for(int dPhi = -1; dPhi <= 1; dPhi++) {
        int ncEta = cEta + dEta;
        int ncPhi = cPhi + dPhi;
        // Start within the tower boundary
        int ntEta = tEta;
        int ntPhi = tPhi;
        // Adjust neighbor trigger tower as needed
        if(ncEta < 0) {ncEta = NCaloLayer1Phi; ntEta = tEta - 1;}
        else if(ncEta > NCaloLayer1Phi) {ncEta = 0; ntEta = tEta + 1;}
        if(ncPhi < 0) {ncPhi = NCaloLayer1Phi; ntPhi = tPhi - 1;}
        else if(ncPhi > NCaloLayer1Phi) {ncPhi = 0; ntPhi = tPhi + 1;}
        // Ignore spill-overs outside the card, defering to next layer
        if(dEta == 0 && dPhi == 0) {
          crystals[tEta][tPhi][cEta][cPhi] = (objects[object] * 0.9);
        }
        else {
          if(ntEta >= 0 && ntEta < NCaloLayer1Eta && ntPhi >= 0 && ntPhi < NCaloLayer1Phi)
            crystals[ntEta][ntPhi][ncEta][ncPhi] = (objects[object] * 0.1 / 8.);
        }
      }
    }
    totalET += objects[object];
    // Set matching track parameters for half the clusters
    if((object % 2) == 0) {
      double pt = objects[object];
      double phi = (tPhi * NCrystalsPerEtaPhi + cPhi) * (0.087 / NCrystalsPerEtaPhi);
      // PT is measured with LSB = 1 GeV
      trackPT[object / 2] = int(pt);
      trackEta[object / 2] = (tEta * NCrystalsPerEtaPhi + cEta) * MaxTrackEta / NCrystalsInEta;
      trackPhi[object / 2] = (tPhi * NCrystalsPerEtaPhi + cPhi) * MaxTrackPhi / NCrystalsInPhi;
    }
  }
  cout << "Total generated ET = " << totalET << endl;
  uint16_t totalDeposited = 0;
  for(int tEta = 0; tEta < NCaloLayer1Eta; tEta++) {
    for(int tPhi = 0; tPhi < NCaloLayer1Phi; tPhi++) {
      for(int cEta = 0; cEta < NCrystalsPerEtaPhi; cEta++) {
        for(int cPhi = 0; cPhi < NCrystalsPerEtaPhi; cPhi++) {
          totalDeposited += crystals[tEta][tPhi][cEta][cPhi];
        }
      }
    }
  }
  cout << "Total deposited ET = " << totalDeposited << endl;
  uint16_t peakEta[NCaloLayer1Eta][NCaloLayer1Phi];
  uint16_t peakPhi[NCaloLayer1Eta][NCaloLayer1Phi];
  uint16_t largeClusterET[NCaloLayer1Eta][NCaloLayer1Phi];
  uint16_t smallClusterET[NCaloLayer1Eta][NCaloLayer1Phi];
  uint16_t totalCardET = 0;
  if(getClustersInCard(crystals, peakEta, peakPhi, largeClusterET, smallClusterET)) {
    cout << "From the cluster simulation: " << endl;
    cout << "tEta\ttPhi\tpeakEta\tpeakPhi\tlargeClusterET\tsmallClusterET" << endl;
    for(int tEta = 0; tEta < NCaloLayer1Eta; tEta++) {
      for(int tPhi = 0; tPhi < NCaloLayer1Phi; tPhi++) {
        if(largeClusterET[tEta][tPhi] > 0) 
          cout << tEta
               << "\t" << tPhi
               << "\t" << peakEta[tEta][tPhi]
               << "\t" << peakPhi[tEta][tPhi]
               << "\t" << largeClusterET[tEta][tPhi]
               << "\t" << smallClusterET[tEta][tPhi] << endl;
        totalCardET += largeClusterET[tEta][tPhi];
      }
    }
    cout << "Total card ET = " << totalCardET << endl;
  }
  else {
    cout << "ClusterFinder failed" << endl;
    return 1;
  }

  uint16_t linkedTrackPT[MaxTracksInCard] = {0};
  uint16_t linkedTrackEta[MaxTracksInCard] = {0};
  uint16_t linkedTrackPhi[MaxTracksInCard] = {0};
  uint16_t linkedTrackQuality[MaxTracksInCard] = {0};
  uint16_t neutralClusterET[MaxNeutralClusters] = {0};
  uint16_t neutralClusterEta[MaxNeutralClusters] = {0};
  uint16_t neutralClusterPhi[MaxNeutralClusters] = {0};

  if(getClusterTrackLinker(smallClusterET, peakEta, peakPhi, 
			   trackPT, trackEta, trackPhi, 
			   linkedTrackPT, linkedTrackEta, linkedTrackPhi, linkedTrackQuality,
			   neutralClusterET, neutralClusterEta, neutralClusterPhi)) {

    cout << "From the cluster-track linking simulation: " << endl;

    cout << "LinkedTracks: " << endl;
    cout << "trackEta\ttrackPhi\ttrackPT\ttrackLinkQuality" << endl;
    for(int track = 0; track < MaxTracksInCard; track++) {
      if(linkedTrackPT[track] > 0) {
	cout << linkedTrackEta[track]
	     << "\t" << linkedTrackPhi[track]
	     << "\t" << linkedTrackPT[track]
	     << "\t" << linkedTrackQuality[track]
	     << endl;
      }
    }

    cout << "Neutral Clusters: " << endl;
    cout << "clusterEta\tclusterPhi\tclusterET" << endl;
    for(int cluster = 0; cluster < MaxNeutralClusters; cluster++) {
      if(neutralClusterET[cluster] > 0) {
	cout << neutralClusterEta[cluster]
	     << "\t" << neutralClusterPhi[cluster]
	     << "\t" << neutralClusterET[cluster]
	     << endl;
      }
    }

  }
     
  return 0;
}
