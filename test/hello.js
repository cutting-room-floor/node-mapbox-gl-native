var mbgl = require('../index.js');
var test = require('tap').test;

test('hello', function(t) {
    t.plan(3);

    mbgl.renderTile(0, 0, 0, function(err, tile) {
        t.type(err, 'null', 'no error');
        t.type(tile, 'string', 'type of tile is string');
        t.equal(tile, 'Hello world.', 'tile should equal "Hello world."');
    });
});
