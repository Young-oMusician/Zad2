
#include "COM.h"
#include "Receiver.h"
#include "signals.h"
#include <chrono>
#include <windows.h>
#include "checksum_algorithms.h"
#include "Exceptions.h"

Receiver::Receiver(const std::shared_ptr<COM> &com) : com(com) {}

bytes Receiver::receive(int checksumAlgorithm) {
    auto connectionTimer = std::chrono::system_clock::now();
    auto signalDelayTimer = std::chrono::system_clock::now();
    bool timeExpires = false;
    bool receiveSOH = false;
    while (!timeExpires && !receiveSOH) {
        if (checksumAlgorithm == ALGEBRAIC_CHECKSUM) {
            com->write(bytes({NAK}));
        } else if (checksumAlgorithm == CRC_CHECKSUM) {
            com->write(bytes({C}));
        }
        signalDelayTimer = std::chrono::system_clock::now();
        while(std::chrono::system_clock::now() - signalDelayTimer < std::chrono::seconds(5)){
            if(com->read() == SOH){
                receiveSOH = true;
                break;
            }
        }
        if ((std::chrono::system_clock::now() - connectionTimer) > std::chrono::seconds(60)) {
            timeExpires = true;
        }
    }
    if (timeExpires) {
        throw SOHSignalNotReceiveException("Transmitter didn't answer SOH !");
    }
    byte currentBlock = 1;
    byte receivedBlock;
    byte transmitterMessage;
    bytes data;
    bytes block;
    do {
            receivedBlock = com->read();
            if(currentBlock != receivedBlock){
                for(int i = 0; i < 130; i++){
                    com->read();
                }
                com->write(bytes({NAK}));
                currentBlock--;
            }
            else {
                byte dopelnienie = com->read();
                block.clear();
                for (int i = 0; i < 128; i++) {
                    block.push_back(com->read());
                }
                if (checksumAlgorithm == ALGEBRAIC_CHECKSUM) {
                    byte receivedChecksum = com->read();
                    byte calculatedChecksum = algebraicChecksum(block);
                    if (receivedChecksum == calculatedChecksum) {
                        com->write(bytes({ACK}));
                        data.insert(data.end(), block.begin(), block.end());
                        currentBlock++;
                    } else {
                        com->write(bytes({NAK}));
                    }
                } else {
                    bytes calculatedChecksum = crcChecksum(block);
                    bytes receivedChecksum({com->read(),com->read()});
                    if (calculatedChecksum[0] == receivedChecksum[0] && calculatedChecksum[1] == receivedChecksum[1]) {
                        com->write(bytes({ACK}));
                        data.insert(data.end(), block.begin(), block.end());
                        currentBlock++;
                    } else {
                        com->write(bytes({NAK}));
                    }
                }
            }
//            Sleep(1000);
            transmitterMessage = com->read();
        }while (transmitterMessage == SOH);
        if (transmitterMessage == EOT) {
            com->write(bytes({ACK}));
        }
        else{
            throw XMODEMProtocolErrorException("Protocol Error !");
        }
        return data;
    }

Receiver::~Receiver() {

}

