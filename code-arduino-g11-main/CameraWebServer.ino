#include "esp_camera.h"
#include "Ultrasonic.h"
#include <Tone32.h>
#include <Firebase_ESP_Client.h>
#include <HTTPClient.h>
#include <WiFi.h>


#define CAMERA_MODEL_AI_THINKER  // Has PSRAM
#define API_KEY "AIzaSyDGbnhVQGbMgCp9t47uLzJnnLAeYRfgg70"
#define DATABASE_URL "https://wallwatcher-319e4-default-rtdb.europe-west1.firebasedatabase.app/"
#include "camera_pins.h"
#include "partition.h"
#define BUZZER_PIN 15
#define BUZZER_CHANNEL 13
#define ESP_ID 1
// ===========================
// Enter your WiFi credentials
// ===========================
const char* ssid = "ESP32";
const char* password = "aaaaaaaa";
//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig configFirebase;
bool signupOK = false;
unsigned long sendDataPrevMillis = 0;

String hostname = "https://api-wallwatcher-test.onrender.com";




bool buzzerActivate = false;
int frequence;
unsigned int note;
String oldFrequency = "";

void startCameraServer();
void setupLedFlash(int pin);
Ultrasonic ultrasconic(14);

void setup() {
  Serial.begin(115200);


  Serial.setDebugOutput(true);
  Serial.println();
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;  // for streaming
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;
  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }
#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  sensor_t* s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);        // flip it back
    s->set_brightness(s, 1);   // up the brightness just a bit
    s->set_saturation(s, -2);  // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }
#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif
#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif
// Setup LED FLash if LED pin is defined in camera_pins.h
#if defined(LED_GPIO_NUM)
  setupLedFlash(LED_GPIO_NUM);
#endif

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  startCameraServer();
  Serial.print("Camera Ready! Use ‘http://");
  Serial.print(WiFi.localIP());
  Serial.println("’ to connect");
  /* Assign the api key (required) */
  /* Assign the RTDB URL (required) */


  configFirebase.api_key = API_KEY;
  configFirebase.database_url =  DATABASE_URL;
  
  if (Firebase.signUp(&configFirebase,&auth,"","")){
    Serial.println("Connexion a firebase ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", configFirebase.signer.signupError.message.c_str());
  }



  Firebase.begin(&configFirebase, &auth);
  Firebase.reconnectWiFi(true);

}
void loop() {
  int RangeInCentimeters = ultrasconic.MeasureInCentimeters();
  if(RangeInCentimeters < 300){
  activeBuzzer(RangeInCentimeters,NOTE_C4);
  delay(RangeInCentimeters*2);
  } 
  Serial.println("distance get par range ");
  Serial.println(RangeInCentimeters);
  if(signupOK){
  setDistanceFirebase(RangeInCentimeters);
  }
  else {
     Serial.println("La connexion a firebase ne fonctionne pas");
  }
}

String formatId(int id) {
    // Convertir l'ID en chaîne de caractères
    String idStr = String(id);
    
    // Ajouter des zéros à gauche jusqu'à ce que la longueur soit de 10 caractères
    while (idStr.length() < 10) {
        idStr = "0" + idStr;
    }
    
    return idStr;
}

void set_distance_to_API(int distance) {
  HTTPClient http;
  const String deviceID = "000000001";
  const String apiEndpoint = "/set-distance";
  String postData = String("{\"distance\":") + String(distance) + String(",\"id\":\"") + String(deviceID) + String("\"}");
  http.begin(hostname + apiEndpoint);
  http.addHeader("Content-Type", "application/json");

  int httpCode = http.POST(postData);

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    } else {
      String errorPayload = http.getString();
      int startTagIndex = errorPayload.indexOf("<pre>") + 5;
      int endTagIndex = errorPayload.indexOf("</pre>");
      String errorMessage = errorPayload.substring(startTagIndex, endTagIndex);
      Serial.println("Error: " + errorMessage);
    }
  } else {
    Serial.printf("HTTP request failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}

void setDistanceFirebase(int distance){
  Serial.println(" passeage dans setdis : ");
  Serial.println(distance);
  Firebase.RTDB.setInt(&fbdo,"000000001/distance/distance",distance);
}

String getBuzzerNote() {
  HTTPClient http;

  http.begin(hostname +  "/buzzer-note");
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    http.end();
    return response;
  } else {
    Serial.printf("Échec de la requête, code : %d\n", httpCode);
    http.end();
    return "";
  }
}

String getBuzzerFrequency() {
  HTTPClient http;

  http.begin(hostname +  "/buzzer-frequency");
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    http.end();
    return response;
  } else {
    Serial.printf("Échec de la requête, code : %d\n", httpCode);
    http.end();
    return "";
  }
}



void activeBuzzer(long frequence, unsigned long note) {
    tone(BUZZER_PIN, note, 50, BUZZER_CHANNEL);
    noTone(BUZZER_PIN, BUZZER_CHANNEL);
}