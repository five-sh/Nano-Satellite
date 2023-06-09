#include <SPI.h>
#include <LoRa.h>

extern int message_count = 0;

extern void init_LoRa_sender(int SS, int RST, int DIO0) {
	Serial.begin(115200);
	
	while (!Serial)
		;
	Serial.println("LoRa Sender");

	LoRa.setPins(SS, RST, DIO0);
	
	while (!LoRa.begin(868E6)) {
		Serial.println(".");
		delay(500);
	}

	/*
	 * The sync word should match that of the reciever, it ensures
	 * that we don't receive data from other receivers.
	 */
	LoRa.setSyncWord(0xF3);
	Serial.println("LoRa Initializing OK!");
}

extern float read_battery_info() {
        /*
         * Read analog info, instead of the usual digital info from pin 33      
         */
        float battery_level = map(analogRead(33), 0.0f, 4095.0f, 0, 100);
        return battery_level;
}

extern void send_message() {
        String msg;
        float battery_info = read_battery_info();

	Serial.print("Sending packet: ");
	Serial.println(message_count);

        msg = "Battery info: " + String(battery_info);
	LoRa.beginPacket();
	LoRa.print(msg);
	LoRa.print(message_count);
	LoRa.endPacket();

        /*
         * Also print the packet to the Serial Monitor
         */
        Serial.println(msg);

	message_count++;

	delay(10000);
}