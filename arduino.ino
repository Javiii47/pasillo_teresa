#include <Arduino.h>

#define array_size 4

int vector_pines[array_size] = {3,4,5,6};          //vector con los pines en orden de botones
int i;
int vector[array_size] = {0, 0, 0, 0};                     //vector a la salida con el tamaño igual al otro
boolean change=0;


long previous_time = 0;
const int interval_time = 300;
boolean busy = false;



void setup(){
    Serial.begin(9600);
    for (i=0; i<array_size ; i++) {
        pinMode(vector_pines[i], INPUT_PULLUP);
    }
}


void loop(){
    if (millis() - previous_time > interval_time){
        previous_time = millis();
        if (busy){
            for (i = 0; i<array_size; i++){
                if (vector[i] != digitalRead(vector_pines[i])){
                    vector[i] = digitalRead(vector_pines[i]);
                    change = true;
                }
            }
            if (change){
                for (i = 0; i<array_size; i++){
                    Serial.print(vector[i]);
                }
                Serial.print("\n");
                change = false;
            }
            
        }

        if (Serial.available()) {
            // Read data and display it
            String message = Serial.readStringUntil('\n');
            //Serial.println("Received: " + message);
            if(message == "1"){
                busy = true;   
            }
            else if (message == "0"){
                busy = false;
            }  
        }
    }
}



