#include <WiFi.h>
#include <HTTPClient.h>

// Wi-Fi credentials
const char* ssid = "E"; 
const char* password = "edilawit123"; 

// Server URL
const char* serverUrl = "http://192.168.82.140:3000/data"; 

// Voltage sensor pin
const int voltagePin = 34; // Analog pin connected to the voltage sensor

// Voltage calculation constants
const float referenceVoltage = 3.3; // ESP32 ADC reference voltage (typically 3.3V)
const int adcResolution = 4095;     // 12-bit ADC resolution
const float voltageDividerRatio = 2.0; // Adjust for your sensor's voltage divider (if any)

void setup() {
  // Start the serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("Connected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    // Read the analog voltage
    int rawValue = analogRead(voltagePin);
    float voltage = (rawValue * referenceVoltage / adcResolution) * voltageDividerRatio;

    // Print the voltage to the Serial Monitor
    Serial.print("Voltage: ");
    Serial.println(voltage);

    // Prepare the JSON data
    String jsonData = "{\"voltage\":" + String(voltage, 2) + "}";

    // Send data to the server
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json"); // Specify content type

    int httpResponseCode = http.POST(jsonData); // Send the request

    // Check the response
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    
    http.end(); // Free resources
  } else {
    Serial.println("WiFi disconnected!");
  }

  delay(1000); // Send data every 10 seconds
}
