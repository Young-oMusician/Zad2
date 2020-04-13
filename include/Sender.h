
#ifndef ZAD2_SENDER_H
#define ZAD2_SENDER_H

#include <memory>
#include <string>
#include "types.h"
#include "COM.h"

class Sender{
private:
    std::shared_ptr<COM> com;

public:
    Sender(const std::shared_ptr<COM> com);

    void send(bytes &data);

    virtual ~Sender();
};

#endif //ZAD2_SENDER_H
