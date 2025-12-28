// SerialPort.h
#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <queue>

#include <obs-module.h>
#include "dak-source-support.h"
#include "serial/serial.hpp"

class SerialPort {
public:
	// Callback type for line received signal (runs in main thread)
	using LineReceivedCallback = std::function<void(std::string &line)>;
	using ErrorCallback = std::function<void(std::string error)>;

	std::string _port;

	SerialPort();
	~SerialPort();

	// Open/close port
	bool open(const std::string &portName, int baudRate = 19200);
	void close();
	bool isOpen() const { return opened; }

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

	// Main thread must call this periodically to process signals
	void processSignals();

	// Check if there are pending signals to process
	bool hasPendingSignals() const;

	// Static utility to list available ports
	static std::vector<std::string> listPorts();

	// Factory method
	static std::unique_ptr<SerialPort> create();

protected:
	bool opened;
	std::atomic<bool> reading;
	int currentBaudRate;
	int readTimeout;

	serial::Serial *portObj;

	// Thread management
	std::unique_ptr<std::thread> readThread;

	// Signal queue for thread-safe communication
	struct Signal {
		int type;
		std::string data;
	};

	std::queue<Signal> signalQueue;
	mutable std::mutex queueMutex;
	std::condition_variable queueCondition;

	// Callbacks (called from main thread)
	LineReceivedCallback lineCallback;
	ErrorCallback errorCallback;
	std::mutex callbackMutex;

	// Reader thread function
	void readThreadFunction();

	// Emit signals from background thread (thread-safe)
	void emitLineReceived(std::string &line);
	void emitError(std::string &error);
};

#endif // SERIALPORT_H
