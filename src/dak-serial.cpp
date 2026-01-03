#include "dak-serial.hpp"
#include <QString>

std::atomic<bool> exit_flag(false);

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

	// Connect internal Qt signals to slots so callbacks are invoked in this object's thread
	connect(this, &SerialPort::lineReceived, this, &SerialPort::onLineReceivedSlot);
	connect(this, &SerialPort::errorOccurred, this, &SerialPort::onErrorSlot);

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
		setConnected(true);
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
	setConnected(false);
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

	exit_flag.store(true);
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


// Slot implementations: these run in the object's thread and call the stored callbacks
void SerialPort::onLineReceivedSlot(const QString &line)
{
	std::lock_guard<std::mutex> lock(callbackMutex);
	if (lineCallback) {
		lineCallback(line.toStdString());
	}
}

void SerialPort::onErrorSlot(const QString &error)
{
	std::lock_guard<std::mutex> lock(callbackMutex);
	if (errorCallback) {
		errorCallback(error.toStdString());
	}
}

void SerialPort::readThreadFunction()
{
	std::string readBuffer;
	std::string readChar;

	while (reading && opened && !exit_flag.load()) {
		readChar = "";
		while (readChar != "\x16" && !exit_flag.load()) {
			readChar = portObj->read(1);
		}

		readBuffer.clear();
		while (readChar != "\x17" && !exit_flag.load()) {
			readChar = portObj->read(1);
			if (readChar != "\x17") {
				readBuffer += readChar;
			}
		}

		if (readBuffer.length() > 0) {
			emitLineReceived(readBuffer);
		}
	}
}

void SerialPort::emitLineReceived(std::string line)
{
	emit lineReceived(QString::fromStdString(line));
}

void SerialPort::emitError(std::string error)
{
	emit errorOccurred(QString::fromStdString(error));
}
