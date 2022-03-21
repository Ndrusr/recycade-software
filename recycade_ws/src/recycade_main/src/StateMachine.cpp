#include<recycade_main/StateMachine.h>

namespace StateMachine{
    RecSM::RecSM(){
        debug = true;
    }

    RecSM::~RecSM(){
        delete[] allStates;
    }

    const char* RecSM::debugState(){
        if(debug){
            return allStates;
        }else{
            return nullptr;
        }
    }

    void RecSM::setState(const char* event){
        const char* mapIn {currentState+event};
    }
    State::State(){
        func = [](){};
    }
    State::State(void(*func_in)()){
        func = func_in;
    }
    void State::run(){
        func();
    }
}