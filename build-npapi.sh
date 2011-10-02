#!/bin/bash

function log() {
    echo $* 1>&2
}

function builddir() {
    echo "bldnpapi"
}

function pkgdir() {
    echo `builddir`/$1.plugin
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

function obj() {
    log "compiling $*"
    dst=`builddir`/$1.o
    shift
    cc -m32 -fvisibility=hidden -fPIC -Wall -O2 -c -o $dst $*
    echo $dst
}

function lib() {
    log "archiving $*"
    dst=$1
    shift
    ar rcs `builddir`/lib$dst.a $*
    echo "-l$dst"
}

function plg() {
    log "linking $*"
    dst=`builddir`/$1
    shift
    obj=$1
    shift
    cc -m32 -fvisibility=hidden -fPIC -bundle -flat_namespace -framework OpenGL -framework AppKit -framework WebKit -framework QuartzCore -o $dst $obj -L `builddir` $*
    echo $dst
}

function pkg() {
    log "packaging $*"
    install -d `resdir $1`
    install -d `bindir $1`
    sed s/NAME/$1/g templates/npapi/webtemplate.r > `builddir`/$1.r
    /Developer/Tools/Rez -o `resdir $1`/$1.rsrc -useDF `builddir`/$1.r
    sed s/NAME/$1/g templates/npapi/webtemplate.plist > `condir $1`/Info.plist
    cp $2 `bindir $1`
    echo `pkgdir $1`
}

function ins() {
    log "installing $*"
    dst="$HOME/Library/Internet Plug-Ins"
    cp -R $1 "$dst/"
    echo "$dst/$1"
}

rm -fR `builddir`
install -d `builddir`
gsobjs="`obj gs gs.c` `obj npapicocoa npapicocoa.m`"
libgs=`lib gs $gsobjs`
testobj=`obj test test.c`
testplg=`plg testplugin $testobj $libgs`
testpkg=`pkg testplugin $testplg`
itestpkg=`ins $testpkg`
echo "installed $itestpkg"

