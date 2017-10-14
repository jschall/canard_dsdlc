# Libcanard_dsdlc

## Acknowledgements
HUGE thanks to Siddharth Purohit, who provided the prototype for this whole thing.

## Dependencies
- python 2.7
- pyuavcan
- empy

## Usage
```
canard_dsdlc.py [-h] namespace_dir [namespace_dir ...] build_dir
```

For example:
```
canard_dsdlc.py <path-to-uavcan-dsdl>/uavcan ./build
```
This will build canard serialization/deserialization code for the uavcan standard message set.
