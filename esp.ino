// Import required libraries
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ezButton.h>


//declaraciones de los pines que serán usados para las huellas
const int pin1_0 = 36;
const int pin1_1 = 39;
const int pin1_2 = 34;
const int pin1_3 = 35;
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
            background: linear-gradient(to bottom, #f3e1e3, #B98389);
            color: #54494B;
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
            height: auto;
            display: flex;
            flex-direction: column;
            justify-content: space-evenly;
            width: 75%;
            gap: 1vh;
        }

        .botones {
            font-size: 5vmin;
            background-color: rgb(240, 225, 192);
            color: #54494B;
            border: 0.5vw solid #F0EBD8;
            border-radius: 4vw;
            height: 8vh;
            min-width: 1vW;
        }

        #boton-secundario {
            font-size: 3.5vmin;
            background-color: rgb(240, 225, 192);
            color: #54494B;
            border: 0.3vw solid #F0EBD8;
            border-radius: 3vw;
            padding: 1vh 2vw;
            margin-top: 4vh;
            margin-bottom: 4vh;
        }

        h4 {
            border-bottom: 1vw solid #584d3f;
            align-self: flex-start;
            margin-left: 10%;
            margin-top: 4vh;
            text-align: left;
        }

        #results_div {
            display: flex;
            flex-direction: column;
            justify-content: flex-start;
            align-self: flex-start;
            margin-left: 10%;
            width: 80%;
            gap: 1vh;
            text-align: left;
        }

        .result_row {
            display: flex;
            justify-content: space-between;
            align-items: center;
            width: 100%;
        }

        .etiquetas {
            font-weight: bold;
        }
    </style>
  </head>
  
  <body>

    <h1>Pasillo Rehabilitador</h1>
    <h4>Elija la fase deseada:</h4>

    <div id="botonera">
        <button class="botones" id="fase1_b">Fase 1</button>
        <button class="botones" id="fase2_b">Fase 2</button>
        <button class="botones" id="fase3_b">Fase 3</button>
    </div>

    <h4>Parámetros medidos:</h4>
    <div id="results_div">
        <div class="result_row">
            <label class="etiquetas">Longitud de paso:</label>
            <label class="resultados" id="long_paso_res">-- cm</label>
        </div>
        <div class="result_row">
            <label class="etiquetas">Longitud de zancada:</label>
            <label class="resultados" id="long_zan_res">-- cm</label>
        </div>
        <div class="result_row">
            <label class="etiquetas">Tiempo de apoyo:</label>
            <label class="resultados" id="tiempo_res">-- s</label>
        </div>
        <div class="result_row">
            <label class="etiquetas">Velocidad de la marcha:</label>
            <label class="resultados" id="vel_res">-- cm/s</label>
        </div>
    </div>

    <button id="boton-secundario">Actualizar parámetros obtenidos</button>

</body>
<script>

    document.getElementById("fase1_b").addEventListener("click", function () {
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "/fase1", true);
            xhttp.onreadystatechange = function () {
                if (xhttp.readyState == XMLHttpRequest.DONE) {
                    if (xhttp.status != 200) {
                        // Error response
                        console.error("Error");
                    }
                }
            };
            xhttp.send();
        });

    document.getElementById("fase2_b").addEventListener("click", function () {
            // Post request to dispense endpoint
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "/fase2", true);
            xhttp.onreadystatechange = function () {
                if (xhttp.readyState == XMLHttpRequest.DONE) {
                    if (xhttp.status != 200) {
                        console.error("Error");
                    }
                }
            };
            xhttp.send();
        });
        
    document.getElementById("fase3_b").addEventListener("click", function () {
            // Post request to dispense endpoint
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "/fase3", true);
            xhttp.onreadystatechange = function () {
                if (xhttp.readyState == XMLHttpRequest.DONE) {
                    if (xhttp.status != 200) {
                        console.error("Error");
                    }
                }
            };
            xhttp.send(); 
        });

    document.getElementById("boton-secundario").addEventListener("click", function () {
            // Post request to dispense endpoint
            var xhttp = new XMLHttpRequest();
            xhttp.open("GET", "/results", true);
            xhttp.onreadystatechange = function () {
                if (xhttp.readyState == XMLHttpRequest.DONE) {
                    if (xhttp.status != 200) {
                        console.error("Error");
                    }
                }
            };
            xhttp.send();
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