'use strict';

/* jshint node:true */

var test = require('tape').test;
var mbgl = require('../index.js');
var PNG = require('pngjs').PNG;
var fs = require('fs');
var path = require('path');
var http = require('http');
var mkdirp = require('mkdirp');

var suitePath = path.dirname(require.resolve('mapbox-gl-test-suite/package.json'));

function renderTest(style, info, dir) {
    return function (t) {
        var width = info.width || 512,
            height = info.height || 512;

        mbgl.renderTile(buffer.toString(), buffer, function(err, image) {
            if (!err) rendered();
            t.fail(err);
        });

        var watchdog = setTimeout(function() {
            t.fail('timed out after 4 seconds');
        }, 4000);

        t.once('end', function() {
            clearTimeout(watchdog);
        });

        function rendered() {
            for (var id in map.sources)
                if (!map.sources[id].loaded())
                    return;
            if (map.style.sprite && !map.style.sprite.loaded())
                return;

            map.off('render', rendered);

            var png = new PNG({width: width, height: height});

            gl.readPixels(0, 0, width, height, gl.RGBA, gl.UNSIGNED_BYTE, png.data);

            // Flip the scanlines.
            var stride = width * 4;
            var tmp = new Buffer(stride);
            for (var i = 0, j = height - 1; i < j; i++, j--) {
                var start = i * stride;
                var end = j * stride;
                png.data.copy(tmp, 0, start, start + stride);
                png.data.copy(png.data, start, end, end + stride);
                tmp.copy(png.data, end);
            }

            mkdirp.sync(dir);

            png.pack()
                .pipe(fs.createWriteStream(path.join(dir, process.env.UPDATE ? 'expected.png' : 'actual.png')))
                .on('finish', t.end);
        }
    };
}

fs.readdirSync(path.join(suitePath, 'tests')).forEach(function(dir) {
    if (dir === 'index.html') return;

    var style = require(path.join(suitePath, 'tests', dir, 'style.json')),
        info  = require(path.join(suitePath, 'tests', dir, 'info.json'));

    for (var k in style.sources) {
        for (var l in style.sources[k].tiles) {
            style.sources[k].tiles[l] = style.sources[k].tiles[l].replace(/^local:\/\//, 'http://localhost:2900/');
        }
    }

    if (style.sprite) {
        style.sprite = style.sprite.replace(/^local:\/\//, 'http://localhost:2900/');
    }

    if (style.glyphs) {
        style.glyphs = style.glyphs.replace(/^local:\/\//, 'http://localhost:2900/');
    }

    for (k in info) {
        (info[k].js === false ? test.skip : test)(dir + ' ' + k, renderTest(style, info[k], path.join(suitePath, 'tests', dir, k)));
    }
});
