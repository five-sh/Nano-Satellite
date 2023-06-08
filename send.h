#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS++.h>

#define GPS_RX_PIN 5
#define GPS_TX_PIN 4

extern int message_count = 0;

TinyGPSPlus gps;
HardwareSerial gpsSerial(1);

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
	LoRa.setSyncWord(0xF5);
	Serial.println("LoRa Initializing OK!");
}

static float *get_coordinates() {
        float error[] = {91, 181};

        while (gpsSerial.available()) {
                char c = gpsSerial.read();
                if (gps.encode(c)) {
                        if (gps.location.isUpdated()) {
                                float latitude = gps.location.lat();
                                float longitude = gps.location.lng();
                                float coords[] = {latitude, longitude};

                                Serial.print("Latitude: ");
                                Serial.println(latitude, 6);
                                Serial.print("Longitude: ");
                                Serial.println(longitude, 6);

                                return coords;
                        }
                }
        }
        return error;
}

static String format_coords(float coordinates[]) {
        String message = "Latitude: " + String(coordinates[0]) + " Longitude: " + String(coordinates[1]);
        return message;
}

extern void send_message(String msg) {
        msg = format_coords(get_coordinates());
	Serial.print("Sending packet: ");
	Serial.println(message_count);

	LoRa.beginPacket();
	LoRa.print(msg);
	LoRa.print(message_count);
	LoRa.endPacket();

        Serial.println(msg);

	message_count++;

	delay(1000);
}