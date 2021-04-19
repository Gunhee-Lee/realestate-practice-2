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
    DateTime today_;     // [TODO] Use QuantLib
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

class BuyAssetAction: public Action
{

};

class SellAssetAction: public Action
{

};

class Environment
{
};

// This class will be implemented as a json file.
class Asset: public virtual IPrintable
{
    std::u16string name_;
    Currency getPrice(Time t);  // QuantLib should replace this line soon.

    public:
    virtual void print();
};

class CashAsset: public Asset
{
    std::u16string name_;       // KRW, USD, JPY, ......
    std::double amount_;        // [TODO] Use QuantLib to have a financial standard of digit resolution. (double is not enough)

    public:
    virtual void print();
};

class HouseAsset: public Asset
{

};

class CarAsset: public Asset
{
    // I don't think a car is an asset, but to demonstrate the depreciating value
    // simulation capabilities, it is required to include this.
};

// Multi-people simulation will be developed later.
class Person: public virtual IPrintable
{
    State currentState_;
    std::u16string name_;

    public:
    virtual void print();
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
