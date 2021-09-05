#!/bin/bash

# get into the right folder
cd "$(dirname "$0")"
cd ../..

mkdir -p res/noto/all
cd res/noto
curl -L "https://noto-website-2.storage.googleapis.com/pkgs/Noto-hinted.zip" -o \
	noto_hinted.zip

cd all
unzip ../noto_hinted.zip
mv *Regular.* ..

cd ..
rm -rf all
rm NotoSerif*
rm noto_hinted.zip
