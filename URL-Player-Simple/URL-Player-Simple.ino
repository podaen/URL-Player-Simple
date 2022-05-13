/*
 Name:		URL_Player_Simple.ino
 Created:	5/11/2022 11:50:58 AM
 Author:	Podaen
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <esp_bt_main.h>//power saving bluetooth
#include <esp_bt.h>//power saving bluetooth

const char* ssid = "SSID";
const char* password = "PASS";
#define BUFFER_SIZE 512
unsigned long last1 = 0;
unsigned long time1 = 0;
unsigned long last2 = 0;
unsigned long time2 = 0;
unsigned long last3 = 0;
unsigned long time3 = 0;
// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(500000);
	while (!Serial);

	WiFi.mode(WIFI_MODE_STA);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(1000);
		// Serial.println("Connecting to WiFi..");
	}
    Serial.println("Connected");
	WiFi.setSleep(false);

	esp_bluedroid_disable();
	esp_bluedroid_deinit();
	esp_bt_controller_disable();
	esp_bt_controller_deinit();
    Serial.println("end setup");

}

// the loop function runs over and over again until power down or reset
void loop() {
	if (WiFi.status() == WL_CONNECTED) {
		HTTPClient http;
        //http = new HTTPClient;
		http.begin("http://22183.live.streamtheworld.com/JOE.mp3");
       // http.begin("http://sunshineradio.ice.infomaniak.ch/sunshineradio-128.mp3");
		Serial.println("[HTTP] begin...\n");

        //http.addHeader("Host", "sunshineradio.ice.infomaniak.ch");
        http.addHeader("Host", "22183.live.streamtheworld.com");
        http.addHeader("Connection", "close");
        http.addHeader("Accept-Encoding", "");
        http.addHeader("Accept", "*/*");
        http.setUserAgent("Mozilla/5.0 (compatible; Googlebot/2.1; +http://www.google.com/bot.html)");

        http.setTimeout(30000);
        int httpCode = http.GET();
        if (httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK) {
                Serial.print("HTTP_CODE_OK: ");
                Serial.println(HTTP_CODE_OK);

                uint8_t buffer[512] = { 0 };
                int buffer_size = 512;

                WiFiClient* stream = http.getStreamPtr();
                //stream* stream = http.getStreamPtr();
                delay(5000);
                size_t result = stream->available();
                //int result = stream->available();
                Serial.println("read bytes");
                if (result > 0) {
                    while (http.connected()) {
                        //last1 = millis();
                        result = stream->available();
                        if (result > 2 * buffer_size) {
                        //if (result >= buffer_size) {
                            time3 = millis() - last3;
                            if (time3 > 700) {
                                Serial.print("ms reads: ");
                                Serial.println(time3);
                                Serial.println("");
                            }
                            //time1 = millis() - last1;
                            //if (time1 > 5) {
                            //    Serial.print("ms to available: ");
                            //    Serial.println(time1);
                            //    Serial.println("");
                            //}
                            size_t size = min((size_t)result, static_cast<size_t>(buffer_size));
                            //Serial.print("size: ");
                            //Serial.println(size);
                            
                            //last2 = millis();
                            int read = stream->read((uint8_t*)buffer, size);
                            //delay(100);
                            if (read > 0) {
                                if (read < 512) {
                                    Serial.print("read: ");
                                    Serial.println(read);
                                    Serial.println("");
                                }
                                //time2 = millis() - last2;
                                //if (time2 > 5) {
                                //    Serial.print("ms to read: ");
                                //    Serial.println(time2);
                                //    Serial.println("");
                                //}
                                last3 = millis();
                            }
                        }
                        else
                        {

                        }
                    }
                }

                Serial.println();
                Serial.print("[HTTP] connection closed or file end.\n");
            }
        }
        else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }
    delay(10000);
}