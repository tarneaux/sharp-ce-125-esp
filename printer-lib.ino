void Printer::Print(string text) {
    // Handle newlines.
    vector<string> paragraphs = this->SplitLines(text);

    for (string paragraph: paragraphs) {
        this->PrintParagraph(paragraph);
    }
}

void Printer::PrintParagraph(string paragraph) {
    // Print the text, splitting in up into 24 character lines.
    size_t max_length = 24;
    vector<string> lines = this->WordWrap(paragraph, max_length);
    for (string line: lines) {
        this->PrintLine(line);
    }
}

Printer::Printer(int busy, int d_out, int sel_1, int sel_2) {
    this->busy = busy;
    this->d_out = d_out;
    this->sel_1 = sel_1;
    this->sel_2 = sel_2;

    this->Init_Pins();
    this->Set_all(false);
}

void Printer::PrintLine(string line) {
    // Add spaces to make the line exactly 24 characters long.
    while (line.length() < 24) {
        line += ' ';
    }
    this->Begin();
    for (char c: line) {
        this->PrintByte(c);
    }
    this->End();
}

void Printer::PrintByte(byte b) {
    // Print all 8 bits of the byte 1 by 1
    for (int i = 0; i < 8; i++) {
        bool bit_i = b >> i & 1; // Get bit i, starting from the right
                                 // (Little Endian)
        digitalWrite(this->d_out, bit_i);
        digitalWrite(this->busy, HIGH);
        delay(1);
        digitalWrite(this->busy, LOW);
        delay(1);
    }
}

void Printer::Begin() {
    this->Set_all(true);
    this->SelectPrinter();
    digitalWrite(this->sel_2, HIGH); // TODO: should we move this to
                                     // SelectPrinter?
}

void Printer::End() {
    this->PrintByte(13); // carriage return ('\r')
    delay(1); // TODO: is this necessary?
    digitalWrite(this->sel_2, LOW);
    delay(1200); // TODO: can we move this to a class variable that keeps track
                 // of whether or not we are printing?
}

void Printer::SelectPrinter() {
    // I have no documentation for this, but I think it's to tell the cassette
    // reader / printer combo that we are talking to the printer.
    digitalWrite(this->sel_1, HIGH);
    digitalWrite(this->sel_2, HIGH);
    delay(1);
    digitalWrite(this->sel_1, LOW);
    delay(3);
    digitalWrite(this->sel_2, LOW);
    delay(5);
}

void Printer::Set_all(bool value) {
    for (int i: {this->busy, this->d_out, this->sel_1, this->sel_2}) {
        digitalWrite(i, value);
    }
}

void Printer::Init_Pins() {
    for (int i: {this->busy, this->d_out, this->sel_1, this->sel_2}) {
        pinMode(i, OUTPUT);
    }
}

vector<string> Printer::WordWrap(
    std::string text,
    int max_length
) {
    vector<string> lines;
    size_t start = 0;

	// If we are printing an empty string, then just print a blank line.
	// Otherwise, we would return an empty vector, which would print nothing.
	if (text.length() == 0) {
		lines.push_back("");
		return lines;
	}

    while (start < text.length()) {
        size_t max_end = start + max_length;
        size_t end;

        if (max_end >= text.length()) {
            // There are less characters left than the max length.
            // The final line will be the remaining characters.
            end = text.length();
        } else {
            // Find the last space before the max length.
            end = text.rfind(' ', max_end);
            // If there is no space in that section, then just take a substring
            // of length max_length.
            if (end == std::string::npos || end < start) {
                end = max_end;
            }
        }

        size_t length = end - start;
        string substring = text.substr(start, length);

        lines.push_back(substring);

        start = end;
        // Skip any spaces at the beginning of the next line.
        while (start < text.length() && text[start] == ' ') {
            start++;
        }
    }
    return lines;
}

vector<string> Printer::SplitLines(
    std::string text
) {
    vector<string> lines;
    size_t start = 0;

    while (start < text.length()) {
        size_t end = text.find('\n', start);
        if (end == std::string::npos) {
            end = text.length();
        }
        size_t length = end - start;
        string substring = text.substr(start, length);

        lines.push_back(substring);

        start = end + 1;
    }
    return lines;
}
