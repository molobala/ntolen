#!/bin/bash
source ./scripts/vars.sh
git submodule update --init -- $DIRECTORY

cd $DIRECTORY
cmake -S . -B $FOLDER -DSDL_STATIC=ON
cmake --build $FOLDER
cmake --install $FOLDER
# if ! [ -d "$DIRECTORY/$FOLDER" ]; then
#   mkdir ${FOLDER} ; #cd ${FOLDER}
#   cmake -S . -B build #--disable-shared
# else
#   cd $DIRECTORY/$FOLDER
# fi

# make
# make install


cp $DIRECTORY/${FOLDER}/libSDL3.a $LIB_DIR
cp $DIRECTORY/${FOLDER}/libSDL3.dylib $LIB_DIR
# cp $DIRECTORY/${FOLDER}/sdl3-config $LIB_DIR/sdl3-config

cp -r $DIRECTORY/include $INCLUDE_DIR/SDL3
# cp -r $DIRECTORY/${FOLDER}/include/SDL_config.h  $INCLUDE_DIR/SDL3/SDL_config.h