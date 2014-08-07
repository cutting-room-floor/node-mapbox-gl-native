var mbgl = require('bindings')('mapbox-gl-native')

mbgl.renderTile(0, 0, 0, function(err, tile) {
    console.log(tile);
});
