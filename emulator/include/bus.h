#pragma once

namespace Frankenstein {

template <typename AddressingType>
class IIRQListener
{
public:
    virtual void processIRQ() = 0;
};

template <typename AddressingType>
class IMemoryListener
{
public:
    virtual void read() = 0;
    virtual void write() = 0;
};

template <typename AddressingType>
class Bus {
public:
    void register(IIRQListener listener);
    void register(IMemoryListener listener, AddressingType address);
};

}


