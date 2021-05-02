/*!
 *  @file       UHS2-MIDI_defs.h
 *  Project     Arduino UHS2 MIDI transport
 *  @brief      UHS2 MIDI transport for the Arduino - Definitions
 *  @author     Yuuichi Akagawa
 *  @date       20/12/2020
 *  @license    MIT - Copyright (c) 2020-2021 Yuuichi Akagawa
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include "UHS2-MIDI_Namespace.h"

BEGIN_UHS2MIDI_NAMESPACE

//USB-MIDI event packet
typedef union
{
  uint8_t buf[4];
  struct{
    uint8_t header;
    uint8_t byte1;
    uint8_t byte2;
    uint8_t byte3;
  } p;
} usbMidiEventPacket_t;

using namespace MIDI_NAMESPACE;

const uint8_t Fsys2cin[] PROGMEM =  {0, 2, 3, 2, 0, 0, 5, 0, 0xf, 0, 0xf, 0xf, 0xf, 0, 0xf, 0xf};

#define MAKEHEADER(cn, cin) (((cn & 0x0f) << 4) | cin)

END_UHS2MIDI_NAMESPACE
