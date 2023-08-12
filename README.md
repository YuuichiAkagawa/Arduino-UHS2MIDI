# Arduino USB Host Shield 2.0 MIDI Transport
[![arduino-library-badge](https://www.ardu-badge.com/badge/UHS2-MIDI.svg?)](https://www.ardu-badge.com/UHS2-MIDI)
[![Check Arduino status](https://github.com/YuuichiAkagawa/Arduino-UHS2MIDI/actions/workflows/check-arduino.yml/badge.svg)](https://github.com/YuuichiAkagawa/Arduino-UHS2MIDI/actions/workflows/check-arduino.yml")
[![License](https://img.shields.io/github/license/YuuichiAkagawa/Arduino-UHS2MIDI.svg?maxAge=3600)](LICENSE)

USB Host Shield 2.0  MIDI transport layer for the [FortySevenEffects Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library) and uses the underlying [USB Host Shield 2.0](https://github.com/felis/USB_Host_Shield_2.0) library.  
This library is based on the [Arduino-USBMIDI](https://github.com/lathoub/Arduino-USBMIDI).

## Installation
Install from Arduino Library Manager. Search for UHS2-MIDI.
![arduino-library-manager](https://user-images.githubusercontent.com/926923/117981071-92c41d80-b36f-11eb-9cd1-30423f708880.png)

## Usage
### Basic / Default
```cpp
#include <UHS2-MIDI.h>
...
USB Usb;
UHS2MIDI_CREATE_DEFAULT_INSTANCE(&Usb);
...
void setup()
{
  MIDI.begin(1);
  if (Usb.Init() == -1) {
    while (1); //halt
  }//if (Usb.Init() == -1...
  ...
}
void loop()
{
  Usb.Task();
  MIDI.read();
  ...
```
will create a instance named `MIDI` (transport instance named `__uhs2MIDI`) and is by default connected to cable number 0 - and listens to incoming MIDI on channel 1.

### Modified
```cpp
#include <UHS2-MIDI.h>
...
USB Usb;
UHS2MIDI_CREATE_INSTANCE(&Usb, 4, MIDI);
```
will create a instance named `MIDI` (transport instance named `__uhs2MIDI`) and is connected to cable number 4.

### Custom Settings
```cpp
#include <UHS2-MIDI.h>
...
struct MySettings : public midi::DefaultSettings
{
    static const unsigned SysExMaxSize = 512; // Accept SysEx messages up to 512 bytes long.
};
USB Usb;
UHS2MIDI_CREATE_CUSTOM_INSTANCE(&Usb, 0, MIDI, MySettings);
```
will create a instance named `MIDI` (transport instance named `__uhs2MIDI`) and change the maximum size of SysEx messages to 512 bytes.
### Advanced
```cpp
#include <UHS2-MIDI.h>
...
USB Usb;
UHS2MIDI_NAMESPACE::uhs2MidiTransport uhs2MIDI2(&Usb, 5);
MIDI_NAMESPACE::MidiInterface<UHS2MIDI_NAMESPACE::uhs2MidiTransport> MIDI2((UHS2MIDI_NAMESPACE::uhs2MidiTransport&)uhs2MIDI2);
```
will create a instance named `uhs2MIDI2` (and underlaying MIDI object `MIDI2`) and is by default connected to cable number 5.

### Change CableNumber
```cpp
#include <UHS2-MIDI.h>
...
USB Usb;
UHS2MIDI_CREATE_DEFAULT_INSTANCE(&Usb);
void setup()
{
  MIDI.begin(4);                            // Launch MIDI and listen to channel 4
  MIDI.getTransport()->setCableNumber(1);   // Change CableNumber to 1 later
```
## Tested boards / modules
- Arduino UNO R3

## Changelog
[Chagelog](/Changelog.md)

## Other Transport protocols:
The libraries below  the same calling mechanism (API), making it easy to interchange the transport layer.
- [Arduino USB-MIDI  Transport](https://github.com/lathoub/USB-MIDI)
- [Arduino AppleMIDI Transport](https://github.com/lathoub/Arduino-AppleMIDI-Library)
- [Arduino ipMIDI  Transport](https://github.com/lathoub/Arduino-ipMIDI)
- [Arduino BLE-MIDI  Transport](https://github.com/lathoub/Arduino-BLE-MIDI)
- [Arduino USB Host Library SAMD MIDI Transport](https://github.com/YuuichiAkagawa/Arduino-USBHSAMD-MIDI)
