#pragma once
class TimeStamp {
public:
    short year;
    short month;
    short day;
    short hour;
    short minute;
    short second;

    bool operator!=(const TimeStamp &other) {
        return year != other.year || month != other.month || day != other.day || 
            hour != other.hour || minute != other.minute || second != other.second;
    }

};

