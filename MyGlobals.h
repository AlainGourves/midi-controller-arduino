#pragma once

#ifndef MyGlobals_h
#define MyGlobals_h

#include <Arduino.h>
// MIDI
#include <USB-MIDI.h>
// Create and bind the MIDI interface to the default hardware Serial port
//USBMIDI_CREATE_DEFAULT_INSTANCE();
extern USBMIDI_NAMESPACE::usbMidiTransport usbMIDI(0);
extern MIDI_NAMESPACE::MidiInterface<USBMIDI_NAMESPACE::usbMidiTransport> MIDI((USBMIDI_NAMESPACE::usbMidiTransport&)usbMIDI);

#endif