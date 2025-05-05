FROM fedora:43

ADD . /app

WORKDIR /app

RUN dnf install -y qt6-qtbase-devel 
RUN dnf install -y git protobuf-compiler protobuf-devel which patchelf lsb_release cargo awk

RUN git submodule update --init --recursive
