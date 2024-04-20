// TODO: what does '\n' print?
// TODO: check every delay is needed
// TODO: swap sel_1 and sel_2?
// TODO: https://suckless.org/coding_style/

#include <string>
#include <vector>
#include "WiFi.h"

#include "secrets.h"
#include "printer-lib.h"

WiFiServer server(80);
Printer printer(D0, D1, D3, D2);

using namespace std;

void setup() {
	WiFi.begin(SECRET_SSID, SECRET_PASS);
	while (WiFi.status() != WL_CONNECTED) {}
	Serial.begin(9600);
	Serial.println("Connected to WiFi");
	Serial.println(WiFi.localIP());
	server.begin();
}

void loop() {
	// See if we have a new client
	WiFiClient client = server.available();
	if (client) {
		// Start the socket communication.
		// Get as much lines as there are and print them.
		while (client.connected()) {
			if (client.available()) {
				String line = client.readStringUntil('\n');
				printer.Print(line.c_str());
				// Return the printed line to the client so they can verify it
				// was actually printed.
				client.print(line + "\n");
			}
		}
	}
}

