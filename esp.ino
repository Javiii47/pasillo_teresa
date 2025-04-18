// Import required libraries
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ezButton.h>


//declaraciones de los pines que serán usados para las huellas
const int pin1_0 = 1;
const int pin1_1 = 2;
const int pin1_2 = 3;
const int pin1_3 = 4;
/*
const int pin2_0 = 5;
const int pin2_1 = 6;
const int pin2_2 = 7;
const int pin2_3 = 8;

const int pin3_0 = 9;
const int pin3_1 = 10;
const int pin3_2 = 11;
const int pin3_3 = 12;
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

ezButton pie3_0(pin3_0);
ezButton pie3_1(pin3_1);  
ezButton pie3_2(pin3_2);  
ezButton pie3_3(pin3_3); 
*/

const char* ssid     = "Brazo";       //nombre de la red WiFi, si la creas tú o si te conectas a una existente
const char* password = "123456789";   //Contraseña de la que crees tú o de la que te conectes


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
 

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
  <html lang = "es">
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1" charset="utf-8">
    <style>
        body {
            width: 100%;
            height: 100%;
            display: flex;
            flex-direction: column;
            align-items: center;  
            background: linear-gradient(to bottom, #a0d4e0, #265c68)
        }
        html {
         font-family: Arial;
         display: flex;
         flex-direction: column;
         align-items: center;  
         margin: 0px auto;
         text-align: center;
         height: 100%;
         width: 100%;
        }
        #botonera {
            height: 30%;
            display: flex;
            flex-direction: column;
            justify-content: space-evenly;
            width: 75%;
            height: 100%;
        }
        .spacer {
            height: 40%;
        }
        #results_div {
            display: flex;
            flex-direction: column;
            justify-content: space-evenly;
            height: 80%;
        }
        .botones {
            font-size: 7vmin;
            background-color: rgb(240, 225, 192);
            color: #000000;
            border: 0.5vw solid #AD9980;
            border-radius: 4vw;
            height: 20%;
            min-width: 1vW; 
        }
        h4 {
            border-bottom: 1vw solid #AD9980;
        }
    </style>
  </head>
  
  <body>
    <h1>Pasillo Rehabilitador</h1>
        <div id="botonera">
            <button class="botones" id="fase1">Fase 1</button>
            <button class="botones" id="fase2">Fase 2</button>
            <button class="botones" id="fase3">Fase 3</button>
        </div>
        <div class="spacer"></div>
        <h4>Parámetros medidos</h4>
        <div id="results_div">

            <label class="etiquetas">Longitud de paso: </label>
            <label class="resultados">-- m</label>
            <label class="etiquetas">Longitud de zancada: </label>
            <label class="resultados">-- m</label>
        </div>
  </body>
<script>

document.getElementById("fase1").addEventListener("click", function () {
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "/down", true);
            xhttp.onreadystatechange = function () {
                if (xhttp.readyState == XMLHttpRequest.DONE) {
                    if (xhttp.status != 200) {
                        // Error response
                        console.error("Error");
                    }
                }
            };
            //xhttp.send();
        });

 document.getElementById("fase2").addEventListener("click", function () {
            // Post request to dispense endpoint
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "/up", true);
            xhttp.onreadystatechange = function () {
                if (xhttp.readyState == XMLHttpRequest.DONE) {
                    if (xhttp.status != 200) {
                        console.error("Error");
                    }
                }
            };
        });
        
  document.getElementById("fase3").addEventListener("click", function () {
            // Post request to dispense endpoint
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "/stop", true);
            xhttp.onreadystatechange = function () {
                if (xhttp.readyState == XMLHttpRequest.DONE) {
                    if (xhttp.status != 200) {
                        console.error("Error");
                    }
                }
            };
            xhttp.send("amount=1"); // Enviar el número 1 como dato
        });
        
  document.getElementById("scriptButton").addEventListener("click", function () {
            // Post request to dispense endpoint
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "/script", true);
            xhttp.onreadystatechange = function () {
                if (xhttp.readyState == XMLHttpRequest.DONE) {
                    if (xhttp.status != 200) {
                        console.error("Error");
                    }
                }
            };
            xhttp.send("amount=1"); // Enviar el número 1 como dato
        });
</script>
</html>)rawliteral";



void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ssid, password);                  //para crear un punto de acceso
  IPAddress IP = WiFi.softAPIP();               //...
  Serial.print("AP IP address: ");              //... 
  Serial.println(IP);                           //...
  Serial.println(WiFi.localIP());               //...

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



  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
    Serial.println("main page sent!");
  });

  server.on("/fase1", HTTP_GET, [](AsyncWebServerRequest *request){       //funcion al pulsar el boton fase1
    request->send_P(200, "text/plain", "Hola");
    
  });
  
  server.on("/fase2", HTTP_GET, [](AsyncWebServerRequest *request){       //funcion al pulsar el boton fase2
    //request->send_P(200, "text/plain", "Hola");
    
  });



  server.on("/fase3", HTTP_GET, [](AsyncWebServerRequest *request){       //funcion al pulsar el boton fase3
    request->send_P(200, "text/plain", "Hola");

  });

  server.on("/middle", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", "Hola");
    
  });


    
  // Start server
  server.begin();
}
 
void loop(){  

}