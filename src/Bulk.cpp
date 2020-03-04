#include "Bulk.h"

#include <iostream>

/// State

State::State(Bulk* bulk): _bulk(bulk) {}

void State::complete()
{
    _bulk->complete();
    clear();
}

void State::dispatch()
{
    if (!_data.empty())
    {
        _bulk->dispatch(_data);
    }
    clear();
}

void State::clear()
{
    _data.clear();
}

void State::setState(StateType type)
{
    _bulk->setState(type);
}

/// Simple State
SimpleState::SimpleState(Bulk* bulk): State(bulk)
{
    _data.reserve(_bulk->getSize());
}

void SimpleState::execute(std::string line)
{
    if (line.empty())
    {
        dispatch();
        complete();
        return;
    }

    if (line == "{")
    {
        dispatch();
        setState(StateType::Group);
        return;
    }

    if (line == "}")
    {
        return;
    }

    _data.emplace_back(line);

    if (_data.size() >= _bulk->getSize())
    {
        dispatch();
    }
}

/// Group State

GroupState::GroupState(Bulk* bulk): State(bulk) {}

void GroupState::execute(std::string line)
{
    if (line.empty())
    {
        complete();
        return;
    }

    if (line == "{")
    {
        ++ _depth;
        return;
    }

    if (line == "}")
    {
        if (_depth > 0)
        {
            -- _depth;
            return;
        }

        dispatch();
        setState(StateType::Simple);
        return;
    }

    _data.emplace_back(line);
}

void GroupState::clear()
{
    State::clear();
    _depth = 0;
}

/// Bulk

Bulk::Bulk(size_t size): _size(size)
{
    _states.emplace(StateType::Simple, new SimpleState(this));
    _states.emplace(StateType::Group, new GroupState(this));

    setState(StateType::Simple);
}

void Bulk::setState(StateType type)
{
    _sm = _states[type];
}

void Bulk::run()
{
    std::string line;

    while (!_isComplete)
    {
        std::getline(std::cin, line);
        _sm->execute(std::move(line));
    }
}

size_t Bulk::getSize() const
{
    return _size;
}

void Bulk::complete()
{
    _isComplete = true;
    unsubscribeAll();
}

/// event dispatcher

void Bulk::dispatch(const Data& data)
{
    for (const auto& listener : _listeners)
    {
        listener->update(data);
    }
}

void Bulk::subscribe(IObserver* listener)
{
    _listeners.push_back(listener);
}

void Bulk::unsubscribeAll()
{
    _listeners.clear();
}

/// Subscribers

void LogWriter::update(const Data &data)
{
    std::cout << "bulk:";
    for (const auto& chunk : data)
    {
        std::cout << ' ' << chunk;
    }
    std::cout << std::endl;
}

void FileWriter::update(const Data &data)
{

}
