
#include "types.h"

#ifndef ZAD2_CHECKSUM_ALGORITHMS_H
#define ZAD2_CHECKSUM_ALGORITHMS_H

const int ALGEBRAIC_CHECKSUM = 0;
const int CRC_CHECKSUM = 1;

byte algebraicChecksum(const bytes &data);
bytes crcChecksum(const bytes &data);

#endif //ZAD2_CHECKSUM_ALGORITHMS_H
