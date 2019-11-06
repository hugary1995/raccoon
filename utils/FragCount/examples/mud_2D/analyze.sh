#!/bin/bash

for d in $1*/ ; do
    cd $d
    gnome-terminal -e "../../../FragCount config.yml"
    cd ..
done
