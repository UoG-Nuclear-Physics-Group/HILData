# HILData

This library provides the data parser and detector classes for data taken with EAGLE at the HIL (Heavy Ion Laboratory) in Warsaw.
The names of the classes typically start with THIL.

The data parser is set up to process EAGLE data written in the lst data format.

Two detector classes are implimented: Eagle HPGe (TEagle), Eagle Pulser (TEaglePulser).
In addition the hit class TPinDiodeHits exist, which is stored in TEagle::fPinDiodeHits.

Other classes included are:
 - File and event classes for RLMD files,
 - THILMnemonic, which provides an enumeration for the detector systems and digitizers, and
 - THILDetectorInformation, which only provides functions to tell whether EAGLE data is present (which should be all the time)

## Installation

To compile simply call ```make``` from inside the HILData directory.

Tested with GRSISort ```v4.0.0.3``` and ROOT ```v6.26/00```.

## Cal Information

The nomenclature of HILData is as follows:
 - **EA** denotes the TEagle system
 - **PD** denotes a pin diode
 - **PU** denotes a pulser, and is based on the Generic Detector system

All EAGLE detector hits are set to address 0x100 plus their ID, and all pin diode hits are set to address 0x200 plus their ID.

Since each RLMD eagle event contains the complete information of that event (one or more Germanium hits, and one or more PIN diode hits), all of this information is stored in a TEagle detector.
The default address used for this TEagle detector is ```0x100```, and for it to be sorted correctly, the cal-file needs at least this one address to be defined:

```
EAX01XN00X      {
Type:      TEagle
Name:      EAX01XN00X
Number:    0
Address:   0x00000100
TimeOffset: 0
Integration: 0
}
//====================================//
```

