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
boolean check_results = false;
int mode;

struct analisis {
  int long_z;
  int long_p;
};

struct analisis resultados;

String message;

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

void end_communication(){
    listening = false;
    Arduino_Serial.print("0");
    }


void start_communication(){
    listening = true;
    Arduino_Serial.print("1");
    Serial.println("Listening");
}

struct analisis read_string(String vector, int config){
  int i=0;
  struct analisis inside_s;
  boolean pie = false;
  boolean entre = false;
  boolean stay = true;
  int suma1 = 0;
  int suma2 = 0;
  Serial.println("entraste a la funcion");


  switch(config){
    case (0):
      Serial.println("caso 0");
      inside_s.long_p = 0;
      inside_s.long_z = 0;
      break;
    case (1):
      Serial.println("caso 1");
      for (i= 0; i<vector.length(), stay==true; i++){
        if (vector[i] == '0'){
          if (entre){
            stay = false;
          }
          else {
            pie = true;
            suma1 += 2;
          }
        }
        else if (vector[i] == '1'){
          if (pie){
            entre = true;
            pie = false;
            suma1 += 2;
          }
          else if (entre) {
            suma1 += 2;
          }
        }
      }
      inside_s.long_p = suma1;
      inside_s.long_z = 0;
      break;

    case (2):
      Serial.println("caso 2");
      inside_s.long_p = 50;
      inside_s.long_z = 10;
      break;
    case (3):
      Serial.println("caso 3");
      inside_s.long_p = 25;
      inside_s.long_z = 35;
      break;
  }

  Serial.println("devolviendo");
  return inside_s; 
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

    mode = 1;
    start_communication(); 
  });
  
  server.on("/fase2", HTTP_GET, [](AsyncWebServerRequest *request){    
    request->send_P(200, "text/plain", "Hola");
    Serial.println("Fase 2 recibida");

    mode = 2;
    start_communication();
  });

  server.on("/fase3", HTTP_GET, [](AsyncWebServerRequest *request){       
    request->send_P(200, "text/plain", "Hola");
    Serial.println("Fase 3 recibida");

    mode = 3;
    start_communication();
  });

  server.on("/results", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "Hola");
    Serial.println("Resultados requeridos");

    // Apagar todos los LEDs
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, 0, 0, 0); // Apagar cada LED
    }
    pixels.show(); // Enviar cambios

    end_communication();
    check_results = true;
    
    
  });

  server.begin(); // Iniciar servidor
}

void loop(){  
  // Aquí podrías leer botones físicos, si quieres
  if (listening){
    if (Arduino_Serial.available()) {
      // Read data and display it
      message = Arduino_Serial.readStringUntil('\n');
      Serial.println("Received: " + message);
    }
  }
  else {
    if (check_results){
      Serial.print("check results");
      resultados = read_string(message, mode);
      mode = 0;
      check_results = false;

      Serial.print("Longitud de paso: ");             //debugging
      Serial.println(resultados.long_p);
      Serial.print("Longitud de zancada: ");
      Serial.println(resultados.long_z);
    }
  }
}
