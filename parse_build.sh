#!/bin/bash

# File to parse
FILE="$2"

# Read the selected node name from a temporary file
NODE="$1"

# Check if node name is empty
if [ -z "$NODE" ]; then
    echo "No node name provided."
    exit 1
fi

# Check if file exists
if [ ! -f "$FILE" ]; then
    echo "File not found: $FILE"
    exit 1
fi

echo "Building node: $NODE file: $FILE "
# Add your build command here, e.g., heptc or any other build tool
build_run -s "$NODE" -p "$FILE"