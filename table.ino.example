// Print the table of printable ASCII characters

#include <string>
#include <vector>

#include "printer-lib.h"

WiFiServer server(80);
Printer printer(D0, D1, D3, D2);

using namespace std;

void setup() {
  // Print a grid of every char between 1 and 7 and 14 and 256 (inclusive)
  // Other characters are spaces or will mess up the grid

  // Start with header
  printer.Print("**: 0|1|2|3|4|5|6|7|8|9");
  printer.Print("00: -|-|-|-|-|-|-|-|-|-");
  
  // Print the grid
  for (int t = 1; t < 26; t++) {
    string line = "";
    if (t < 10) {
      line += "0";
    }
    // Print the hundreds and tens digits
    line += to_string(t);
    line += ": ";
    for (int o = 0; o < 10; o++) {
      // The ones digit is the column.
      int n = t * 10 + o;
      if ((n >= 1 && n <= 7) || (n >= 14 && n <= 255)) {
        line += (char)n;
      } else {
        line += (char)127; // Block character
      }
      line += "|";
    }
    line.pop_back(); // Remove the last '|'
    printer.Print(line);
  }
}

void loop() {
}

