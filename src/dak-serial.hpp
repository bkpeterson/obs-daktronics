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

class SerialPort {
public:
	// Callback type for line received signal (runs in main thread)
	using LineReceivedCallback = std::function<void(const std::string &line)>;
	using ErrorCallback = std::function<void(const std::string &error)>;

	std::string _port;

	SerialPort();
	virtual ~SerialPort();

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
	bool setTimeout(int milliseconds);

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

	// Platform-specific abstract methods
	virtual bool platformOpen(const std::string &portName) = 0;
	virtual void platformClose() = 0;
	virtual int platformRead(char *buffer, int size) = 0;
	virtual void platformFlush() = 0;
	virtual bool platformSetBaudRate(int baudRate) = 0;
	virtual bool platformSetTimeout(int milliseconds) = 0;

	// Platform-specific port listing
	static std::vector<std::string> platformListPorts();

	// Emit signals from background thread (thread-safe)
	void emitLineReceived(const std::string &line);
	void emitError(const std::string &error);
};

// Platform-specific implementations

#ifdef _WIN32
#include <windows.h>

class WindowsSerialPort : public SerialPort {
public:
	WindowsSerialPort();
	~WindowsSerialPort() override;

protected:
	bool platformOpen(const std::string &portName) override;
	void platformClose() override;
	int platformRead(char *buffer, int size) override;
	void platformFlush() override;
	bool platformSetBaudRate(int baudRate) override;
	bool platformSetTimeout(int milliseconds) override;

private:
	HANDLE hSerial;
	DCB dcb;
	COMMTIMEOUTS timeouts;

	bool applyDCB();
};

#else
#include <termios.h>

class PosixSerialPort : public SerialPort {
public:
	PosixSerialPort();
	~PosixSerialPort() override;

protected:
	bool platformOpen(const std::string &portName) override;
	void platformClose() override;
	int platformRead(char *buffer, int size) override;
	void platformFlush() override;
	bool platformSetBaudRate(int baudRate) override;
	bool platformSetTimeout(int milliseconds) override;

private:
	int fd;
	struct termios tty;
	struct termios tty_old;

	bool applyTermios();
	speed_t baudRateToSpeed(int baudRate);
};

#endif

#endif // SERIALPORT_H
