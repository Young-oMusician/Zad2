
#ifndef ZAD2_RECEIVER_H
#define ZAD2_RECEIVER_H

#include "COM.h"
#include "types.h"
#include <memory>


class Receiver{
private:
    std::shared_ptr<COM> com;
public:
    Receiver(const std::shared_ptr<COM> &com);

    bytes receive(int checksumAlgorithm);

    virtual ~Receiver();
};


#endif //ZAD2_RECEIVER_H
