#pragma once

#include <string>
#include <vector>
#include <map>
#include <list>

class Bulk;

using Data = std::vector<std::string>;

/// State Type

enum class StateType {Simple, Group};

///  State

class State
{
public:
    using Ptr = std::shared_ptr<State>;

public:
    explicit State(Bulk* bulk);
    virtual ~State() = default;

    virtual void execute(std::string line) = 0;
    void complete();
    void dispatch();
    virtual void clear();

    void setState(StateType type);

protected:
    Bulk* _bulk;
    Data _data;
};

/// Simple State

class SimpleState: public State
{
public:
    explicit SimpleState(Bulk* bulk);

    void execute(std::string line) override;
};

/// Group State

class GroupState: public State
{
public:
    explicit GroupState(Bulk* bulk);

    void execute(std::string line) override;
    void clear() override;

private:
    size_t _depth;
};

/// Observer

class IObserver
{
public:
    virtual ~IObserver() = default;
    virtual void update(const Data& data) = 0;
};

/// Bulk

class Bulk
{
public:
    explicit Bulk(size_t size);

    size_t getSize() const;
    void setState(StateType type);
    void run();
    void dispatch(const Data& data);
    void complete();

    void subscribe(IObserver* obj);
    void unsubscribeAll();

private:
    size_t _size;
    std::map<StateType, State::Ptr> _states;
    State::Ptr _sm;
    bool _isComplete;

    std::list<IObserver*> _listeners;
};

/// Writers

class LogWriter : public IObserver
{
public:
    void update(const Data &data) override;
};

class FileWriter : public IObserver
{
public:
    void update(const Data &data) override;
};
