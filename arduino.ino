#include <Arduino.h>

int vector[4] = {3,4,5,6};
int i;
int vector2[4];



void setup(){
    Serial.begin(9600);
    for (i=0; i<4 ; i++) {
        pinMode(vector[i], INPUT_PULLUP);
    }
}


void loop(){

    
    for (i = 0; i<4; i++){
        vector2[i] = digitalRead(vector[i]);
    }

    //Serial.print("Vector: ");
    for (i = 0; i<4; i++){
        Serial.print(vector2[i]);
        Serial.print(", ");
    }
    Serial.print("\n");
    
    

}



