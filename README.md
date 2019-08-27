# BrainTools
 
This repository contains the Brain Tools library which is a collection of algorithms for brain signals manipulation and classification through AI methods.
 
# Pre requisites
 
This library was tested in Ubuntu 16.04, 18.04, and 19.04. Make sure you are using one of those distributions with build-essentials installed.
 
## Ubuntu installation
 
To install the build-essentials package through apt in a Ubuntu Linux do the following.
 
```
sudo apt upgrade && apt update && apt install -u build-essentials
```
 
In order to install the brain tools library into your system do the following in this project’s root directory.
 
```
sudo make && make install
```
 
# Docker
 
In order to allows to setup a clean and fresh development environment, this repository contains a Dockerfile. This file allows you to build a linux base docker image and install the Brain Tools library on it so it can be used asap. To build its docker file do the following in the project's root directory.

```
docker build -t brain_tools .
```

To run its contains run.

```
docker run -it -v /home:/home --name brain_tools_dev_container bash
```

You will be prompted to this project's container with the library already installed. It is a fresh Ubuntu distribution that allows you to develop your programs using the library. When running the container with the aforementioned command, your home folder from the host system will be mapped as a volume into the container machine so you can use any IDE and other editing softwares and yet run your created code inside your container environment. 

# Contributors

Alysson Ribeiro da Silva (Maintainer)
Cleiton Lopes Aguiar
Elieyer Fermino de Oliveira