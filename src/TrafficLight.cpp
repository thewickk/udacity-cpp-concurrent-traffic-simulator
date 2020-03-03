#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive()
{

    // TASK FP.5a
    std::unique_lock<std::mutex> uLock(_mutex);
    _conditionVariable.wait(uLock, [this] { return !_messages.empty(); } );

    T message = std::move(_messages.back());
    _messages.pop_back();

    return message; // no copy due to Return Value Optimization
}

template <typename T>
void MessageQueue<T>::send(T &&message)
{
    // TASK FP.4a
    std::lock_guard<std::mutex> lGuard(_mutex);

    _messages.push_back(std::move(message));
    _conditionVariable.notify_one();
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // TASK FP.5b
    while (true)
    {
        TrafficLightPhase trafficLight = _lightPhase.receive();
        if (trafficLight == TrafficLightPhase::green)
        {
          return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    std::lock_guard<std::mutex> lGuard(_mutex);
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // TASK FP.2b
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}


// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{

    // TASK FP.2a
    std::random_device random;  
    std::mt19937 generate(random()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> distribution(4.0, 6.0);
    double timeCycle = distribution(generate);

    std::chrono::time_point<std::chrono::system_clock> update = std::chrono::system_clock::now();
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        long timeFromUpdate = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - update).count();
        if (timeFromUpdate >= timeCycle)
        {

            {
                std::lock_guard<std::mutex> lck(_mutex);
                if (_currentPhase == TrafficLightPhase::red)
                {
                    _currentPhase = TrafficLightPhase::green;
                }
                else if (_currentPhase == TrafficLightPhase::green)
                {
                    _currentPhase = TrafficLightPhase::red;
                }
            }

            _lightPhase.send(std::move(getCurrentPhase()));

            update = std::chrono::system_clock::now();
            timeCycle = distribution(generate);
        }
    }
}

