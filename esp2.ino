#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <Adafruit_NeoPixel.h>
#include <HardwareSerial.h>

#define TXD1 19
#define RXD1 21

HardwareSerial Arduino_Serial(1);


// Pines de LEDS
const int PIN = 13;          // Pin donde está conectada la tira de leds
const int NUMPIXELS = 5;    // Número de leds conectados

boolean listening;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Datos de red
const char* ssid     = "Pasillo";      
const char* password = "123456789";    

// Crear servidor web en el puerto 80
AsyncWebServer server(80);

// Inicializar LittleFS
void initLittleFS() {
  if (!LittleFS.begin(true)) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  Serial.println("LittleFS mounted successfully");
}

void setup(){
  Serial.begin(115200);
  
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);                 
  IPAddress IP = WiFi.softAPIP();              
  Serial.print("AP IP address: ");             
  Serial.println(IP);                          
  Serial.println(WiFi.localIP());    

  Arduino_Serial.begin(9600, SERIAL_8N1, RXD1, TXD1);          

  initLittleFS();

  // Inicializar LEDs
  pixels.begin();
  pixels.show(); // Todos apagados al principio

  // Rutas del servidor
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.serveStatic("/", LittleFS, "/");

  server.on("/fase1", HTTP_GET, [](AsyncWebServerRequest *request){       
    request->send_P(200, "text/plain", "Hola");
    Serial.println("Fase 1 recibida");

    for (int i = 0; i < 2; i++) { 
      pixels.setPixelColor(i, 255, 0, 0); // Rojo en los dos primeros
    }

    for (int i = 2; i < 5; i++) { 
      pixels.setPixelColor(i, 0, 255, 0); // Verde en los siguientes
    }

    pixels.show(); // Enviar cambios al hardware

    
    listening = true;
    Arduino_Serial.print("1");
    Serial.println("Listening");
    

  });
  
  server.on("/fase2", HTTP_GET, [](AsyncWebServerRequest *request){    
    request->send_P(200, "text/plain", "Hola");
    Serial.println("Fase 2 recibida");
  });

  server.on("/fase3", HTTP_GET, [](AsyncWebServerRequest *request){       
    request->send_P(200, "text/plain", "Hola");
    Serial.println("Fase 3 recibida");
  });

  server.on("/results", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "Hola");
    Serial.println("Resultados requeridos");

    // Apagar todos los LEDs
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, 0, 0, 0); // Apagar cada LED
    }
    pixels.show(); // Enviar cambios

    while(listening){
      if (Arduino_Serial.available()) {
        listening = false;
        Arduino_Serial.print("0");
      }
    }
    
  });

  server.begin(); // Iniciar servidor
}

void loop(){  
  // Aquí podrías leer botones físicos, si quieres
  if (listening){
    if (Arduino_Serial.available()) {
      // Read data and display it
      String message = Arduino_Serial.readStringUntil('\n');
      Serial.println("Received: " + message);
    }
  }
}
