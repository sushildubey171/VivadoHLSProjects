#include <stdint.h>
#include <stdio.h>

#include "MakeHT.h"

#define NCrts 18
#define NCrds 7
#define NRgns 2
#define NHFRgns 8

void WriteLinkMapHT(uint16_t rgnET[NCrts*NCrds*NRgns], uint16_t hfET[NCrts*NHFRgns], uint16_t HT[1]) {
  // This code is to write suitable mapping of inputs to signals in the CTP7_HLS project from Ales
  // Block 1 of User Code
  for(int iRgn = 0; iRgn < NCrts*NCrds*NRgns; iRgn++) {
    printf("rgnET_%d : IN STD_LOGIC_VECTOR (15 downto 0);\n", iRgn);
  }
  for(int iHFRgn = 0; iHFRgn < NCrts * NHFRgns; iHFRgn++) {
    printf("hfET_%d : IN STD_LOGIC_VECTOR (15 downto 0);\n", iHFRgn);
  }
  printf("HT_d0 : OUT STD_LOGIC_VECTOR (15 downto 0);\n\n\n");
  // Block 2
  for(int iRgn = 0; iRgn < NCrts*NCrds*NRgns; iRgn++) {
    printf("signal rgnET_%d : STD_LOGIC_VECTOR(15 DOWNTO 0);\n", iRgn);
  }
  for(int iHFRgn = 0; iHFRgn < NCrts * NHFRgns; iHFRgn++) {
    printf("signal hfET_%d : STD_LOGIC_VECTOR(15 DOWNTO 0);\n", iHFRgn);
  }
  printf("signal HT_d0 : STD_LOGIC_VECTOR (15 downto 0);\n\n\n");
  // Block 3
  for(int iRgn = 0; iRgn < NCrts*NCrds*NRgns; iRgn++) {
    printf("rgnET_%d => rgnET_%d,\n", iRgn, iRgn);
  }
  for(int iHFRgn = 0; iHFRgn < NCrts * NHFRgns; iHFRgn++) {
    printf("hfET_%d => hfET_%d,\n", iHFRgn, iHFRgn);
  }
  printf("HT_d0 => HT_d0,\n\n\n");
  // Block 4
  for(int iRgn = 0; iRgn < NCrts*NCrds*NRgns; iRgn++) {
    // Each link can carry 192-bits, or 12x16-bits of data
    // Each iRgn needs 16 bits
    int link = (iRgn / 12);
    int loBit = (iRgn % 12) * 16;
    int hiBit = loBit + 15;
    printf("rgnET_%d <= s_INPUT_LINK_ARR( %d )(%d downto %d);\n", iRgn, link, hiBit, loBit);
  }
  for(int iHFRgn = 0; iHFRgn < NCrts * NHFRgns; iHFRgn++) {
    // Each link can carry 192-bits, or 12x16-bits of data
    // Each iHFRgn needs 16 bits
    int link = (iHFRgn / 12) + 21;
    int loBit = (iHFRgn % 12) * 16;
    int hiBit = loBit + 15;
    printf("hfET_%d <= s_INPUT_LINK_ARR( %d )(%d downto %d);\n", iHFRgn, link, hiBit, loBit);
  }
  printf("HT_d0 <= s_OUTPUT_LINK_ARR( 0 )(15 downto 0);\n");
}

int main(int argc, char **argv) {
	uint16_t rgnET[NCrts*NCrds*NRgns];
	uint16_t hfET[NCrts*NHFRgns];

	// Test data; Construct it using indices for the fun of it

	int iCrt;
	int iCrd;
	int iRgn;
	int i;
	int iHFRgn;
	for(iCrt = 0; iCrt < NCrts; iCrt++) {
		for(iCrd = 0; iCrd < NCrds; iCrd++) {
			for(iRgn = 0; iRgn < NRgns; iRgn++) {
				i = iCrt * NCrds * NRgns + iCrd * NRgns + iRgn;
				rgnET[i] = i;
			}
		}
		for(iHFRgn = 0; iHFRgn < NHFRgns; iHFRgn++) {
			i = iCrt * NHFRgns + iHFRgn;
			hfET[i] = i;
		}
	}

	// Determine HT using software

	uint16_t HT = 0;
	for(iCrt = 0; iCrt < NCrts; iCrt++) {
		for(iCrd = 0; iCrd < NCrds; iCrd++) {
			for(iRgn = 0; iRgn < NRgns; iRgn++) {
				i = iCrt * NCrds * NRgns + iCrd * NRgns + iRgn;
				if(rgnET[i] > MinETCutForHT) HT += rgnET[i];
			}
		}
		for(iHFRgn = 0; iHFRgn < NHFRgns; iHFRgn++) {
			i = iCrt * NHFRgns + iHFRgn;
			if(hfET[i] > MinHFETCutForHT) HT += hfET[i];
		}
	}

	// Determine HT using hardware simulation

	uint16_t hlsHT = 0;
	MakeHT(rgnET, hfET, &hlsHT);

	// Compare

	printf("C says: HT = %d; HLS says: HT = %d\n", HT, hlsHT);;
	if(HT != hlsHT) {
		printf("Test failed\n");
		return 1;
	}
	else printf("Test succeeded\n");

	WriteLinkMapHT(rgnET, hfET, &hlsHT);

	return 0;

}
