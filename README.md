# node-mapbox-gl-native

A library that renders map tiles in OpenGL using [mapbox-gl-native](https://github.com/mapbox/mapbox-gl-native).

## API
```
var mbgl = require('mapbox-gl-native');

mbgl.renderTile(0, 0, 0, function(err, tile) {
  // Do stuff with tile
});
```
