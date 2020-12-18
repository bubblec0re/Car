#include <Arduino.h>

class Timer
{
public:
    Timer(uint64_t periodms)
    {
        _period = periodms;
        _checkTime = millis();
    }

    bool itsTime()
    {
        uint64_t now = millis();
        if (now - _checkTime > _period)
        {
            _checkTime = millis();
            return true;
        }
        return false;
    }

    void restart()
    {
        _checkTime = millis();
    }

private:
    uint64_t _period, _checkTime;
};