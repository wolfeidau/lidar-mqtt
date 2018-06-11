# lidar-mqtt

This service reads scan data from the [ydlidar](http://www.ydlidar.com/product/X4) and writes it to an [MQTT](http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html) topic.

# build requirements

The build process currently supports a number of Unix and Linux flavors. The build process requires the following tools:
  * CMake (cmake.org)
  * GNU Make (www.gnu.org/software/make)
  * GCC (gcc.gnu.org)

On Debian based systems this requires you to install the following packages:

```
apt install gcc g++ git make cmake libssh-dev 
```

# building

To prepare for a build we need to download our dependencies, I have written a script which does this.

```
./setup.sh
```

Using cmake build the software.

```
mkdir build
cd build
cmake ..
make -j4
```

# dependencies

- [Paho MQTT C++ Client](https://eclipse.org/paho/clients/cpp/)
- [Paho MQTT C Client](https://eclipse.org/paho/clients/c/)
- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)
- [ydlidar](https://github.com/EAIBOT/ydlidar)

# license

Copyright 2018 Mark Wolfe and licensed under the Apache License, Version 2.0.