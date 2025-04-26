#include <Arduino.h>

int vector[4] = {3,4,5,6};
int i;
int vector2[4];

long previous_time = 0;
const int interval_time = 300;
boolean busy = false;



void setup(){
    Serial.begin(9600);
    for (i=0; i<4 ; i++) {
        pinMode(vector[i], INPUT_PULLUP);
    }
}


void loop(){
    if (millis() - previous_time > interval_time){
        previous_time = millis();
        if (busy){
            for (i = 0; i<4; i++){
            vector2[i] = digitalRead(vector[i]);
            }

            for (i = 0; i<4; i++){
                Serial.print(vector2[i]);
                Serial.print(", ");
            }
            Serial.print("\n");
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



