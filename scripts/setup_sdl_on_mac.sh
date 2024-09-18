#!/bin/bash
source ./scripts/vars.sh

# Install SDL3
CURRENT_DIR=$DIRECTORY/SDL3
git submodule update --init -- $CURRENT_DIR

cd $CURRENT_DIR
cmake -S . -B $FOLDER -DSDL_STATIC=ON
cmake --build $FOLDER

cp $CURRENT_DIR/${FOLDER}/libSDL3.a $LIB_DIR
cp $CURRENT_DIR/${FOLDER}/libSDL3.dylib $LIB_DIR
cp -r $CURRENT_DIR/include $INCLUDE_DIR/SDL3

# Install SDL3_image

CURRENT_DIR=$DIRECTORY/SDL3_image
git submodule update --init -- $CURRENT_DIR

cd $CURRENT_DIR
cmake -S . -B $FOLDER -DSDL3_DIR=../SDL3/$FOLDER
cmake --build $FOLDER

# cp $CURRENT_DIR/${FOLDER}/libSDL3_image.a $LIB_DIR
cp $CURRENT_DIR/${FOLDER}/libSDL3_image.dylib $LIB_DIR
cp -r $CURRENT_DIR/include/SDL3_image $INCLUDE_DIR/SDL3/SDL3_image


# Install SDL3_ttf

CURRENT_DIR=$DIRECTORY/SDL3_ttf
git submodule update --init -- $CURRENT_DIR

cd $CURRENT_DIR
cmake -S . -B $FOLDER -DSDL3_DIR=../SDL3/$FOLDER
cmake --build $FOLDER

# cp $CURRENT_DIR/${FOLDER}/libSDL3_image.a $LIB_DIR
cp $CURRENT_DIR/${FOLDER}/libSDL3_ttf.dylib $LIB_DIR
cp -r $CURRENT_DIR/include/SDL3_ttf $INCLUDE_DIR/SDL3/SDL3_ttf