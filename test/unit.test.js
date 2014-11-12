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
    t.equal(err.message, 'Wrong number of arguments', 'Returns message');
    t.end();
});

test('bad callback', function(t) {
    var err;
    try {
        mbgl.render(null, null, null);
    } catch(e) {
        err = e;
    }
    t.ok(err instanceof Error, 'Throws error');
    t.equal(err.message, 'Callback must be a function', 'Returns message');
    t.end();
});

test('bad style', function(t) {
    mbgl.render(null, null, function(err, image){
        t.ok(err instanceof Error, 'Throws error');
        t.equal(err.message, 'First argument must be a style object', 'Returns message');
        t.end();
    });
});

test('missing options', function(t) {
    mbgl.render({}, null, function(err, image){
        t.ok(err instanceof Error, 'Throws error');
        t.equal(err.message, 'Second argument must be an options object', 'Returns message');
        t.end();
    });
});

test('empty', function(t) {
    mbgl.render({}, {}, function(err, image) {
        t.error(err);
        t.equal(image.length, 4668);
        t.equal(image.toString().slice(1, 4), 'PNG');
        t.end();
    });
});

test('api', function(t) {
    var map = new mbgl.Map();
    map.load({});
    map.render({}, function(err, image) {
        t.error(err);
        t.equal(image.length, 4668);
        t.equal(image.toString().slice(1, 4), 'PNG');
        t.end();
    });
});
