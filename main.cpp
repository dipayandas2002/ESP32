// Pin Recommendations : https://randomnerdtutorials.com/esp32-pinout-reference-gpios/

#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

// ==== WiFi and Firebase credentials ====
#define WIFI_SSID "POCOX3"
#define WIFI_PASSWORD "dipayandas2002"
#define ReadPin 33
#define WritePin 22
// #define PowerPin 23

#define DATABASE_URL "https://esp32ddas-default-rtdb.firebaseio.com/"
#define API_KEY "AIzaSyAsStgGUdvG2ol_PThdSvq_gwqj9PNGtfI"
#define USER_EMAIL "dipayandas360yt@gmail.com"
#define USER_PASSWORD "DipuJUETCE"

FirebaseData data;
FirebaseAuth auth;
FirebaseConfig config;

// bool signUpStatus = false; // Signup Status
int ReadVar = 0; // Reading the data from RTDB
int WriteVariable  = 0; // Writing to RTDB
unsigned long T_small = 0, T_large = 0;
int timeInterval = 250;


void setup() {
  pinMode(ReadPin, INPUT_PULLDOWN);
  pinMode(WritePin, OUTPUT);
  // pinMode(ReadPin, OUTPUT);
  Serial.begin(115200);


  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" Connected!");

  config.database_url = DATABASE_URL;
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  // if (Firebase.signUp(&config, &auth, "", "")) {
  //   signUpStatus = true;
  //   Serial.println("Signup Done");
  // } else {
  //   Serial.printf("Signup failed: %s\n", config.signer.signupError.message.c_str());
  // }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  T_large = millis();
  WriteVariable = digitalRead(ReadPin);
  // digitalWrite(PowerPin, HIGH);
  
  if (Firebase.ready()  &&  (T_large-T_small) >= timeInterval  ) {
    
    // Writing To DB
    if (Firebase.RTDB.setInt(&data, "/Light/One", WriteVariable)) {
      Serial.printf("Data Updated: %d\n", WriteVariable);
    } else {
      Serial.println("Failed to update");
    }
  
    // Reading To DB
    if (Firebase.RTDB.getInt(&data, "/Light/Two" , &ReadVar)) {
      Serial.printf("Data Printed: %d\n", ReadVar);
    } else {
      Serial.printf("Failed to Read : %s\n" , data.errorReason());
    }
  T_small = T_large;
  digitalWrite(WritePin, ReadVar ? HIGH : LOW);
  }

}
