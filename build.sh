#!/bin/bash

docker run -it -v `pwd`:/build efibuilder /bin/sh -c 'cd /build && make'
