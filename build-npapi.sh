#!/bin/bash

function builddir() {
    echo "bldnpapi"
}

function pkgdir() {
    echo `builddir`/$1.webplugin
}

function condir() {
    echo `pkgdir $1`/Contents
}

function resdir() {
    echo `condir $1`/Resources
}

function bindir() {
    echo `condir $1`/MacOS
}

function compile() {
    echo "compiling $1"
    cc -fvisibility=hidden -fPIC -Wall -O2 -c -o `builddir`/$*
}

function link() {
    echo "linking $1"
    cc -fvisibility=hidden -fPIC -bundle -flat_namespace -framework OpenGL -framework AppKit -framework WebKit -framework QuartzCore -o `builddir`/$1 `builddir`/$2 `builddir`/$3
}

function package() {
    echo "packaging $1"
    install -d `resdir $1`
    install -d `bindir $1`
    sed s/NAME/$1/g webtemplate.r > `builddir`/$1.r
    /Developer/Tools/Rez -o `resdir $1`/$1.rsrc -useDF `builddir`/$1.r
    sed s/NAME/$1/g webtemplate.plist > `condir $1`/Info.plist
    cp `builddir`/$1 `bindir $1`
}

function pkginst() {
    echo "installing $1"
    cp -R `pkgdir $1` "$HOME/Library/Internet Plug-Ins/"
}

install -d `builddir`
compile npapicocoa.o npapicocoa.m
compile test.o test.c
link testplugin test.o npapicocoa.o
package testplugin
pkginst testplugin
