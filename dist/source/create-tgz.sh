#!/bin/bash

git clone ../.. gravit-$1
git checkout tags/$1
tar czvf gravit-$1.tgz --exclude .git gravit-$1
