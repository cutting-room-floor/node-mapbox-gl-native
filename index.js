var mbgl = require('bindings')('mbgl')

var x = 0,
    y = 0,
    z = 0;

mbgl.renderTile(x, y, z, function(tileImage) {
    console.log(tileImage);
});

