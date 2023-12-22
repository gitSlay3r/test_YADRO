#pragma once

class TapeDevice 
{
public:
    virtual ~TapeDevice() {}

    virtual bool read(int& data) = 0;
    virtual void write(int data) = 0;
    virtual void rewind() = 0;
    virtual void moveNext() = 0;
};