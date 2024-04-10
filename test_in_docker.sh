#/bin/bash

docker build . -t webcrawler
docker run --rm  -v `pwd`:/work webcrawler cd /work ; rm -rf tb; mkdir tb; cd tb; cmake ../ ; make -j ; ctest
