'use strict';

/* jshint node:true */

var test = require('tape');
var mbgl = require('../..');
var fs = require('fs');
var path = require('path');

var suitePath = path.dirname(require.resolve('mapbox-gl-test-suite/package.json'));

console.log(suitePath);

function renderTest(style, info, dir, key) {
    return function (t) {
        var completed = 0;
        var remaining = 10;
        var start = +new Date;

        var fileSource = new mbgl.FileSource();
        fileSource.request = function(req) {
            fs.readFile(path.join(suitePath, decodeURIComponent(req.url)), function(err, data) {
                req.respond(err, { data: data });
                t.error(err);
            });
        };
        fileSource.cancel = function() {};

        var view = new mbgl.View({
            ratio: 1.0,
            width: 512,
            height: 512
        });

        var map = new mbgl.Map(fileSource, view);
        map.load(style);

        function render() {
            map.render(info[key], function(err, image) {
                t.error(err);

                t.ok(true, 'render @ ' + ((+new Date) - start) + 'ms');
                if (++completed === remaining) {
                    map.release();
                    t.end();
                } else {
                    render();
                }
            });
        }

        render();
    };
}

function rewriteLocalSchema(uri) {
    return uri.replace(/^local:\/\//, '');
}

test('Concurrency', function(t) {
    var dir = 'line-join';
    var k = 'round';

    var style = require(path.join(suitePath, 'tests', dir, 'style.json')),
        info  = require(path.join(suitePath, 'tests', dir, 'info.json'));

    for (var k in style.sources) {
        var source = style.sources[k];

        if (source.tiles) {
            source.tiles = source.tiles.map(rewriteLocalSchema);
        }

        if (source.url) {
            source.url = rewriteLocalSchema(source.url);
        }
    }

    if (style.sprite) style.sprite = rewriteLocalSchema(style.sprite);
    if (style.glyphs) style.glyphs = rewriteLocalSchema(style.glyphs);

    style = JSON.stringify(style);

    for (k in info) {
        t.test(dir + ' ' + k, renderTest(style, info, dir, k));
    }

    t.end();
});
