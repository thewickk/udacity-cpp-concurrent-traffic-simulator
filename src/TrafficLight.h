#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <thread>
#include <future>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;
// TASK FP.1
enum TrafficLightPhase {
    red,
    green
};


// TASK FP.3
template <class T>
class MessageQueue
{
public:
    MessageQueue() {}

    T receive();

    void send(T &&message);

private:
    std::mutex _mutex;
    std::condition_variable _conditionVariable;
    std::deque<T> _messages;
};


// TASK FP.1
class TrafficLight : TrafficObject
{
public:
    // constructor / desctructor
    TrafficLight();
    // ~TrafficLight();
    
    // getters / setters

    // typical behaviour methods
   // TASK FP.1
    void waitForGreen();
    void simulate();
    TrafficLightPhase getCurrentPhase();
    
private:
    // typical behaviour methods
    // TASK FP.1
    void cycleThroughPhases();
    TrafficLightPhase _currentPhase;

    std::condition_variable _condition;
    std::mutex _mutex;

    // TASK FP.4b
    MessageQueue<TrafficLightPhase> _lightPhase;
};

#endif