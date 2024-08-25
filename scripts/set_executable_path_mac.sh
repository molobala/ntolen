#!/bin/bash
TARGET_NAME=$1
if [[ "$OSTYPE" == "darwin"* ]]; then
install_name_tool -add_rpath \@executable_path/libSDL3.0.dylib $TARGET_NAME
install_name_tool -change \@rpath/libSDL3.0.dylib \@executable_path/libSDL3.0.dylib $TARGET_NAME
install_name_tool -change /usr/local/lib/libSDL3.0.dylib \@executable_path/libSDL3.0.dylib $TARGET_NAME
fi