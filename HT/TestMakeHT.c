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
  int iRgn, iHFRgn, link, loBit, hiBit;
  for(iRgn = 0; iRgn < NCrts*NCrds*NRgns; iRgn++) {
    printf("rgnET_%d : IN STD_LOGIC_VECTOR (15 downto 0);\n", iRgn);
  }
  for(iHFRgn = 0; iHFRgn < NCrts * NHFRgns; iHFRgn++) {
    printf("hfET_%d : IN STD_LOGIC_VECTOR (15 downto 0);\n", iHFRgn);
  }
  printf("HT_0 : OUT STD_LOGIC_VECTOR (15 downto 0);\n\n\n");
  // Block 2
  for(iRgn = 0; iRgn < NCrts*NCrds*NRgns; iRgn++) {
    printf("signal rgnET_%d : STD_LOGIC_VECTOR(15 DOWNTO 0);\n", iRgn);
  }
  for(iHFRgn = 0; iHFRgn < NCrts * NHFRgns; iHFRgn++) {
    printf("signal hfET_%d : STD_LOGIC_VECTOR(15 DOWNTO 0);\n", iHFRgn);
  }
  printf("signal HT_0 : STD_LOGIC_VECTOR (15 downto 0);\n\n\n");
  // Block 3
  for(iRgn = 0; iRgn < NCrts*NCrds*NRgns; iRgn++) {
    printf("rgnET_%d => rgnET_%d,\n", iRgn, iRgn);
  }
  for(iHFRgn = 0; iHFRgn < NCrts * NHFRgns; iHFRgn++) {
    printf("hfET_%d => hfET_%d,\n", iHFRgn, iHFRgn);
  }
  printf("HT_0 => HT_0,\n\n\n");
  // Block 4
  for(iRgn = 0; iRgn < NCrts*NCrds*NRgns; iRgn++) {
    // Each link can carry 192-bits, or 12x16-bits of data
    // Each iRgn needs 16 bits
    link = (iRgn / 12);
    loBit = (iRgn % 12) * 16;
    hiBit = loBit + 15;
    printf("rgnET_%d <= s_INPUT_LINK_ARR( %d )(%d downto %d);\n", iRgn, link, hiBit, loBit);
  }
  for(iHFRgn = 0; iHFRgn < NCrts * NHFRgns; iHFRgn++) {
    // Each link can carry 192-bits, or 12x16-bits of data
    // Each iHFRgn needs 16 bits
    link = (iHFRgn / 12) + 21;
    loBit = (iHFRgn % 12) * 16;
    hiBit = loBit + 15;
    printf("hfET_%d <= s_INPUT_LINK_ARR( %d )(%d downto %d);\n", iHFRgn, link, hiBit, loBit);
  }
  printf("s_OUTPUT_LINK_ARR( 0 )(15 downto 0) <= HT_0;\n");
}

int main(int argc, char **argv) {

  uint16_t rgnET[NCrts*NCrds*NRgns];
  uint16_t hfET[NCrts*NHFRgns];
  uint16_t et;
  uint16_t HT = 0;
  uint16_t hlsHT[1] = {0};

  int iCrt;
  int iCrd;
  int iRgn;
  int i;
  int j;
  int iHFRgn;

  // Setup LUTs

  for(i = 0; i < 0x10000; i++) {
    for(iRgn = 0; iRgn < NCrds * NRgns; iRgn++) {
      if(iRgn < (NCrds * NRgns)/2) {
	rgnETLUT[i][iRgn] = (int) 1.1 * (float) i;
      }
      else {
	rgnETLUT[i][iRgn] = i;
      }
    }
    for(iHFRgn = 0; iHFRgn < NHFRgns; iHFRgn++) {
      hfETLUT[i][iHFRgn] = (int) 1.3 * (float) i;
    }
  }


  // Test data; Construct it using indices for the fun of it
  
  for(iRgn = 0; iRgn < NCrts * NCrds * NRgns; iRgn++) {
    rgnET[iRgn] = iRgn;
  }
  for(iHFRgn = 0; iHFRgn < NHFRgns; iHFRgn++) {
    hfET[iHFRgn] = iHFRgn;
  }

  // Determine HT using software

  for(iRgn = 0; iRgn < NCrts * NCrds * NRgns; iRgn++) {
    j = (iRgn % (NCrds*NRgns));
    et = rgnETLUT[rgnET[iRgn]][j];
    if(et > MinETCutForHT) HT += et;
  }
  for(iHFRgn = 0; iHFRgn < NCrts * NHFRgns; iHFRgn++) {
    j = (iHFRgn % (NHFRgns));
    et = hfETLUT[hfET[iHFRgn]][j];
    if(et > MinHFETCutForHT) HT += et;
  }

  // Determine HT using hardware simulation

  MakeHT(rgnET, hfET, hlsHT);

  // Compare

  printf("C says: HT = %d; HLS says: HT = %d\n", HT, hlsHT[0]);;
  if(HT != hlsHT[0]) {
    printf("Test failed\n");
    return 1;
  }
  else printf("Test succeeded\n");

  WriteLinkMapHT(rgnET, hfET, hlsHT);

  return 0;

}
