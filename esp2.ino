#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <Adafruit_NeoPixel.h>
#include <HardwareSerial.h>

#define TXD1 19
#define RXD1 21
#define offset_huella 36       //hay que calcular fisicamente el offset entre el talon de la huella y la primera linea de medida

HardwareSerial Arduino_Serial(1);


// Pines de LEDS
const int PIN = 25;          // Pin donde está conectada la tira de leds
const int NUMPIXELS = 55;    // Número de leds conectados 54

const int PIN2 = 26;          // Segundo pin donde va la otra tira LED
const int NUMPIXELS2 = 55;     // Número de LEDs en esa segunda tira


boolean listening;
boolean prepare_results = false;
int mode;

struct analisis {
  int long_z;
  int long_p1;
  int long_p2;
};

struct analisis resultados;

String message, message_e;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_BRG + NEO_KHZ800);
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(NUMPIXELS2, PIN2, NEO_BRG + NEO_KHZ800);

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
  boolean first = true;
  int suma1 = 0;
  int suma2_1 = 0;
  int suma2_2 = 0;
  Serial.println("entraste a la funcion");


  switch(config){
    case (0):
      Serial.println("caso 0");
      inside_s.long_p1 = 0;
      inside_s.long_p2 = 0;
      inside_s.long_z = 0;
      break;

    case (1):
      Serial.println("caso 1");
      for (i= 0; i<vector.length() && first==true; i++){
        if (vector [i] == '1'){
          suma1 += 2;
        }
        else{
          first = false;
        }
      } 
      inside_s.long_p1 = suma1 + offset_huella;
      inside_s.long_p2 = 0;
      inside_s.long_z = 0;
      break;

    default:
      Serial.println("caso 2");
      for (i= 0; i<vector.length() && stay==true; i++){
        if (vector[i] == '0'){
          if (entre){
            stay = false;
          }
          else {
            pie = true;
            suma2_1 += 2;
          }
        }
        else if (vector[i] == '1'){
          if (pie){
            entre = true;
            pie = false;
            suma2_1 += 2;
          }
          else if (entre) {
            suma2_1 += 2;
          }
          else {
            suma1 += 2;
          }
        }
        suma2_2 += 2;
      }
      inside_s.long_p2 = suma2_1;      
      inside_s.long_p1 = suma1 + offset_huella;
      inside_s.long_z = suma2_2 - 2 + offset_huella;
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

  // Inicializar LED1
  pixels.begin();
  pixels.setBrightness(10);
  pixels.show(); // Todos apagados al principio

  // Inicializar LED1
  pixels2.begin();
  pixels2.setBrightness(10);
  pixels2.show(); // Apagado al inicio

  // Rutas del servidor
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.serveStatic("/", LittleFS, "/");

  server.on("/fase1", HTTP_GET, [](AsyncWebServerRequest *request){       
    request->send_P(200, "text/plain", "Hola");
    Serial.println("Fase 1 recibida");

    for (int i = 0; i < 3; i++) { 
      pixels.setPixelColor(i, 255, 0, 0); // ROJO
      pixels2.setPixelColor(i, 255, 0, 0); // 
    }

    for (int i = 3; i < 17; i++) { 
      pixels.setPixelColor(i,0, 0, 255); // Verde 
      pixels2.setPixelColor(i,0, 0, 255);
    }
    
    for (int i = 17; i < 55; i++) { 
      pixels.setPixelColor(i, 255, 0, 0); // ROJO
      pixels2.setPixelColor(i, 255, 0, 0);
    }

    pixels.show(); // Enviar cambios al hardware
    pixels2.show(); // Enviar cambios al hardware

    mode = 1;
    start_communication(); 
  });
  
  server.on("/fase2", HTTP_GET, [](AsyncWebServerRequest *request){    
    request->send_P(200, "text/plain", "Hola");
    Serial.println("Fase 2 recibida");

    for (int i = 0; i < 3; i++) { 
      pixels.setPixelColor(i, 255, 0, 0); // ROJO
      pixels2.setPixelColor(i, 255, 0, 0); // 
    }

    for (int i = 3; i < 17; i++) { 
      pixels.setPixelColor(i,0, 0, 255); // Verde 
      pixels2.setPixelColor(i,0, 0, 255);
    }
    
    for (int i = 17; i < 23; i++) { 
      pixels.setPixelColor(i, 255, 0, 0); // ROJO
      pixels2.setPixelColor(i, 255, 0, 0); // 
    }

    for (int i = 23; i < 37; i++) { 
      pixels.setPixelColor(i, 0, 0, 255); // Verde 
      pixels2.setPixelColor(i, 0, 0, 255);
    }

    for (int i = 37; i < 55; i++) { 
       pixels.setPixelColor(i, 255, 0, 0); // ROJO
      pixels2.setPixelColor(i, 255, 0, 0); // 
    }

    pixels.show(); // Enviar cambios al hardware
    pixels2.show(); // Enviar cambios al hardware

    mode = 2;
    start_communication();
  });

  server.on("/fase3", HTTP_GET, [](AsyncWebServerRequest *request){       
    request->send_P(200, "text/plain", "Hola");
    Serial.println("Fase 3 recibida");
    
    for (int i = 0; i < 12; i++) { 
      pixels.setPixelColor(i, 255, 0, 0); // ROJO
      pixels2.setPixelColor(i, 255, 0, 0); // 
    }

    for (int i = 12; i < 26; i++) { 
      pixels.setPixelColor(i,0, 0, 255); // Verde 
      pixels2.setPixelColor(i,0, 0, 255);
    }
    
    for (int i = 26; i < 38; i++) { 
      pixels.setPixelColor(i, 255, 0, 0); // ROJO
      pixels2.setPixelColor(i, 255, 0, 0); // 
    }

    for (int i = 38; i < 52; i++) { 
      pixels.setPixelColor(i, 0, 0, 255); // Verde 
      pixels2.setPixelColor(i, 0, 0, 255);
    }

    for (int i = 52; i < 55; i++) { 
       pixels.setPixelColor(i, 255, 0, 0); // ROJO
      pixels2.setPixelColor(i, 255, 0, 0); // 
    }

    pixels.show(); // Enviar cambios al hardware
    pixels2.show(); // Enviar cambios al hardware
    mode = 3;
    start_communication();
  });

  server.on("/results", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", message_e);
    Serial.println("Resultados requeridos");

    // Apagar todos los LEDs
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, 0, 0, 0); // Apagar cada LED
      pixels2.setPixelColor(i, 0, 0, 0); // Apagar cada LED
    }
    pixels.show(); // Enviar cambios
    pixels2.show(); // Enviar cambio

    end_communication();

    Serial.print("Longitud de paso1: ");             //debugging
    Serial.println(resultados.long_p1);
    Serial.print("Longitud de zancada: ");
    Serial.println(resultados.long_z);
    Serial.print("Longitud de paso2: ");
    Serial.println(resultados.long_p2);
    
    
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
      prepare_results = true;
    }
  }

  if (prepare_results){
      Serial.println("preparando datos");
      resultados = read_string(message, mode);
      prepare_results = false;
      message_e = "";
      message_e += resultados.long_p1;
      message_e += ",";
      message_e += resultados.long_z;
      message_e += ",";
      message_e += resultados.long_p2;
  }
}