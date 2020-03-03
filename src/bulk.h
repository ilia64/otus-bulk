#pragma once

/// STATE MACHINE

using Data = std::vector<std::string>;
using DataPtr = std::shared_ptr<Data>;

enum class StateType {Simple, Group}

class State
{
public:
    using Ptr = std::shared_ptr<State>;

public:
    explicit State(Bulk* bulk): _bulk(bulk){};
    virtual ~State() = 0;

    virtual void execute(std::string line) = 0;
    virtual void reset()
    {
        _data->clear();
    }

protected:
    Bulk* _bulk;
    DataPtr _data;
};


class SimpleState: public State
{
public:
    explicit SimpleState(Bulk* bulk): State(bulk)
    {
        _data->capacity(_bulk->getSize());
    };

    void execute(std::string line) override
    {
        if (line == "{")
        {
            _bulk->setState(StateType::Group);
            return;
        }

        _data->emplace_back(line);

        if (_data->size() >= _bulk->getSize())
        {
            _bulk->dispatch()
            reset();
        }
    }
};


class GroupState: public State
{
public:
    explicit GroupState(Bulk* bulk): State(bulk){};

    void execute(std::string line) override
    {
        if (line == "}")
        {
            _bulk->dispatch()

            reset();

            _bulk->setState(StateType::Simple);
            return;
        }

        _data->emplace_back(line);

        if (_data->size() >= _bulk->getSize())
        {

        }
    }
};


class Bulk
{
public:

    explicit Bulk(size_t size): _size(size)
    {
        _states.emplace(StateType::Simple, new SimpleState(this));
        _states.emplace(StateType::Group, new GroupState(this));

        setState(StateType::Simple);
    }

    void setState(StateType type)
    {
        _sm = _states[type];
    }

    void run()
    {
        for(std::string line; std::getline(std::cin, line);)
        {
            _sm.execute(std::move(line));
        }
    }

    size_t getSize()
    {
        return _size;
    }

    void dispatch(const DataPtr data)
    {
        std::cout << data.size() << std::endl;
    }

private:
    size_t _size;
    std::map<StateType, State::Ptr> _states;
    State::Ptr _sm;
};