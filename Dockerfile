FROM ubuntu:latest

RUN apt update && apt install -y \
    build-essential \
    cmake \
    flex \
    bison \
    libncurses5-dev \
    wget \
    git \
    python3 \
    python3-pip \
    python3-venv \
    ninja-build \
    ccache \
    gperf \
    libffi-dev \
    libssl-dev \
    dfu-util \
    libusb-1.0-0

RUN git clone --recursive https://github.com/espressif/esp-idf.git

WORKDIR /esp-idf
RUN /esp-idf/install.sh

ENV IDF_PATH=/esp-idf
ENV PATH=$IDF_PATH/tools:$PATH

RUN echo "source $IDF_PATH/export.sh >> /dev/null" >> ~/.bashrc

RUN mkdir /code
WORKDIR /code