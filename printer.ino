// TODO: what does '\n' print?
// TODO: check every delay is needed
// TODO: swap sel_1 and sel_2?
// TODO: https://suckless.org/coding_style/

#include <string>
#include <vector>
#include "printer-lib.h"

using namespace std;

void setup() {
    Printer printer(D0, D1, D3, D2);

    string toprint = "This is a test of the printer.\nAnd this is another line.";

    printer.Print(toprint);
}

void loop() {
}

