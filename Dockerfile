################
# distribution #
################
FROM ubuntu:19.04

##########################
# update and upgrade apt #
##########################
RUN apt upgrade && apt update

################################
# install all dev dependencies #
################################
#
# build-essential  - for g++ gcc and essentials for building programs
#
RUN apt install -y build-essential 

############################
# install all helper tools #
############################
#
# make             - used to build using makefiles
# nano             - basic console editing tool
#
RUN apt install -y make nano

# include all the source code files into this projets's image
ADD . /BrainTools

# set the working directory
WORKDIR /BrainTools

# compile and install the brain tools
RUN make && make install

# set home as workdir
WORKDIR /home