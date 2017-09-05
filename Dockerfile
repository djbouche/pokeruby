# docker build -t test/myass .
FROM ubuntu:latest
RUN apt-get update
RUN apt-get upgrade
RUN apt-get install -y git build-essential wget g++-multilib

ENV ROOT=/app
RUN mkdir -p $ROOT && chmod 777 $ROOT

ENV CACHE_ROOT=$ROOT/cache
ENV DEV_ROOT=$ROOT/dev
ENV DEVKITPRO=$ROOT/opt/devkitpro
ENV DEVKITARM=$DEVKITPRO/devkitARM
ENV AGBCC_ROOT=$DEV_ROOT/agbcc

RUN mkdir -p $CACHE_ROOT
RUN mkdir -p $DEV_ROOT
RUN mkdir -p $DEVKITPRO

# https://github.com/pret/pokeruby/blob/master/INSTALL.md
# https://devkitpro.org/wiki/Getting_Started/devkitARM
# How to not be stupid in the future: ADD . /path/inside/docker/container
RUN wget -O $ROOT/devkitARM.tar.bz2 http://homura.nolimitzone.com:3666/devkitARM_r47-x86_64-linux.tar.bz2
RUN wget -O $ROOT/libgba.tar.bz2 http://homura.nolimitzone.com:3666/libgba-20150106.tar.bz2
RUN tar xjvf $ROOT/devkitARM.tar.bz2 -C $ROOT/opt/devkitpro
RUN mkdir -p $ROOT/opt/devkitpro/libgba
RUN tar xjvf $ROOT/libgba.tar.bz2 -C $ROOT/opt/devkitpro/libgba

# Install https://github.com/pret/agbcc.git
RUN git clone https://github.com/pret/agbcc.git $AGBCC_ROOT
RUN cd $AGBCC_ROOT && ./build.sh

RUN apt-get install -y libpng-dev
# ENV RUBY_ROOT $DEV_ROOT/pokeruby
# RUN git clone https://github.com/pret/pokeruby.git $RUBY_ROOT
# RUN cd $RUBY_ROOT && ./build_tools.sh

# Post install stuff
# Open interactive docker:
# docker run -tiv $(pwd):/app/host test/myass bash
#
# cd /app/dev/agbcc && ./install.sh /app/host
# cd /app/host
# ./build_tools.sh