// Import required libraries
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ezButton.h>
#include <LittleFS.h>


//declaraciones de los pines que serán usados para las huellas
const int pin1_0 = 4;
const int pin1_1 = 5;
const int pin1_2 = 6;
const int pin1_3 = 7;
/*
const int pin2_0 = 5;
const int pin2_1 = 6;
const int pin2_2 = 7;
const int pin2_3 = 8;

*/

ezButton pie1_0(pin1_0);
ezButton pie1_1(pin1_1);  
ezButton pie1_2(pin1_2);  
ezButton pie1_3(pin1_3); 
/*
ezButton pie2_0(pin2_0);
ezButton pie2_1(pin2_1);  
ezButton pie2_2(pin2_2);  
ezButton pie2_3(pin2_3); 

*/

const char* ssid     = "Pasillo";       //nombre de la red WiFi, si la creas tú o si te conectas a una existente
const char* password = "123456789";   //Contraseña de la que crees tú o de la que te conectes


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
 

//const char index_html[] PROGMEM


void initLittleFS() {
  if (!LittleFS.begin(true)) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  Serial.println("LittleFS mounted successfully");
}


void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);                  //para crear un punto de acceso
  IPAddress IP = WiFi.softAPIP();               //...
  Serial.print("AP IP address: ");              //... 
  Serial.println(IP);                           //...
  Serial.println(WiFi.localIP());               //...

  initLittleFS();

  /*                      
  Serial.print("Connecting to ");               //para conectarse a una red wifi existente
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());               //hasta aquí, descomentar esto y comentar lo otro
  */

 server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.serveStatic("/", LittleFS, "/");


  server.on("/fase1", HTTP_GET, [](AsyncWebServerRequest *request){       //funcion al pulsar el boton fase1
    request->send_P(200, "text/plain", "Hola");
    Serial.println("Fase 1 recibida");
  });
  
  server.on("/fase2", HTTP_GET, [](AsyncWebServerRequest *request){       //funcion al pulsar el boton fase2
    request->send_P(200, "text/plain", "Hola");
    Serial.println("Fase 2 recibida");
  });



  server.on("/fase3", HTTP_GET, [](AsyncWebServerRequest *request){       //funcion al pulsar el boton fase3
    request->send_P(200, "text/plain", "Hola");
    Serial.println("Fase 3 recibida");
  });

  server.on("/results", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "Hola");
    Serial.println("Resultados requeridos");
  });


    
  // Start server
  server.begin();
}
 
void loop(){  
  
}