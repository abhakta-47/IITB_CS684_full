#!/usr/bin/env bash

src="/volume1/builds/$1_c"
dest_src="$2"
# echo "$src $dest_src"
# exit 0
cp "$src/$1_types.c" "$dest_src/$1_types.cpp"
cp "$src/$1_types.h" "$dest_src/$1_types.h"
cp "$src/$1.c" "$dest_src/$1.cpp"
cp "$src/$1.h" "$dest_src/$1.h"
sed -i 's/int/long/g' "$dest_src/$1.cpp"
sed -i 's/int/long/g' "$dest_src/$1.h"
sed -i 's/int/long/g' "$dest_src/$1_types.h"
echo "heptagon compiled"
# sed --in-place '/pervasives.h/d' "$dest_src/linefollower_types.h"

