# Bloons Tools
Bloons Tools is a set of tools and modules for BloonsTD6 that runs on Windows and Linux.  
Development is **very** early on and is not intended to be used currently.

## Installation
There are no pre-built binaries due to the early age of the project.
They will be supplied when the project is more mature.  

You have to compile from source for now.

### Compiling
Each tool/module has their own requirements. Some requires others to work.  

First clone the project to a directory of your choice.  
```shell
git clone https://github.com/Sierra-Foxy/BloonsTools.git
cd BloonsTools
```

Then make a directory to build in. Don't build in source.
```shell
mkdir build
cd build
```

Then build and install the project.
```shell
cmake [<options>] ..
make
sudo make install
```
You can run `make` with the option `-j#` where # is the number of cores you want to use.  
You can build only certain tools using the `-D` option.  
e.g. `cmake -DBUILD_ALL=OFF -DBUILD_SERVER ..` to build only the server.
