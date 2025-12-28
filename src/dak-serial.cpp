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
	//std::string lineBuffer;
	std::string readBuffer;
	std::string readChar;
	//bool readingHeader = true;
	//obs_log(LOG_INFO, "Reading header");
	/*

			header, unprocessed = message.split(chr(2))
            text, checksum = unprocessed.split(chr(4))

            start = int(header[-4] + header[-3] + header[-2] + header[-1]) + 1
            end = start + len(text) - 1
            self.process(text, (start, end))


        self.header = self.rtd.partition(b'\x16')[2].partition(b'\x01')[0]
        self.code = self.rtd.partition(b'\x01')[2].partition(b'\x02')[0].partition(b'\x04')[0]
        self.text = self.rtd.partition(b'\x02')[2].partition(b'\x04')[0]
        self.checksum = self.rtd.partition(b'\x04')[2].partition(b'\x17')[0]

        code = self.code.decode()
        code = code[-4:]
        text = self.text.decode()
        self.dakString = self.dakString[:int(code)] + text + self.dakString[int(code) + len(text):]



		code = rtd.partition(b'\x01')[2].partition(b'\x02')[0].partition(b'\x04')[0]
        text = rtd.partition(b'\x02')[2].partition(b'\x04')[0]

        code = code.decode()
        code = code[-4:]
        text = text.decode()

        doLog('Code: ')
        doLog(code)
        doLog('Text: ')
        doLog(text)

        dakString = dakString[:int(code)] + text + dakString[int(code) + len(text):]



*/
	while (reading && opened) {
		readChar = "";
		while (readChar != "\x16") {
			readChar = portObj->read(1);
		}

		readBuffer.clear();
		while (readChar != "\x17") {
			readChar = portObj->read(1);
			if (readChar != "\x17") {
				readBuffer += readChar;
			}
		}

		//Read until a start transmission character encountered
		//portObj->readline(readBuffer, 65536, {0x16});

		//Read until end of transmission
		//readBuffer.clear();
		//int bytesRead = static_cast<int>(portObj->readline(readBuffer, 65536, {0x17}));

		if (readBuffer.length() > 0) {
			//if (!readBuffer.empty()) {
			emitLineReceived(readBuffer);
			//}
			//} else if (bytesRead < 0) {
			//std::string errEmitted = "Read error occurred";
			//emitError(errEmitted);
			// Small delay to prevent tight loop on persistent errors
			//std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}
}

void SerialPort::emitLineReceived(std::string &line)
{
	std::lock_guard<std::mutex> lock(queueMutex);
	Signal signal;
	signal.type = 0;
	signal.data = line;
	signalQueue.push(signal);
	queueCondition.notify_one();
}

void SerialPort::emitError(std::string &error)
{
	std::lock_guard<std::mutex> lock(queueMutex);
	Signal signal;
	signal.type = 1;
	signal.data = error;
	signalQueue.push(signal);
	queueCondition.notify_one();
}
