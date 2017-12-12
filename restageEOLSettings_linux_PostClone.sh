#!/usr/bin/env bash
set +

# If you have already cloned the repository
# and forgot to set the eol to handle both
# linux and windows without altering the
# branch run this.

git config --global core.autocrlf input
# Configure Git on Linux to properly handle line endings

git add . -u
git commit -m "Saving files before refreshing line endings"
rm .git/index
git reset
git status
git add -u
# It is perfectly safe to see a lot of messages here that read
# "warning: CRLF will be replaced by LF in file."
git add .gitattributes
git commit -m "Normalize all the line endings"
