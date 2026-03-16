#!/usr/bin/bash
echo "install font ..."

sudo mkdir /usr/share/clc && sudo mv debian/font.ttf /usr/share/clc/
clear

echo "installing following package:\nlibx11-xcb-dev\nlibxcb-composite0\nlibxcb-composite0-dev\nlibx11-dev\nlibxcursor-dev\nlibxrandr-dev\nlibgl1-mesa-dev\nlibxfixes-dev\nlibxext-dev\nlibxcb-cursor-dev\nlibxi-dev\nxorg-dev\nmake\ncmake"
sudo apt update
sudo apt install -y libx11-xcb-dev libxcb-composite0 libxcb-composite0-dev libx11-dev libxcursor-dev libxrandr-dev libgl1-mesa-dev libxfixes-dev libxext-dev libxcb-cursor-dev libxi-dev xorg-dev make cmake
git clone https://github.com/ColleagueRiley/RGFW.git include/RGFW
git clone https://github.com/DareksCoffee/GlyphGL.git include/glyph
