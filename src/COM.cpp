#include "COM.h"
#include "Exceptions.h"
#include <string>

using namespace std;

//throws PortOpeningException
COM::COM(string portNumber){
    //Otwiera port podany w pierwszym parametrze funkcji
    com = CreateFile(("\\\\.\\"+portNumber).c_str(),          //numer portu
                     GENERIC_READ | GENERIC_WRITE,            //tryb dostępu
                     0,                                       //opcje udostępniania (niedostępne dla RS-232 więc 0)
                     NULL,                                    //tryb bezpieczeństwa (jw.)
                     OPEN_EXISTING,                           //otwiera tylko istniejące (nie można stworzyć portu software'owo)
                     0,                                       //opcja overlappingu (wykożystywane przy programach wielowątkowych)
                     NULL);                                   //parametr używane dla operacji na plikach
    //sprawdzenie czy port otwarto poprawnie
    if(com == INVALID_HANDLE_VALUE){
    throw PortOpeningException("Cannot open selected serial port !");
    }

    //inicjalizacja i ustawienie konfiguracji otwartego wcześniej portu
    DCB dcbSerialParams = {0};
    GetCommState(com, &dcbSerialParams);                        //pobranie danych konfiguracyjnych portu
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.Parity = NOPARITY;
    SetCommState(com, &dcbSerialParams);                        //ustawienie zdefiniowanej konfiguracji
    //inicjalizacja i ustawienie konfiguracji zegarów
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 5000;
    timeouts.ReadTotalTimeoutConstant = 5000;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 5000;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    SetCommTimeouts(com, &timeouts);
}

void COM::write(const bytes &data){
    DWORD noOfBytesWritten = 0;
    WriteFile(com,
            data.data(),
            data.size(),
            &noOfBytesWritten,
            NULL);

}

byte COM::read() {
    byte readByte;
    DWORD noOfReadBytes = 0;

    ReadFile(com,
            &readByte,
            sizeof(readByte),
            &noOfReadBytes,
            NULL);
    return readByte;
}

COM::~COM() {
    CloseHandle(com);
}
