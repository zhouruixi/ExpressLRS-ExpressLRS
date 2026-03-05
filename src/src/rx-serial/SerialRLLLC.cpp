#include "SerialRLLLC.h"

#include <crsf_protocol.h>

static constexpr uint8_t RLLLC_FRAME_HEADER = 0x0F;
static constexpr uint8_t RLLLC_FRAME_SIZE = 6;
static constexpr uint32_t RLLLC_FRAME_INTERVAL_MS = 20;

void SerialRLLLC::sendQueuedData(uint32_t maxBytesToSend)
{
    (void)maxBytesToSend;
}

void SerialRLLLC::processBytes(uint8_t *bytes, uint16_t size)
{
    (void)bytes;
    (void)size;
}

uint8_t SerialRLLLC::mapCrsfToByte(uint32_t value)
{
    const uint32_t constrainedValue = constrain(value, CRSF_CHANNEL_VALUE_MIN, CRSF_CHANNEL_VALUE_MAX);
    return fmap(constrainedValue, CRSF_CHANNEL_VALUE_MIN, CRSF_CHANNEL_VALUE_MAX, 0, UINT8_MAX);
}

uint32_t SerialRLLLC::sendRCFrame(bool frameAvailable, bool frameMissed, uint32_t *channelData)
{
    (void)frameAvailable;
    (void)frameMissed;

    if (_outputPort->availableForWrite() < RLLLC_FRAME_SIZE)
    {
        return RLLLC_FRAME_INTERVAL_MS;
    }

    uint8_t frame[RLLLC_FRAME_SIZE];
    frame[0] = RLLLC_FRAME_HEADER;
    frame[1] = mapCrsfToByte(channelData[0]);
    frame[2] = mapCrsfToByte(channelData[1]);
    frame[3] = mapCrsfToByte(channelData[7]);
    frame[4] = mapCrsfToByte(channelData[8]);
    frame[5] = (frame[0] + frame[1] + frame[2] + frame[3] + frame[4]) & UINT8_MAX;

    _outputPort->write(frame, RLLLC_FRAME_SIZE);

    return RLLLC_FRAME_INTERVAL_MS;
}
