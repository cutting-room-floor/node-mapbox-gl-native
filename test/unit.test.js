'use strict';

/* jshint node:true */

var test = require('tape').test;
var mbgl = require('../index.js');
var fs = require('fs');
var path = require('path');
var http = require('http');
var suitePath = path.dirname(require.resolve('mapbox-gl-test-suite/package.json'));

test('missing arguments', function(t) {
    var err;
    try {
        mbgl.render();
    } catch(e) {
        err = e;
    }
    t.ok(err instanceof Error, 'Throws error');
    t.equal(err.message, 'Too few arguments', 'Returns message');
    t.end();
});

test('bad style', function(t) {
    var err;
    try {
        mbgl.render(null, null, null, null);
    } catch(e) {
        err = e;
    }
    t.ok(err instanceof Error, 'Throws error');
    t.equal(err.message, 'first argument must be a style object', 'Returns message');
    t.end();
});

test('missing options', function(t) {
    var err;
    try {
        mbgl.render({}, null, null, null);
    } catch(e) {
        err = e;
    }
    t.ok(err instanceof Error, 'Throws error');
    t.equal(err.message, 'second argument must be an options object', 'Returns message');
    t.end();
});

test('missing path', function(t) {
    var err;
    try {
        mbgl.render({}, {}, null, null);
    } catch(e) {
        err = e;
    }
    t.ok(err instanceof Error, 'Throws error');
    t.equal(err ? err.message : '', 'third argument must be a string', 'Returns message');
    t.end();
});

test('empty', function(t) {
    mbgl.render({}, {}, '/', function(err, image) {
        t.error(err);
        t.equal(image.length, 4668);
        t.equal(image.toString().slice(1, 4), 'PNG');
        t.end();
    });
});
