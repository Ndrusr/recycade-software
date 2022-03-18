#include<map>
namespace StateMachine{
class RecSM{

protected:
    char currentState{'0'};
    const char* allStates{"012"};
    bool debug; 

    class State{
    protected:
        void (*func)();

    public:
        char stateNum;
        State();
        State(void(*func_in)());
        void run();
    };
    std::map<const char*, State> stateMap;

public:
    RecSM();
    ~RecSM();
    void setState(const char* event);
    const char* debugState();
};

}