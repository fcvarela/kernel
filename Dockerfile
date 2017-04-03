FROM ubuntu

RUN apt-get update
RUN apt-get install -y gcc-mingw-w64-x86-64 gcc gnu-efi make
