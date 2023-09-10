#include <string>
#include <vector>

using namespace std;

class Printer {
    public:
        Printer(int busy, int d_out, int sel_1, int sel_2);
        void Print(string text);
    private:
        void PrintParagraph(string paragraph);
        void Begin();
        void End();
        void SelectPrinter();
        void Set_all(bool value);
        void Init_Pins();
        void PrintByte(byte b);
        void PrintLine(string line);
        static vector<string> WordWrap(
            string text,
            int max_length
        );
        static vector<string> SplitLines(
            string text
        );
        int busy;
        int d_out;
        int sel_1;
        int sel_2;
};

