#!/bin/bash

for d in *sample_1/ ; do
  cd $d

  raccoon-opt -i field.i
  cd ..
  folder=${d::-1}
  echo $folder
  echo generating field screenshots for Gc...
  pvbatch gold/camera.py ${folder} field.e Gc 0.00072 0.00088 material
  echo generating field screenshots for psic...
  pvbatch gold/camera.py ${folder} field.e psic 0.000027 0.000033 material
  echo generating field screenshots for d...
  pvbatch gold/camera.py ${folder} out.e d 0 1 damage
done
