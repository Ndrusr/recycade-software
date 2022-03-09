#include"StateMachine.h"

namespace StateMachine{
    RecSM::RecSM(){
        currentState = 0x0;
        allStates=0x0, 0x1, 0x2;
    }

    unsigned char const* RecSM::debugState(){
        if(debug){
            return allStates;
        }else{
            return nullptr;
        }
    }

}