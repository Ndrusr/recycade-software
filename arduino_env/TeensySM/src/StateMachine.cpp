#include"StateMachine.h"

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

    

}