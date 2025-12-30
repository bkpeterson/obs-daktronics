#ifndef DAKLOGGER_H
#define DAKLOGGER_H

#include <QObject>
#include <QString>

class DAKLogger : public QObject
{
    Q_OBJECT

public:
    static DAKLogger& instance();

signals:
    // Signal emitted when a new log message is available
    void logMessage(const QString &message);

private:
    DAKLogger(QObject *parent = nullptr);
    ~DAKLogger();
};

#endif // DAKLOGGER_H
