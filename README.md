# rpu-emulation

A Sample RPU emulation project for Xilinx Zynq MPSOC

### How to build

run build script from the git root dir

```
./bootstrap.sh
./build.sh
```

### Install drivers

```
cd install
sudo insmod rpu_emulator.ko
```

### Run RPU emulator

```
sudo ./install/usr/local/bin/rpu_emulator
```

### Run APU app

```
sudo ./install/usr/local/bin/apu_app
```
