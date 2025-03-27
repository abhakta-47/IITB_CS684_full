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
echo "heptagon compiled!"
# sed --in-place '/pervasives.h/d' "$dest_src/linefollower_types.h"

# Paths to files
line_follower_header="$dest_src/line_follower.h"
supervisor_code="$dest_src/supervisor.ino"

# Extract mappings from line_follower.h (format: variable name -> state name)
declare -A var_map
while read -r type varname; do
    varname_clean=$(echo "$varname" | tr -d ';')  # Remove semicolon
    var_map["$type"]="$varname_clean"
done < <(grep -E "Line_follower__st(_[0-9]+)?" "$line_follower_header")

# echo "Variable Map:"
# for varname in "${!var_map[@]}"; do
#     echo "$varname -> ${var_map[$varname]}"
# done

# Generate sed command dynamically
sed_cmd=""
while read -r line; do
    type=$(echo "$line" | awk '{print $1}')
    oldname=$(echo "$line" | awk -F '_mem\\.' '{print $2}' | tr -d ';')
    # echo "$type -> $oldname"
    sed_cmd+="s/_mem\.$oldname/_mem\.${var_map[$type]}/g; "
done < <(grep -E "Line_follower__st(_[0-9]+)? [a-zA-Z0-9_]+ *= *_mem\." "$supervisor_code")
# echo "$sed_cmd"

sed -i "$sed_cmd" "$supervisor_code"
echo "String replacements done!"
