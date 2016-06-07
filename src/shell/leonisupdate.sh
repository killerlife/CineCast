#!/bin/bash

if [ -f "/home/leonis/update/leonisupdate.zt" ]
then
    cd /home/leonis/update
    tar -zxpvf leonisupdate.zt
    if [ -f "install.sh" ]
    then
	./install.sh
    fi
    rm -rf *
fi
