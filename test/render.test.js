'use strict';

/* jshint node:true */

var test = require('tape').test;
var mbgl = require('../index.js');
var fs = require('fs');
var path = require('path');
var util = require('util');
var child_process = require('child_process');
var http = require('http');
var mkdirp = require('mkdirp');

var suitePath = path.dirname(require.resolve('mapbox-gl-test-suite/package.json'));

function startFixtureServer(callback) {
    var e = null;
    var p = child_process.spawn(path.join(suitePath, 'bin/server.py'));
    p.stdout.on('data', function(data) {
        //console.log('STDOUT ' + data.toString());
    });
    p.stderr.on('data', function(data) {
        data = data.toString().split('\n').forEach(function(l) {
            if (!/^127\.0\.0\.1.+\"GET\s.+\sHTTP\/1\.1\"\s200\s.+$/.test(l) && l.lenggth > 0)  {
                console.error(l);
            }
        });
    });

    // TODO how to determine it's ready?
    setTimeout(function() {
        callback(e, p);
    }, 100);
}

function renderTest(style, info, dir) {
    return function (t) {
        var watchdog = setTimeout(function() {
            t.fail('timed out after 4 seconds');
            t.end();
        }, 4000);

        t.once('end', function() {
            clearTimeout(watchdog);
        });

        var map = new mbgl.Map();
        map.load(style, info, function(err) {
            t.ifError(err);
            map.render(function(err, image) {
                mkdirp.sync(dir);

                fs.writeFile(path.join(dir, process.env.UPDATE ? 'expected.png' : 'actual.png'), image, function(err) {
                    if (err) t.fail(err);
                    t.pass('generated image async');
                    t.end();
                });
            });
        });
    };
}

startFixtureServer(function(err, p) {
    if (err) throw err;

    function rewriteLocalSchema(uri) {
        return uri.replace(/^local:\/\//, 'http://127.0.0.1:2900/');
    }

    var tests = fs.readdirSync(path.join(suitePath, 'tests')).filter(function(v) {
        return v !== 'index.html';
    }).forEach(function(dir) {
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
    });

    test('cleanup', function(t) {
        p.kill();
        t.end();
    });
});
