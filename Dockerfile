FROM ubuntu:20.04

LABEL maintainer="prmottajr@gmail.com"
LABEL version="1.1.0"
LABEL description="This is custom Docker Image for the neblina-core library."

ARG DEBIAN_FRONTEND=noninteractive

RUN apt update

RUN apt install -y git gcc g++ libgtest-dev cmake vim libunwind8 && \
    rm -rf /var/lib/apt/lists/* && \
    apt clean

RUN cd /tmp && \
    git clone https://github.com/paulomotta/neblina-core && \
    cd neblina-core && \
    cmake . && \
    make && \
    make install && \
    ldconfig 

RUN ln -s /usr/local/lib /usr/local/lib64

# COPY DrMemory-Linux-2.5.0 /tmp/DrMemory-Linux-2.5.0

# COPY src /tmp/neblina-core/src
# COPY include /tmp/neblina-core/include
# COPY test /tmp/neblina-core/test
# COPY CMakeLists.txt /tmp/neblina-core/CMakeLists.txt

# RUN cd /tmp && \
#     cd neblina-core && \
#     cmake . && \
#     make && \
#     make install && \
#     ldconfig 

VOLUME ["/src"]