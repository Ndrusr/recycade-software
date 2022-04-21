class StateMachine{
public:
  StateMachine(){
    state = 0;
  }
  void switchStates(byte zCommand){
    switch(zCommand){
      case (byte)'A':
        state = 1;
        break;
      case (byte)'G':
        state = 2;
        break;
      case (byte)'I':
        state = 0;
        break;
    }
  }
  int getSMState(){
    return state;
  }
  
private:
  int state;

};
