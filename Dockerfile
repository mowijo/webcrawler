FROM ubuntu:24.04

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install  -y  cmake
RUN DEBIAN_FRONTEND=noninteractive apt-get install -y \
    autotools-dev \
    build-essential \
    g++ \
    libboost-all-dev \
    libbz2-dev \
    libgmock-dev\
    libgtest-dev \
    libicu-dev \
    libssl-dev \
    python3.11-dev\
    ; \
    rm -rf /var/lib/apt/lists/*

