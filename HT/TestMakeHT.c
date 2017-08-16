#include <stdint.h>
#include <stdio.h>

#include "MakeHT.h"

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
  printf("s_OUTPUT_LINK_ARR( 0 )(31 downto 16) <= HT_1;\n");
  printf("s_OUTPUT_LINK_ARR( 0 )(47 downto 32) <= HT_2;\n");
}

int main(int argc, char **argv) {

  uint16_t rgnET[NCrts*NCrds*NRgns];
  uint16_t hfET[NCrts*NHFRgns];
  uint16_t et;
  uint16_t HT[3] = {0, 0, 0};
  uint16_t hlsHT[3] = {0, 0, 0};

  uint32_t rgnHT = 0;
  uint32_t hfHT = 0;
  uint32_t sum = 0;

  int iCrt;
  int iCrd;
  int iRgn;
  int i;
  int j;
  int iHFRgn;

  // Test data; Construct it using indices for the fun of it
  
  for(iRgn = 0; iRgn < NCrts * NCrds * NRgns; iRgn++) {
    rgnET[iRgn] = iRgn / 2;
  }
  for(iHFRgn = 0; iHFRgn < NCrts * NHFRgns; iHFRgn++) {
    hfET[iHFRgn] = iHFRgn;
  }

  // Determine HT using software

  for(iRgn = 0; iRgn < NCrts * NCrds * NRgns; iRgn++) {
    j = (iRgn % (NCrds*NRgns));
    et = rgnETLUT[rgnET[iRgn]][j];
    if(et > MinETCutForHT) rgnHT += et;
    if(rgnHT > 0xFFFF) rgnHT = 0xFFFF;
  }
  for(iHFRgn = 0; iHFRgn < NCrts * NHFRgns; iHFRgn++) {
    j = (iHFRgn % (NHFRgns));
    et = hfETLUT[hfET[iHFRgn]][j];
    if(et > MinHFETCutForHT) hfHT += et;
    if(hfHT > 0xFFFF) hfHT = 0xFFFF;
  }
  sum = rgnHT + hfHT;
  if(sum > 0xFFFF) sum = 0xFFFF;
  HT[0] = (uint16_t) sum;
  HT[1] = (uint16_t) rgnHT;
  HT[2] = (uint16_t) hfHT;

  // Determine HT using hardware simulation

  MakeHT(rgnET, hfET, hlsHT);

  // Compare

  printf("C says: HT = %d; HLS says: HT = %d\n", HT[0], hlsHT[0]);
  printf("C says: rgnHT = %d; HLS says: rgnHT = %d\n", HT[1], hlsHT[1]);
  printf("C says: hfHT = %d; HLS says: hfHT = %d\n", HT[2], hlsHT[2]);
  if(HT[0] != hlsHT[0]) {
    printf("Test failed\n");
    return 1;
  }
  else printf("Test succeeded\n");

  WriteLinkMapHT(rgnET, hfET, hlsHT);

  return 0;

}
