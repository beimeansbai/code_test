#!/bin/sh
cd $1
git log --reverse --pretty=format:"%H"
