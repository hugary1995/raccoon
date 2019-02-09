#!/bin/bash

parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
cd "$parent_path"
cd ../doc/
./moosedocs.py build --check
ncftpput -R -v -u "${hugary_username}" -p "${hugary_password}" srv138.main-hosting.eu /public_html/raccoon/ /home/gary/.local/share/moose/site/*
