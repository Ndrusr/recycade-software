#include<Arduino.h>
#include<TeensyThreads.h>

Threads threads;

volatile bool state{false};


void run(){
    state = !state;
    delay(500);
}

void setup(){
    threads.addThread(run);
    pinMode(13, OUTPUT);
}

void loop(){
    digitalWrite(13,  state);
    delay(500);
}