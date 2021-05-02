# Arduino USB Host Shield 2.0 MIDI Transport
Pre-release of USB Host Shield 2.0  MIDI transport layer for the [FortySevenEffects Arduino MIDI Library](https://github.com/FortySevenEffects/arduino_midi_library) and uses the underlying [USB Host Shield 2.0](https://github.com/felis/USB_Host_Shield_2.0) library.
This library is based on the [Arduino-USBMIDI](https://github.com/lathoub/Arduino-USBMIDI).

__This libraries is EXTREMELY ALPHA!__

## Installation
copy it in "libraries" directory.

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
  MIDI.begin();
  if (Usb.Init() == -1) {
    while (1); //halt
  }//if (Usb.Init() == -1...
...
void loop()
{
  Usb.Task();
  MIDI.read();
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

### Advanced
```cpp
#include <UHS2-MIDI.h>
...
USB Usb;
UHS2MIDI_NAMESPACE::uhs2MidiTransport uhs2MIDI2(&Usb, 5);
MIDI_NAMESPACE::MidiInterface<UHS2MIDI_NAMESPACE::uhs2MidiTransport> MIDI2((UHS2MIDI_NAMESPACE::uhs2MidiTransport&)uhs2MIDI2);
```
will create a instance named `uhs2MIDI2` (and underlaying MIDI object `MIDI2`) and is by default connected to cable number 5.

## Tested boards / modules
- Arduino UNO

## Other Transport protocols:
The libraries below  the same calling mechanism (API), making it easy to interchange the transport layer.
- [Arduino USB-MIDI  Transport](https://github.com/lathoub/USB-MIDI)
- [Arduino AppleMIDI Transport](https://github.com/lathoub/Arduino-AppleMIDI-Library)
- [Arduino ipMIDI  Transport](https://github.com/lathoub/Arduino-ipMIDI)
- [Arduino BLE-MIDI  Transport](https://github.com/lathoub/Arduino-BLE-MIDI)
- [Arduino USB Host Library SAMD MIDI Transport](https://github.com/YuuichiAkagawa/Arduino-USBHSAMD-MIDI)