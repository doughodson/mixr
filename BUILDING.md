# Building MIXR on Ubuntu 24.04 / GCC 13

## Dependencies

```bash
sudo apt install -y \
  build-essential \
  libfreetype-dev \
  libftgl-dev \
  libgl1-mesa-dev \
  libglu1-mesa-dev \
  freeglut3-dev \
  libprotobuf-dev \
  protobuf-compiler \
  libjpeg-dev \
  libpng-dev
```

## JSBSim

MIXR requires JSBSim v1.1.0 — later versions changed the API
from raw pointers to std::shared_ptr which breaks compilation.

```bash
git clone https://github.com/JSBSim-Team/jsbsim
cd jsbsim
git checkout v1.1.0
cmake -B build \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=../mixr-3rdparty \
  -DJSBSIM_BUILD_SHARED_LIBS=OFF
cmake --build build -j$(nproc)
cmake --install build
cd ..
```

## Build MIXR

```bash
source setenv.sh
cd src
make base
make simulation
make models
make models/dynamics/jsbsim
```

## Verified on

- Ubuntu 24.04 LTS
- GCC 13.3.0
- JSBSim v1.1.0
