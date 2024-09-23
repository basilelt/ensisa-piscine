#!/bin/bash
#set -euxo pipefail
set -euo pipefail

# Check if running on macOS
if [[ $(uname) == "Darwin" ]]; then
    # Install dependencies using brew
    brew install gcc make gtk+3 llvm subversion doxygen graphviz
fi

# Check if running on Linux
if [[ $(uname) == "Linux" ]]; then
    # Install dependencies using apt
    sudo apt update
    sudo apt install -y gcc make libgtk-3-dev lldb subversion doxygen graphviz
fi

# Make run_client.sh, and svnignore.sh executable
chmod +x compile.sh
chmod +x svnignore.sh
