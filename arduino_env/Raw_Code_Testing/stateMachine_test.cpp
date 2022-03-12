#include"StateMachine.h"
#include<iostream>

int main(){
    StateMachine::RecSM TestMachine;

    std::cout<< TestMachine.debugState() <<std::endl;
    

    return 0;
}