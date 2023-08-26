#!/bin/bash

echo ""
git add *
echo ""
echo "- git files added -----------------------------------------------------"
echo ""
git stash
echo ""
echo "- git files stashed ---------------------------------------------------"
echo ""
git pull
echo ""
echo "- git pull completed --------------------------------------------------"
echo ""
