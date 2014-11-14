'use strict';

/* jshint node:true */

var test = require('tape').test;
var mbgl = require('../index.js');
var fs = require('fs');
var path = require('path');
var child_process = require('child_process');

var suitePath = path.dirname(require.resolve('mapbox-gl-test-suite/package.json'));

function startFixtureServer(callback) {
    var e = null;
    var p = child_process.spawn(path.join(suitePath, 'bin/server.py'), [2800]);
    p.stderr.on('data', function(data) {
        data = data.toString().split('\n').forEach(function(l) {
            if (!/^127\.0\.0\.1.+\"GET\s.+\sHTTP\/1\.1\"\s200\s.+$/.test(l) && l.length > 0)  {
                console.error(l);
            }
        });
    });

    callback(e, p);
}

function renderTest(style, info, dir) {
    return function (t) {
        var remaining = 2;
        t.plan = remaining * 3;
        var start = +new Date;
        var map = new mbgl.Map();
        map.load(style);
        for (var i = 0; i < remaining; i++) {
            map.render(info, function(err, image) {
                t.error(err);
                t.equal(image.length, 24679);
                t.ok(true, 'x10 render @ ' + ((+new Date) - start) + 'ms');
            });
        }
    };
}

startFixtureServer(function(err, p) {
    if (err) throw err;

    function rewriteLocalSchema(uri) {
        return uri.replace(/^local:\/\//, 'http://127.0.0.1:2800/');
    }

    var dir = 'line-join';
    var key = 'round';

    var style = require(path.join(suitePath, 'tests', dir, 'style.json')),
        info  = require(path.join(suitePath, 'tests', dir, 'info.json'));

    for (var k in style.sources) {
        if (style.sources[k].tiles)
            style.sources[k].tiles = style.sources[k].tiles.map(rewriteLocalSchema);
    }

    if (style.sprite) style.sprite = rewriteLocalSchema(style.sprite);
    if (style.glyphs) style.glyphs = rewriteLocalSchema(style.glyphs);

    for (var key in info) {
        test(dir + ' ' + k, renderTest(style, info[key], path.join(suitePath, 'tests', dir, key)));
    }

    test('cleanup', function(t) {
        p.kill();
        t.end();
    });
});
