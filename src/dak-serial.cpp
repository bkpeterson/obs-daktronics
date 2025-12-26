#include "dak-serial.hpp"

SerialPort::SerialPort()
	: opened(false),
	  reading(false),
	  currentBaudRate(19200),
	  readTimeout(1000),
	  lineCallback(nullptr),
	  errorCallback(nullptr),
	  _port("")
{
	obs_log(LOG_INFO, "Creating serial port...");
	portObj = new serial::Serial();
}

SerialPort::~SerialPort()
{
	stopReading();
	close();
}

std::unique_ptr<SerialPort> SerialPort::create()
{
	return std::make_unique<SerialPort>();
}

bool SerialPort::open(const std::string &portName, int baudRate)
{
	obs_log(LOG_INFO, "Calling serial port open...");
	if (opened) {
		close();
	}

	currentBaudRate = baudRate;

	portObj->setPort(portName);
	portObj->setBaudrate(baudRate);
	portObj->open();

	if (portObj->isOpen()) {
		obs_log(LOG_INFO, "Port opened!!!");
		opened = true;
		_port = portName;
		return true;
	}
	obs_log(LOG_INFO, "Port NOT open.");
	return false;
}

void SerialPort::close()
{
	if (!opened)
		return;

	stopReading();
	portObj->close();
	opened = false;
	_port = "";
}

bool SerialPort::startReading()
{
	if (!opened || reading) {
		return false;
	}

	reading = true;
	readThread = std::make_unique<std::thread>(&SerialPort::readThreadFunction, this);
	return true;
}

void SerialPort::stopReading()
{
	if (!reading)
		return;

	reading = false;

	// Wake up any waiting threads
	queueCondition.notify_all();

	if (readThread && readThread->joinable()) {
		readThread->join();
	}
	readThread.reset();
}

void SerialPort::setLineReceivedCallback(LineReceivedCallback callback)
{
	std::lock_guard<std::mutex> lock(callbackMutex);
	lineCallback = callback;
}

void SerialPort::setErrorCallback(ErrorCallback callback)
{
	std::lock_guard<std::mutex> lock(callbackMutex);
	errorCallback = callback;
}

bool SerialPort::setBaudRate(int baudRate)
{
	if (!opened)
		return false;

	portObj->setBaudrate(baudRate);
	currentBaudRate = baudRate;
	return true;
}

void SerialPort::flush()
{
	if (!opened)
		return;
	portObj->flush();
}

std::vector<std::string> SerialPort::listPorts()
{
	return serial::list_ports();
}

bool SerialPort::hasPendingSignals() const
{
	std::lock_guard<std::mutex> lock(queueMutex);
	return !signalQueue.empty();
}

void SerialPort::processSignals()
{
	std::vector<Signal> signalsToProcess;

	// Extract all pending signals
	{
		std::lock_guard<std::mutex> lock(queueMutex);
		while (!signalQueue.empty()) {
			signalsToProcess.push_back(signalQueue.front());
			signalQueue.pop();
		}
	}

	// Process signals outside the lock
	std::lock_guard<std::mutex> callbackLock(callbackMutex);

	for (const auto &signal : signalsToProcess) {
		switch (signal.type) {
		case 0:
			if (lineCallback) {
				lineCallback(signal.data);
			}
			break;

		case 1:
			if (errorCallback) {
				errorCallback(signal.data);
			}
			break;
		}
	}
}

void SerialPort::readThreadFunction()
{
	std::string lineBuffer;
	std::string readBuffer;
	bool readingHeader = true;
	obs_log(LOG_INFO, "Reading header");

	while (reading && opened) {
		int bytesRead = static_cast<int>(portObj->readline(readBuffer, 65536, {0x11}));

		if (bytesRead > 0) {
			const char *buf = readBuffer.c_str();

			// Process each character
			for (int i = 0; i < bytesRead; i++) {
				char c = buf[i];

				if (readingHeader && c != 0x10) {
					continue;
				} else {
					if (readingHeader && c == 0x10) {
						readingHeader = false;
						obs_log(LOG_INFO, "Done reading header.");
						continue;
					}
				}

				lineBuffer += c;

				// Prevent buffer overflow
				if (lineBuffer.size() > 4096) {
					emitError("Line buffer overflow - line too long");
					readingHeader = true;
					obs_log(LOG_INFO, "Reading header");
					lineBuffer.clear();
				}
			}

			// Complete line received - emit signal
			if (!lineBuffer.empty()) {
				obs_log(LOG_INFO, "Read line: %s", lineBuffer.c_str());
				emitLineReceived(lineBuffer);
				readingHeader = true;
				obs_log(LOG_INFO, "Reading header");
				lineBuffer.clear();
			}

		} else if (bytesRead < 0) {
			emitError("Read error occurred");
			// Small delay to prevent tight loop on persistent errors
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		// bytesRead == 0 means timeout, which is normal
	}
}

void SerialPort::emitLineReceived(const std::string &line)
{
	std::lock_guard<std::mutex> lock(queueMutex);
	Signal signal;
	signal.type = 0;
	signal.data = line;
	signalQueue.push(signal);
	queueCondition.notify_one();
}

void SerialPort::emitError(const std::string &error)
{
	std::lock_guard<std::mutex> lock(queueMutex);
	Signal signal;
	signal.type = 1;
	signal.data = error;
	signalQueue.push(signal);
	queueCondition.notify_one();
}
