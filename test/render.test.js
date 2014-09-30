'use strict';

/* jshint node:true */

var test = require('tape').test;
var mbgl = require('../index.js');
var fs = require('fs');
var path = require('path');
var http = require('http');
var mkdirp = require('mkdirp');

var suitePath = path.dirname(require.resolve('mapbox-gl-test-suite/package.json'));

function renderTest(style, info, dir) {
    return function (t) {
        var watchdog = setTimeout(function() {
            t.fail('timed out after 4 seconds');
            t.end();
        }, 4000);

        t.once('end', function() {
            clearTimeout(watchdog);
        });

        mbgl.render(JSON.stringify(style), JSON.stringify(info), suitePath + '/', function(err, image) {
            mkdirp.sync(dir);

            fs.writeFile(path.join(dir, process.env.UPDATE ? 'expected.png' : 'actual.png'), image, function(err) {
                if (err) t.fail(err);
                t.pass('generated image async');
                t.end();
            });
        });
    };
}

fs.readdirSync(path.join(suitePath, 'tests')).forEach(function(dir) {
    if (dir === 'index.html') return;

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

    for (var k in info) {
        test(dir + ' ' + k, renderTest(style, info[k], path.join(suitePath, 'tests', dir, k)));
    }
});
