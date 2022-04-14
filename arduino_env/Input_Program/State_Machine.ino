class Input_State_Machine
{

  public:

  String state;
  String ns;
  String output;
  String start_state = "idle";

  void startMachine(){
    state = start_state;
    return;
  }

  void get_next_values(String state, String inp){
    if (state=="idle" && inp=="open"){
      
    }
    return;
  }
  
  String step_function(String inp){
    get_next_values(state,inp);
    state = ns;
    return output;
  }
};
