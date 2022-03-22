#include<ros/ros.h>
#include<std_msgs/Char.h>

#include<recycade_main/StateMachine.h>

namespace recycade_main{

class Recycade: public StateMachine::RecSM{

public:
    Recycade();
    ~Recycade();

protected:
    ros::NodeHandle nh;

    std_msgs::Char stateMsg;
    ros::Publisher statePub;
    
    ros::Subscriber<std_msgs::Char> feedbackSub;

private:
    void rising(){
        currentState = '1';
        stateMsg.data = currentState;
        
    }

    void falling(){

    }

    void game(){

    }

    void gameover(){

    }

    std::map<const char*, StateMachine::State> recycadeStateMap{
        {"0a", StateMachine::State()}, // a -> recieved input
        {"1b", StateMachine::State()}, // b -> passed inspection
        {"1c", StateMachine::State()}, // c -> failed inspection 
        {"2d", StateMachine::State()}  // d -> game over
    }

};

}