var mbgl = require('../index.js');
var test = require('tap').test;

test('hello', function(t) {
    t.plan(3);

    var buffer = new Buffer("buffer");

    mbgl.renderTile(buffer.toString(), buffer, function(err, image) {
        t.type(err, 'null', 'no error');
        t.type(image, 'string', 'type of image is string');
        t.equal(image, 'Hello world.', 'image should equal "Hello world."');
    });
});
