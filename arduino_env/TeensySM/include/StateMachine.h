namespace StateMachine{
class RecSM{

protected:
    unsigned char currentState;
    unsigned char const* allStates;
    unsigned const char maxState{0x2};
    bool debug;

public:
    RecSM();
    ~RecSM();
    void setState(unsigned const char event);
    unsigned char const* debugState();
};

}