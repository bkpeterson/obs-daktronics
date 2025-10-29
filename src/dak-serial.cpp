#include "dak-serial.hpp"
#include <cstring>

SerialPort::SerialPort() 
    : opened(false)
    , reading(false)
    , currentBaudRate(BAUD_19200)
    , readTimeout(1000)
    , lineDelimiter('\n')
    , lineCallback(nullptr)
    , errorCallback(nullptr) 
    , _port("") {
}

SerialPort::~SerialPort() {
    stopReading();
    close();
}

std::unique_ptr<SerialPort> SerialPort::create() {
#ifdef _WIN32
    return std::make_unique<WindowsSerialPort>();
#else
    return std::make_unique<PosixSerialPort>();
#endif
}

bool SerialPort::open(const std::string& portName, BaudRate baudRate, char delimiter) {
    if (opened) {
        close();
    }

    currentBaudRate = baudRate;
    lineDelimiter = delimiter;

    if (platformOpen(portName)) {
        opened = true;
        _port = portName;
        return true;
    }
    return false;
}

void SerialPort::close() {
    if (!opened) return;
    
    stopReading();
    platformClose();
    opened = false;
    _port = "";
}

bool SerialPort::startReading() {
    if (!opened || reading) {
        return false;
    }

    reading = true;
    readThread = std::make_unique<std::thread>(&SerialPort::readThreadFunction, this);
    return true;
}

void SerialPort::stopReading() {
    if (!reading) return;

    reading = false;
    
    // Wake up any waiting threads
    queueCondition.notify_all();
    
    if (readThread && readThread->joinable()) {
        readThread->join();
    }
    readThread.reset();
}

void SerialPort::setLineReceivedCallback(LineReceivedCallback callback) {
    std::lock_guard<std::mutex> lock(callbackMutex);
    lineCallback = callback;
}

void SerialPort::setErrorCallback(ErrorCallback callback) {
    std::lock_guard<std::mutex> lock(callbackMutex);
    errorCallback = callback;
}

bool SerialPort::setBaudRate(BaudRate baudRate) {
    if (!opened) return false;
    if (platformSetBaudRate(baudRate)) {
        currentBaudRate = baudRate;
        return true;
    }
    return false;
}

bool SerialPort::setTimeout(int milliseconds) {
    readTimeout = milliseconds;
    if (!opened) return true;
    return platformSetTimeout(milliseconds);
}

void SerialPort::flush() {
    if (!opened) return;
    platformFlush();
}

std::vector<std::string> SerialPort::listPorts() {
    return platformListPorts();
}

bool SerialPort::hasPendingSignals() const {
    std::lock_guard<std::mutex> lock(queueMutex);
    return !signalQueue.empty();
}

void SerialPort::processSignals() {
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
    
    for (const auto& signal : signalsToProcess) {
        switch (signal.type) {
            case Signal::LINE_RECEIVED:
                if (lineCallback) {
                    lineCallback(signal.data);
                }
                break;
                
            case Signal::ERROR:
                if (errorCallback) {
                    errorCallback(signal.data);
                }
                break;
        }
    }
}

void SerialPort::readThreadFunction() {
    std::string lineBuffer;
    char readBuffer[256];

    while (reading && opened) {
        int bytesRead = platformRead(readBuffer, sizeof(readBuffer) - 1);
        
        if (bytesRead > 0) {
            readBuffer[bytesRead] = '\0';
            
            // Process each character
            for (int i = 0; i < bytesRead; i++) {
                char c = readBuffer[i];
                
                if (c == lineDelimiter) {
                    // Complete line received - emit signal
                    if (!lineBuffer.empty()) {
                        emitLineReceived(lineBuffer);
                        lineBuffer.clear();
                    }
                } else if (c != '\r') {  // Skip carriage returns
                    lineBuffer += c;
                    
                    // Prevent buffer overflow
                    if (lineBuffer.size() > 4096) {
                        emitError("Line buffer overflow - line too long");
                        lineBuffer.clear();
                    }
                }
            }
        } else if (bytesRead < 0) {
            emitError("Read error occurred");
            // Small delay to prevent tight loop on persistent errors
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        // bytesRead == 0 means timeout, which is normal
    }
}

void SerialPort::emitLineReceived(const std::string& line) {
    std::lock_guard<std::mutex> lock(queueMutex);
    Signal signal;
    signal.type = Signal::LINE_RECEIVED;
    signal.data = line;
    signalQueue.push(signal);
    queueCondition.notify_one();
}

void SerialPort::emitError(const std::string& error) {
    std::lock_guard<std::mutex> lock(queueMutex);
    Signal signal;
    signal.type = Signal::ERROR;
    signal.data = error;
    signalQueue.push(signal);
    queueCondition.notify_one();
}

// Windows Implementation
#ifdef _WIN32
#include <setupapi.h>
#include <devguid.h>
#pragma comment(lib, "setupapi.lib")

WindowsSerialPort::WindowsSerialPort() : SerialPort(), hSerial(INVALID_HANDLE_VALUE) {
    memset(&dcb, 0, sizeof(dcb));
    memset(&timeouts, 0, sizeof(timeouts));
    dcb.DCBlength = sizeof(dcb);
}

WindowsSerialPort::~WindowsSerialPort() {
    platformClose();
}

bool WindowsSerialPort::platformOpen(const std::string& portName) {
    std::string fullPortName = "\\\\.\\" + portName;
    hSerial = CreateFileA(fullPortName.c_str(),
                          GENERIC_READ,
                          0, NULL, OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL, NULL);

    if (hSerial == INVALID_HANDLE_VALUE) {
        return false;
    }

    if (!GetCommState(hSerial, &dcb)) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
        return false;
    }

    dcb.BaudRate = currentBaudRate;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;
    dcb.fDtrControl = DTR_CONTROL_ENABLE;

    if (!applyDCB()) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
        return false;
    }

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = readTimeout;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 0;
    timeouts.WriteTotalTimeoutMultiplier = 0;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
        return false;
    }

    return true;
}

