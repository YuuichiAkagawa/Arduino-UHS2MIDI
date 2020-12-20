/*
 Copyright (c) 2020 lathoub

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
/*
 *******************************************************************************
 * Original version: https://github.com/lathoub/Arduino-USBMIDI/blob/master/src/USB-MIDI.h
 *
 * Modified for the USB Host Shield 2.0 + USBH_MIDI
 * copyright (c) 2020 Yuuichi Akagawa
 *******************************************************************************
 */
 

#pragma once

#include <MIDI.h>
#include <usbh_midi.h>


#include "UHS2-MIDI_defs.h"
#include "UHS2-MIDI_Namespace.h"

BEGIN_UHS2MIDI_NAMESPACE
class uhs2MidiTransport : public USBH_MIDI
{
private:
    byte mTxBuffer[4];
    size_t mTxIndex;
    MidiType mTxStatus;

    byte mRxBuffer[4];
    size_t mRxLength;
    size_t mRxIndex;

    usbMidiEventPacket_t mPacket;
    uint8_t cableNumber;

public:
    uhs2MidiTransport(USB *p, uint8_t cableNumber = 0) : USBH_MIDI(p)
    {
      this->cableNumber = cableNumber;
    };

    static const bool thruActivated = false;

    uint8_t uhs2MidiTransport::SendData(usbMidiEventPacket_t packet){
        if( !bPollEnable ) return 4;
        return pUsb->outTransfer(bAddress, epInfo[epDataOutIndex].epAddr, 4, packet.buf);
    };

    usbMidiEventPacket_t uhs2MidiTransport::RecvData(){
      usbMidiEventPacket_t packet;
      if ( USBH_MIDI::RecvData(packet.buf, true) == 0 ) {
        packet.p.header = 0;
      }
      return packet;
    };


    void begin()
    {
      mTxIndex = 0;
      mRxIndex = 0;
      mRxLength = 0;
    };

    bool beginTransmission(MidiType status)
    {
      mTxStatus = status;

      byte cin = 0;
      if (status < SystemExclusive) {
        // Non System messages
        cin = type2cin[((status & 0xF0) >> 4) - 7][1];
        mPacket.p.header = MAKEHEADER(cableNumber, cin);
      }
      else {
        // Only System messages
        cin = system2cin[status & 0x0F][1];
        mPacket.p.header = MAKEHEADER(cableNumber, 0x04);
      }

      mPacket.p.byte1 = mPacket.p.byte2 = mPacket.p.byte3  = 0;
      mTxIndex = 0;

      return true;
    };

    void write(byte byte)
    {
      if (mTxStatus != MidiType::SystemExclusive) {
        if (mTxIndex == 0)      mPacket.p.byte1 = byte;
        else if (mTxIndex == 1) mPacket.p.byte2 = byte;
        else if (mTxIndex == 2) mPacket.p.byte3 = byte;
      }
      else if (byte == MidiType::SystemExclusiveStart) {
        mPacket.p.header = MAKEHEADER(cableNumber, 0x04);
        mPacket.p.byte1 = byte;
      }
      else // SystemExclusiveEnd or SysEx data
      {
        auto i = mTxIndex % 3;
        if (byte == MidiType::SystemExclusiveEnd)
          mPacket.p.header = MAKEHEADER(cableNumber, (0x05 + i));

        if (i == 0) {
          mPacket.p.byte1 = byte; mPacket.p.byte2 = mPacket.p.byte3 = 0x00;
        }
        else if (i == 1) {
          mPacket.p.byte2 = byte; mPacket.p.byte3 = 0x00;
        }
        else if (i == 2) {
          mPacket.p.byte3 = byte;
          if (byte != MidiType::SystemExclusiveEnd)
            SendData(mPacket);
        }
      }
      mTxIndex++;
    };

    void endTransmission()
    {
      SendData(mPacket);
    };

    byte read()
    {
      RXBUFFER_POPFRONT(byte);
      return byte;
    };

    unsigned available()
    {
      // consume mRxBuffer first, before getting a new packet
      if (mRxLength > 0)
        return mRxLength;

      mRxIndex = 0;

      mPacket = RecvData();
      if (mPacket.p.header != 0) {
        auto cn  = GETCABLENUMBER(mPacket);
        if (cn != cableNumber)
          return 0;

        auto cin = GETCIN(mPacket);
        auto len = cin2Len[cin][1];
        switch (len) {
          case 0:
            if (cin == 0x4 || cin == 0x7)
              RXBUFFER_PUSHBACK3
              else if (cin == 0x5)
                RXBUFFER_PUSHBACK1
                else if (cin == 0x6)
                  RXBUFFER_PUSHBACK2
                  break;
          case 1:
            RXBUFFER_PUSHBACK1
            break;
          case 2:
            RXBUFFER_PUSHBACK2
            break;
          case 3:
            RXBUFFER_PUSHBACK3
            break;
          default:
            break; // error
        }
      }

      return mRxLength;
    };
};


END_UHS2MIDI_NAMESPACE

#define UHS2MIDI_CREATE_INSTANCE(USBH, CableNr, Name)  \
  UHS2MIDI_NAMESPACE::uhs2MidiTransport __uhs2##Name(USBH, CableNr);\
  MIDI_NAMESPACE::MidiInterface<UHS2MIDI_NAMESPACE::uhs2MidiTransport> Name((UHS2MIDI_NAMESPACE::uhs2MidiTransport&)__uhs2##Name);

#define UHS2MIDI_CREATE_DEFAULT_INSTANCE(USBH)  \
  UHS2MIDI_CREATE_INSTANCE(USBH, 0, MIDI)
