#ifndef OBS_SERIAL_PLUGIN_HPP
#define OBS_SERIAL_PLUGIN_HPP

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QStringList>
#include <QIODevice>

// Define a default baud rate
#define DEFAULT_BAUD_RATE 19200

// ====================================================================
// SerialPortWorker (Runs in separate QThread)
// ====================================================================

/**
 * @brief Worker class to manage QSerialPort in a separate thread.
 * This class uses Qt signals and slots to handle asynchronous serial communication.
 */
class SerialPortWorker : public QObject {
    Q_OBJECT

public:
    QSerialPort *serialPort = nullptr;
    QString portName;
    int baudRate = DEFAULT_BAUD_RATE;

    explicit SerialPortWorker(QObject *parent = nullptr);
    ~SerialPortWorker() override; // Add virtual destructor

signals:
    // Signal emitted when a complete line of data is received
    void dataReceived(const QString& data);

public slots:
    /**
     * @brief Slot to initialize and connect the serial port.
     */
    void connectPort();

    /**
     * @brief Slot to disconnect and close the serial port.
     */
    void disconnectPort();

private slots:
    /**
     * @brief Slot triggered by QSerialPort::readyRead() signal.
     */
    void readData();
};


#endif // OBS_SERIAL_PLUGIN_HPP
