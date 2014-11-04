'use strict';

/* jshint node:true */

var test = require('tape').test;
var mbgl = require('../index.js');
var fs = require('fs');
var path = require('path');
var http = require('http');
var mkdirp = require('mkdirp');
var suitePath = path.dirname(require.resolve('mapbox-gl-test-suite/package.json'));

var dir = 'line-join';
var key = 'round';
var style = require(path.join(suitePath, 'tests', dir, 'style.json')),
    info  = require(path.join(suitePath, 'tests', dir, 'info.json'));

for (var k in style.sources) {
    for (var l in style.sources[k].tiles) {
        style.sources[k].tiles[l] = style.sources[k].tiles[l].replace(/^local:\/\//, '');
    }
}

if (style.sprite) {
    style.sprite = style.sprite.replace(/^local:\/\//, '');
}

if (style.glyphs) {
    style.glyphs = style.glyphs.replace(/^local:\/\//, '');
}

test(dir + ' ' + key, renderTest(style, info[key], path.join(suitePath, 'tests', dir, key)));

function renderTest(style, info, dir) {
    return function (t) {
        var remaining = 2;
        var start = +new Date;
        var map = new mbgl.Map();
        console.log(map);
        for (var i = 0; i < remaining; i++) {
            map.load(style, info, suitePath + '/', function(err) {
                t.ifError(err);
                map.render(function(err, image) {
                    t.ifError(err);
                    t.equal(image.length, 24679);
                    t.ok(true, 'x10 render @ ' + ((+new Date) - start) + 'ms');
                    if (!--remaining) t.end();
                });
            });
        }
    };
}

