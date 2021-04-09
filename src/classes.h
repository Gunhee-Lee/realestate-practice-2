/* This file is temporary class files. After some development, each class will be 
   moved to its own header file.
*/

class IPrintable
{
    public:
    virtual void print() = 0;
};

class State: public virtual IPrintable
{
    DateTime today;     // Use QuantLib
    std::vector<Asset> currentAssets_;  // Assets include cash and debts.
};

class Action
{

};

class SimulationRun
{
    State initialState_;
    std::vector<Action> actions_;
};

class WaitAction: public Action
{

};

class Environment
{
};

// This class will be implemented as a json file.
class Asset: public virtual IPrintable
{
    std::u16string name;
    Currency getPrice(Time t);  // QuantLib should replace this line soon.
};

class Person: public virtual IPrintable
{
    State currentState_;

};

// Control class.
class FinancialSimulator
{
    private:
    State initialState_;
    State desiredState_;
    Environment env_;
    SimulationRun run_(initialState);

    public:
    void makeSimulationRun();
};
