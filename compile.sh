#!/bin/bash
#set -euxo pipefail
set -euo pipefail

# Compile the client
make all

# Navigate to the client bin directory
cd build/bin

# Make the client executable
chmod +x game

# Copy the client executable to the root directory
cp ./game ../../game
