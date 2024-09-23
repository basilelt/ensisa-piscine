#!/bin/bash

# Check if the extensions.txt file exists
if [ ! -f extensions.txt ]; then
  echo "extensions.txt file not found!"
  exit 1
fi

# Install the extensions listed in extensions.txt
cat extensions.txt | xargs -L 1 code --install-extension

# Print a message indicating the import is complete
echo "VSCode extensions have been imported from extensions.txt"
