// Pin Recommendations :
// https://i0.wp.com/randomnerdtutorials.com/wp-content/uploads/2018/08/ESP32-DOIT-DEVKIT-V1-Board-Pinout-36-GPIOs-updated.jpg?w=750&quality=100&strip=all&ssl=1
// https://circuitstate.com/wp-content/uploads/2022/12/ESP32-DevKit-V1-Pinout-Diagram-r0.1-CIRCUITSTATE-Electronics-2.png

#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

// ==== WiFi and Firebase credentials ====
#define WIFI_SSID "POCOX3"
#define WIFI_PASSWORD "dipayandas2002"

//Pin numbers are according to the GPIO and not the original pcb pin number 
#define ReadPin 33
#define WritePin 22
#define SwitchConnectedPin 23
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
unsigned long T_small = 0, T_large = 0;
int timeInterval = 250;
int SwitchReadVar = 0;

int PresState = 0, PrevState = 0;

void setup()
{
  pinMode(ReadPin, INPUT_PULLDOWN);
  pinMode(WritePin, OUTPUT);
  pinMode(SwitchConnectedPin, OUTPUT);
  // pinMode(ReadPin, OUTPUT);
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
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

void loop()
{
  T_large = millis();
  PresState = digitalRead(ReadPin);
  int state = 0;
  // digitalWrite(PowerPin, HIGH);

  if (Firebase.ready() && (T_large - T_small) >= timeInterval)
  {

    // Writing To DB

    // if (Firebase.RTDB.setInt(&data, "/Light/One", PresState)) {
    //   Serial.printf("Data Updated: %d\n", PresState);
    // } else {
    //   Serial.println("Failed to update");
    // }

    // TOGGLING FUNCTION
    if (PresState != PrevState)
    {
      if (Firebase.RTDB.getInt(&data, "/Light/One", &state))
      {
        if (Firebase.RTDB.setInt(&data, "/Light/One", !state))
        {
          // printf("Successful toggling");
        }
        // printf("Unsuccessful Toggle");
      }
    }

    // Reading To DB
    if (Firebase.RTDB.getInt(&data, "/Light/Two", &ReadVar))
    {
      // Serial.printf("Data Printed: %d\n", ReadVar);
        digitalWrite(WritePin, ReadVar ? HIGH : LOW);
    }
    else
    {
      // Serial.printf("Failed to Read : %s\n", data.errorReason());
    }

    // Controlling Led with the Switch
    if (Firebase.RTDB.getInt(&data, "/Light/One", &SwitchReadVar))
    {
      digitalWrite(SwitchConnectedPin, SwitchReadVar ? HIGH : LOW);
      // Serial.printf("Data Printed switch: %d\n", SwitchReadVar);
    }
    else
    {
      // Serial.printf("Failed to Read : %s\n", data.errorReason());
    }
    // loop
    T_small = T_large;
  
   
  }

  PrevState = PresState;
}
