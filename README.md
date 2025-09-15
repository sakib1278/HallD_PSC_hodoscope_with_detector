## Build Instructions

1. Create a build directory:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
## Run the simulation:
    ./sim

## Requirements
Geant4 (built with GEANT4_USE_QT and GEANT4_USE_OPENGL_X11 if you want GUI/visualization)

CMake 3.10+

A C++ compatible compiler

## Reading The Root File
x = position of the particle in the local x direction, unit in cm  
Px = momentum of the particle in the local x direction, unit in MeV  
energy = unit in MeV  
angle = angle of the particle with respect to the surface of the detector, unit in degree  
