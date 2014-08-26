'use strict';

/* jshint node:true */

var test = require('tape').test;
var mbgl = require('../index.js');
var fs = require('fs');
var st = require('st');
var path = require('path');
var http = require('http');
var mkdirp = require('mkdirp');

var suitePath = path.dirname(require.resolve('mapbox-gl-test-suite/package.json')),
    server = http.createServer(st({path: suitePath}));

test('before render', function(t) {
    server.listen(2900, t.end);
});

function renderTest(style, info, dir) {
    return function (t) {
        mbgl.renderTile(JSON.stringify(style), JSON.stringify(info), function(err, image) {
            if (err) t.fail(err);

            mkdirp.sync(dir);

            fs.writeFile(path.join(dir, process.env.UPDATE ? 'expected.png' : 'actual.png'), image, function(err) {
                if (err) t.fail(err);
                t.end();
            });
        });

        var watchdog = setTimeout(function() {
            t.fail('timed out after 4 seconds');
        }, 4000);

        t.once('end', function() {
            clearTimeout(watchdog);
        });
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
        test(dir + ' ' + k, renderTest(style, info[k], path.join(suitePath, 'tests', dir, k)));
    }
});

test('after render', function(t) {
    server.close(t.end);
});
