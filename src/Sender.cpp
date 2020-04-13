#include "Sender.h"
#include "signals.h"
#include "checksum_algorithms.h"
#include "types.h"
#include "Exceptions.h"

Sender::Sender(const std::shared_ptr<COM> com) : com(com){}

Sender::~Sender() {}
//throws ConnectionCancelledException
void Sender::send(bytes &data) {
    byte connectionSignal = 0;
    byte checksumAlgorithm;
    //oczekiwanie na sygnał odbiornika (na jego zasadzie dobiera algorytm wyliczania sumy kontrolnej)
    do{
        connectionSignal = com->read();
    }while(connectionSignal == 0);
    if(connectionSignal == NAK){
        checksumAlgorithm = 0;
    }
    else if(connectionSignal == C){
        checksumAlgorithm = 1;
    }
    else{
        throw XMODEMProtocolErrorException("Protocol error when trying to connect !");
    }
    //wyodrębnia 128 bajtowe bloki z danych z pliku
    int numberOfPackages = (data.size() / 128) + 1;
    for(byte bytesIterator = 0; bytesIterator < numberOfPackages; bytesIterator++){
        //dzielenie danych na bloki 128 bajtowe
        bytes::iterator packageBegin = data.begin() + bytesIterator * 128;
        bytes::iterator packageEnd;
        //w przypadku gdy dojdziemy do końca wektora danych
        if(data.end() - packageBegin <= 128){
            packageEnd = data.end();
        }
        else{
           packageEnd = data.begin() + (bytesIterator + 1) * 128;
        }
        //zapakowanie bloku do wektora
        bytes package(packageBegin,packageEnd);
        //dopełnienie wektora do pełnych 128 bajtów
        if(package.size() < 128){
            package.insert(package.end(),128 - package.size(), 0);
        }
        bytes header = {SOH,
                        (byte)(bytesIterator+1),
                        (byte) (255 - (bytesIterator + 1))};
        com->write(header);
        com->write(package);
        if(checksumAlgorithm == ALGEBRAIC_CHECKSUM){
            com->write(bytes({algebraicChecksum(package)}));
        }
        else{
            com->write(crcChecksum(package));
        }
        //nasłuchiwanie odpowiedzi odbiorcy
        byte answer = 0;
        do{
            answer = com->read();
        }while(answer == 0);
        if(answer == NAK){
            bytesIterator--;
        }else if(answer == CAN){
            throw ConnectionCancelledException("Connection Cancelled !!");
        }
        Sleep(1000);
    }
    byte finalACK = 0;
    do{
        com->write(bytes({EOT}));
        Sleep(1000);
        finalACK = com->read();
    }while(finalACK != ACK);
}
