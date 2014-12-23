# node-mapbox-gl-native

A library that renders map tiles in OpenGL using [mapbox-gl-native](https://github.com/mapbox/mapbox-gl-native).

[![Build Status](https://api.travis-ci.com/mapbox/node-mapbox-gl-native.svg?token=Phdq58g7NsfstW6gyeYW)](https://magnum.travis-ci.com/mapbox/node-mapbox-gl-native)

## API

```
var mbgl = require('mapbox-gl-native');

var map = new mbgl.Map();
map.load(style);
mbgl.render(info, function(err, image) {});
```

## Installing

By default, installs binaries. On these platforms no external dependencies are needed.

- 64 bit OS X or 64 bit Linux
- Node v0.10.x

Just run:

```
npm install
```

However, other platforms will fall back to a source compile: see [building from source](#building-from-source) for details.

## Building from source

Install [mapbox-gl-native](https://github.com/mapbox/mapbox-gl-native), then run `npm install --build-from-source`.

## Testing

On Linux you need XServer running:

```sh
sudo apt-get install xvfb
export DISPLAY=:99
XVFBARGS="$DISPLAY -ac -screen 0 1024x768x16"
PIDFILE=${HOME}/xvfb_${DISPLAY:1}.pid
/sbin/start-stop-daemon --start --quiet --pidfile $PIDFILE --make-pidfile --background --exec $XVFB -- $XVFBARGS
```
