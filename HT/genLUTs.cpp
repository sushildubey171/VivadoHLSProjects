#include <stdint.h>
#include <iostream>
#include <iomanip>
using namespace std;
#define NCrds 7
#define NRgns 2
#define NHFRgns 8
int main(int argc, char** argv) {
  cout << "#ifndef LUTs_h" << endl;
  cout << "#define LUTs_h" << endl;
  cout << "#include <stdint.h>" << endl;
  cout << "// Rgn LUT" << endl;
  cout << "static const uint12_t rgnETLUT[0x1000][NCrds*NRgns] = {" << endl;
  for(int i = 0; i < 0x1000; i++) {
    for(int j = 0; j < NCrds*NRgns; j++) {
      uint32_t value = i * (1 + 0.1 * j) + 0.1 * j; // For the heck of it
      if(value > 0xFFF) value = 0xFFF;
      if(j == 0) cout << "{" << showbase << internal << setw(6) << setfill('0') << hex << value << ", ";
      else if(j == (NCrds*NRgns - 1)) cout << showbase << internal << setw(6) << setfill('0') << hex << value << "}";
      else cout << showbase << internal << setw(6) << setfill('0') << hex << value << ", ";
    }
    if(i != 0xFFF) cout << "," << endl;
  }
  cout << endl << "};" << endl;
  cout << "// HF LUT " << endl;
  cout << "static const uint12_t hfETLUT[0x1000][NHFRgns] = {" << endl;
  for(int i = 0; i < 0x1000; i++) {
    for(int j = 0; j < NHFRgns; j++) {
      uint32_t value = i * (1 + 0.1 * j) + 0.1 * j; // For the heck of it
      if(value > 0xFFF) value = 0xFFF;
      if(j == 0) cout << "{" << showbase << internal << setw(6) << setfill('0') << hex << value << ", ";
      else if(j == (NHFRgns - 1)) cout << showbase << internal << setw(6) << setfill('0') << hex << value << "}";
      else cout << showbase << internal << setw(6) << setfill('0') << hex << value << ", ";
    }
    if(i != 0xFFF) cout << "," << endl;
  }
  cout << endl << "};" << endl;
  cout << "#endif" << endl;
}
