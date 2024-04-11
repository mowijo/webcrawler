#/bin/bash

docker build . -t webcrawler
echo -e "#!/bin/bash\ncd /work \n rm -rf tb \n mkdir tb \n cd tb \n cmake ../ \n make -j \n ctest " > work
chmod ugo+x work
docker run --rm  -w /work  -v `pwd`:/work webcrawler ./work
rm work
rm -rf tb
