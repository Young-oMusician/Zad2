#ifndef ZAD2_EXCEPTIONS_H
#define ZAD2_EXCEPTIONS_H

#include <stdexcept>

class PortOpeningException : public std::runtime_error{
public:
    PortOpeningException(std::string message) : std::runtime_error(message){}
};

class SOHSignalNotReceiveException : public std::runtime_error{
public:
    SOHSignalNotReceiveException(const std::string &message) : std::runtime_error(message){}
};

class XMODEMProtocolErrorException : public std::runtime_error{
public:
    XMODEMProtocolErrorException(const std::string &arg) : runtime_error(arg) {}
};

class ConnectionCancelledException : std::runtime_error{
public:
    ConnectionCancelledException(const std::string &message) : std::runtime_error(message){}
};


#endif //ZAD2_EXCEPTIONS_H
