// SerialPort.h
#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <thread>
#include <mutex>
#include <QString>
#include <QObject>

#include <obs-module.h>
#include "dak-source-support.h"
#include "serial/serial.hpp"

class SerialPort : public QObject {
	Q_OBJECT

public:
	// Callback type for line received signal (runs in main thread)
	using LineReceivedCallback = std::function<void(std::string line)>;
	using ErrorCallback = std::function<void(std::string error)>;

	std::string _port;

	SerialPort();
	~SerialPort();

	// Open/close port
	bool open(const std::string &portName, int baudRate = 19200);
	void close();
	bool isOpen() const { return opened; }

	// Factory method
	static std::unique_ptr<SerialPort> create();

	std::string getPort() const { return _port; };

	// Start/stop reading
	bool startReading();
	void stopReading();
	bool isReading() const { return reading; }

	// Set callbacks (will be called from main thread)
	void setLineReceivedCallback(LineReceivedCallback callback);
	void setErrorCallback(ErrorCallback callback);

	// Configuration
	bool setBaudRate(int baudRate);

	// Utility
	void flush();

	// Static utility to list available ports
	static std::vector<std::string> listPorts();

	// Qt signals emitted from the reader thread (delivered to this object's thread)
Q_SIGNALS:
	void lineReceived(const QString &line);
	void errorOccurred(const QString &error);

private Q_SLOTS:
	// Internal slot handlers run in the object's thread and will invoke callbacks
	void onLineReceivedSlot(const QString &line);
	void onErrorSlot(const QString &error);


signals:
	// Signal emitted when a new log message is available
	void setConnected(const bool isConnected);

protected:
	bool opened;
	std::atomic<bool> reading;
	int currentBaudRate;
	int readTimeout;

	serial::Serial *portObj;

	// Thread management
	std::unique_ptr<std::thread> readThread;


	// Callbacks (called from main thread)
	LineReceivedCallback lineCallback;
	ErrorCallback errorCallback;
	std::mutex callbackMutex;

	// Reader thread function
	void readThreadFunction();

	// Emit signals from background thread (thread-safe)
	void emitLineReceived(std::string line);
	void emitError(std::string error);
};

#endif // SERIALPORT_H
