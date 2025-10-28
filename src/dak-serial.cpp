#include "dak-serial.hpp" // Includes declarations from obs_serial_plugin.hpp

// --- Qt Includes (Only QDebug needed here, others in header) ---
#include <QDebug>      // For logging (mapped to OBS blog)
#include <QStringList> // Needed for readData implementation

// ====================================================================
// SerialPortWorker Implementation
// ====================================================================

SerialPortWorker::SerialPortWorker(QObject *parent) : QObject(parent)
{
	// QSerialPort must be instantiated in the worker thread's context
}

SerialPortWorker::~SerialPortWorker()
{
	if (serialPort) {
		// Disconnect from the main thread if still connected
		QObject::disconnect(serialPort, nullptr, this, nullptr);
		delete serialPort;
		serialPort = nullptr;
	}
}

/**
 * @brief Slot to initialize and connect the serial port.
 */
void SerialPortWorker::connectPort()
{
	if (serialPort) {
		delete serialPort;
		serialPort = nullptr;
	}

	serialPort = new QSerialPort(portName, this);

	// Set the required serial port parameters
	serialPort->setBaudRate(baudRate);
	serialPort->setDataBits(QSerialPort::Data8);
	serialPort->setParity(QSerialPort::NoParity);
	serialPort->setStopBits(QSerialPort::OneStopBit);
	serialPort->setFlowControl(QSerialPort::NoFlowControl);

	// Attempt to open the port in ReadOnly mode
	if (serialPort->open(QIODevice::ReadOnly)) {
		blog(LOG_INFO, "QSerialPortWorker: Successfully opened port %s at %d baud.",
		     portName.toStdString().c_str(), baudRate);

		// Connect the readyRead signal to the reading slot
		connect(serialPort, &QSerialPort::readyRead, this, &SerialPortWorker::readData);
	} else {
		blog(LOG_ERROR, "QSerialPortWorker: Failed to open port %s: %s", portName.toStdString().c_str(),
		     serialPort->errorString().toStdString().c_str());
	}
}

/**
 * @brief Slot to disconnect and close the serial port.
 */
void SerialPortWorker::disconnectPort()
{
	if (serialPort && serialPort->isOpen()) {
		// Disconnect signals before closing to prevent unwanted readData calls during thread exit
		QObject::disconnect(serialPort, nullptr, this, nullptr);
		serialPort->close();
		blog(LOG_INFO, "QSerialPortWorker: Closed port %s.", portName.toStdString().c_str());
	}
	if (serialPort) {
		delete serialPort;
		serialPort = nullptr;
	}
}

/**
 * @brief Slot triggered by QSerialPort::readyRead() signal.
 * Reads data from the port and emits a signal for each complete line.
 */
void SerialPortWorker::readData()
{
	if (!serialPort)
		return;

	// Read all available data
	QByteArray newData = serialPort->readAll();

	// Convert to string and simplify (remove leading/trailing whitespace)
	QString dataString = QString::fromLocal8Bit(newData).simplified();

	if (!dataString.isEmpty()) {
		// Split by newline and emit each line separately (representing one complete value)
		QStringList lines = dataString.split('\n', Qt::SkipEmptyParts);
		for (const QString &line : lines) {
			// Remove carriage returns if present
			QString cleanedLine = line;
			if (cleanedLine.endsWith('\r')) {
				cleanedLine.chop(1);
			}
			emit dataReceived(cleanedLine);
		}
	}
}

// Include the necessary MOC implementation (must be at the end of the file)
#include "dak-serial.moc"