void WindowsSerialPort::platformClose() {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
}

int WindowsSerialPort::platformRead(char* buffer, int size) {
    DWORD bytesRead;
    if (ReadFile(hSerial, buffer, size, &bytesRead, NULL)) {
        return bytesRead;
    }
    return -1;
}

void WindowsSerialPort::platformFlush() {
    PurgeComm(hSerial, PURGE_RXCLEAR);
}

bool WindowsSerialPort::platformSetBaudRate(BaudRate baudRate) {
    dcb.BaudRate = baudRate;
    return applyDCB();
}

bool WindowsSerialPort::platformSetTimeout(int milliseconds) {
    timeouts.ReadTotalTimeoutConstant = milliseconds;
    return SetCommTimeouts(hSerial, &timeouts) != 0;
}

bool WindowsSerialPort::applyDCB() {
    return SetCommState(hSerial, &dcb) != 0;
}

std::vector<std::string> SerialPort::platformListPorts() {
    std::vector<std::string> ports;
    
    for (int i = 1; i <= 255; i++) {
        std::string portName = "COM" + std::to_string(i);
        std::string fullName = "\\\\.\\" + portName;
        
        HANDLE hPort = CreateFileA(fullName.c_str(),
                                   GENERIC_READ,
                                   0, NULL, OPEN_EXISTING, 0, NULL);
        
        if (hPort != INVALID_HANDLE_VALUE) {
            ports.push_back(portName);
            CloseHandle(hPort);
        }
    }
    
    return ports;
}

#else
// POSIX Implementation (Linux/macOS)
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <sys/stat.h>

PosixSerialPort::PosixSerialPort() : SerialPort(), fd(-1) {
    memset(&tty, 0, sizeof(tty));
    memset(&tty_old, 0, sizeof(tty_old));
}

PosixSerialPort::~PosixSerialPort() {
    platformClose();
}

bool PosixSerialPort::platformOpen(const std::string& portName) {
    fd = ::open(portName.c_str(), O_RDONLY | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        return false;
    }

    if (tcgetattr(fd, &tty) != 0) {
        ::close(fd);
        fd = -1;
        return false;
    }

    tty_old = tty;

    // Configure port
    speed_t speed = baudRateToSpeed(currentBaudRate);
    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_iflag &= ~IGNBRK;
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = readTimeout / 100;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~(PARENB | PARODD);
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (!applyTermios()) {
        ::close(fd);
        fd = -1;
        return false;
    }

    return true;
}

void PosixSerialPort::platformClose() {
    if (fd >= 0) {
        tcsetattr(fd, TCSANOW, &tty_old);
        ::close(fd);
        fd = -1;
    }
}

int PosixSerialPort::platformRead(char* buffer, int size) {
    return ::read(fd, buffer, size);
}

void PosixSerialPort::platformFlush() {
    tcflush(fd, TCIFLUSH);
}

bool PosixSerialPort::platformSetBaudRate(BaudRate baudRate) {
    speed_t speed = baudRateToSpeed(baudRate);
    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);
    return applyTermios();
}

bool PosixSerialPort::platformSetTimeout(int milliseconds) {
    tty.c_cc[VTIME] = milliseconds / 100;
    return applyTermios();
}

bool PosixSerialPort::applyTermios() {
    return tcsetattr(fd, TCSANOW, &tty) == 0;
}

speed_t PosixSerialPort::baudRateToSpeed(BaudRate baudRate) {
    switch (baudRate) {
        case BAUD_9600: return B9600;
        case BAUD_19200: return B19200;
        case BAUD_38400: return B38400;
        case BAUD_57600: return B57600;
        case BAUD_115200: return B115200;
        default: return B9600;
    }
}

std::vector<std::string> SerialPort::platformListPorts() {
    std::vector<std::string> ports;
    
    DIR* dir = opendir("/dev");
    if (dir) {
        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string name = entry->d_name;
            
#ifdef __APPLE__
            if (name.find("tty.") == 0 || name.find("cu.") == 0) {
#else
            if (name.find("ttyUSB") == 0 || 
                name.find("ttyACM") == 0 || 
                name.find("ttyS") == 0) {
#endif
                std::string fullPath = "/dev/" + name;
                
#ifdef __APPLE__
                int testFd = ::open(fullPath.c_str(), O_RDONLY | O_NOCTTY | O_NONBLOCK);
                if (testFd >= 0) {
                    ports.push_back(fullPath);
                    ::close(testFd);
                }
#else
                struct stat info;
                if (stat(fullPath.c_str(), &info) == 0 && S_ISCHR(info.st_mode)) {
                    ports.push_back(fullPath);
                }
#endif
            }
        }
        closedir(dir);
    }
    
    return ports;
}

#endif