namespace StateMachine{
class RecSM{

protected:
    char currentState{'0'};
    const char* allStates{"012"};
    bool debug;

public:
    RecSM();
    ~RecSM();
    void setState(const char event);
    const char* debugState();
};

}