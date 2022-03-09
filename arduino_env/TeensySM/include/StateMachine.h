namespace StateMachine{
class RecSM{

protected:
    unsigned char currentState;
    unsigned char const* allStates;
    unsigned const char maxState;

public:
    RecSM();
    ~RecSM();
    void setState(unsigned const char event);

};

}