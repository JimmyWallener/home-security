#ifndef UNOCOMM_H
#define UNOCOMM_H

#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>

class UNOComm {
public:
    UNOComm();
    void begin();
    static void onReceiveWrapper(int numBytes);
    void onReceive(int numBytes);
    String getRtcData();
private:
    DateTime currentTime;
    static UNOComm* instance; // Pointer to the current instance
};

#endif // UNOCOMM_H
