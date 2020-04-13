
#include "checksum_algorithms.h"


byte algebraicChecksum(const bytes &data) {
    int sum = 0;
    for(byte addend : data){
        sum += addend;
        sum %= 256;
    }
    return (byte)sum;
}

bytes crcChecksum(const bytes &data){
    int crc;
    char i;

    crc = 0;
    for(byte it : data)
    {
        crc = crc ^  (int) it << 8;
        i = 8;
        do
        {
            if (crc & 0x8000)
                crc = crc << 1 ^ 0x1021;
            else
                crc = crc << 1;
        } while(--i);
    }
    bytes result;
    result.push_back((byte)(crc >>  8));
    result.push_back((byte)(crc >> 0));
    return result;
}

