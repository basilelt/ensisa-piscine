#!/bin/bash
#set -euxo pipefail
set -euo pipefail

# Clear existing svn:ignore and svn:global-ignores properties recursively
svn propdel svn:ignore --recursive . >/dev/null 2>&1 || true
svn propdel svn:global-ignores . >/dev/null 2>&1 || true

# Set svn:global-ignores property at the root directory
svn propset svn:global-ignores -F .svnignore . >/dev/null

# Use svn status to process items
svn status --no-ignore | while read -r status path; do
    if [[ "$status" == "I" ]]; then
        # Ignored item
        if svn info "$path" >/dev/null 2>&1; then
            echo "Removing versioned and ignored item: $path"
            svn rm --keep-local "$path"
        else
            echo "Skipping unversioned and ignored item: $path"
        fi
    elif [[ "$status" == "?" ]]; then
        # Unversioned item
        echo "Adding unversioned item: $path"
        svn add "$path"
    fi
done
