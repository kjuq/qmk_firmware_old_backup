#!/bin/sh

# cd to the directory where the script is located
cd "$(dirname "$0")"

export PATH=~/Library/xPacks/@xpack-dev-tools/arm-none-eabi-gcc/11.3.1-1.1.2/.content/bin:$PATH
make planck/rev6:default_my_keymap:dfu-util

