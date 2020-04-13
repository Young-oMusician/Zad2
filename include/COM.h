
#ifndef ZAD2_COM_H
#define ZAD2_COM_H

#include <windows.h>
#include <string>
#include <stdexcept>
#include "types.h"



class COM{
private:
    HANDLE com;

public:
    COM(std::string portNumber);

    void write(const bytes &data);

    byte read();

    virtual ~COM();

};

#endif //ZAD2_COM_H
