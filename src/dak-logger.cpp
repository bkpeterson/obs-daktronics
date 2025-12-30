#include "dak-logger.hpp"

DAKLogger& DAKLogger::instance()
{
    static DAKLogger instance;
    return instance;
}

DAKLogger::DAKLogger(QObject *parent) : QObject(parent) {}

DAKLogger::~DAKLogger() {}
