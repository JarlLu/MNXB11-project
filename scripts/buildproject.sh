#!/bin/bash

# This script builds the project inside the container, 
# using the provided Makefile.

# Find and change directory to git base folder
# This assumes the project is run inside a git folder
# One can remove this if no git is present and fix
# paths manually
GITROOT=$(git rev-parse --show-toplevel)

if [ -d $GITROOT ]; then
     cd $GITROOT
   else
     echo "not a git repository, exiting..."
     exit 1
fi

# Build the main, assuming Makefile is in the current path
echo "Starting build..."
startmnxb11container.sh "make -B"
