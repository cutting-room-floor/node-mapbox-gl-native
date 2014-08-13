# node-mapbox-gl-native

A library that renders map tiles in OpenGL using [mapbox-gl-native](https://github.com/mapbox/mapbox-gl-native).

[![Build Status](https://secure.travis-ci.org/mapbox/node-mapbox-gl-native.png)](https://travis-ci.org/mapbox/node-mapbox-gl-native)

## API
```
var mbgl = require('mapbox-gl-native');

mbgl.renderTile(0, 0, 0, function(err, tile) {
  // Do stuff with tile
});
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

```
npm install --build-from-source
```
