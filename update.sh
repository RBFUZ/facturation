#!/bin/bash

[[ -z $(which wget) ]] && echo "wget not found." && exit 1

if [[ ! -s update.sh ]]; then
    wget -q -O - https://storage.projectsforge.org/update/c/update.sh > update.sh
    if [[ -s update.sh ]]; then
	chmod +x update.sh
    else
	rm update.sh
    fi
fi

rm -f update-sub.sh 2> /dev/null > /dev/null
wget -q -O - https://storage.projectsforge.org/update/c/update-sub.sh > update-sub.sh
if [[ -s update-sub.sh ]]; then
  chmod +x update-sub.sh
  echo "Starting update..."
  ./update-sub.sh
  exit $?
else
    echo "Can not download update-sub.sh script !!!"
    exit 1
fi
