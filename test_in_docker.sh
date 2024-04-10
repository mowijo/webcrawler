#/bin/bash

docker build . -r webcrawler
docker run --rm  -v `pwd`:/work webcrawler cd /work ; rm -rf tb; mkdir tb; cd tb; cmake ../ ; make -j ; ctest
