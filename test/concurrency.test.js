'use strict';

/* jshint node:true */

var test = require('tape').test;
var mbgl = require('../index.js');
var fs = require('fs');
var path = require('path');
var http = require('http');
var st = require('st');

var suitePath = path.dirname(require.resolve('mapbox-gl-test-suite/package.json'));
var server = http.createServer(st({path: suitePath}));

function startFixtureServer(callback) {
    server.listen(0, function(err) {
        callback(err, err ? null : server.address().port);
    });
}

function renderTest(style, info, dir) {
    return function (t) {
        var remaining = 10;
        t.plan(remaining * 2);
        var start = +new Date;
        var map = new mbgl.Map();
        map.load(style);
        for (var i = 0; i < remaining; i++) {
            map.render(info, function(err, image) {
                t.error(err);
                t.ok(true, 'x10 render @ ' + ((+new Date) - start) + 'ms');
            });
        }
    };
}

startFixtureServer(function(err, port) {
    if (err) throw err;

    function rewriteLocalSchema(uri) {
        return uri.replace(/^local:\/\//, 'http://127.0.0.1:'+ port +'/');
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

    style = JSON.stringify(style);

    for (var key in info) {
        test(dir + ' ' + k, renderTest(style, info[key], path.join(suitePath, 'tests', dir, key)));
    }

    test('cleanup', function(t) {
        server.close();
        t.end();
    });
});
