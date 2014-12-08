'use strict';

/* jshint node:true */

var test = require('tape').test;
var mbgl = require('../index.js');

test('missing load arguments', function(t) {
    var map = new mbgl.Map();
    t.throws(function() {
        map.load();
    }, /Wrong number of arguments/, 'throws error');
    t.end();
});

test('bad style', function(t) {
    var map = new mbgl.Map();
    t.throws(function() {
        map.load(null);
    }, /First argument must be a style object/, 'throws error');
    t.end();
});

test('good load args', function(t) {
    var map = new mbgl.Map();
    t.doesNotThrow(function() {
        map.load({});
    }, 'does not throw error');
    t.end();
});

test('missing render arguments', function(t) {
    var map = new mbgl.Map();
    t.throws(function() {
        map.render();
    }, /Wrong number of arguments/, 'throws error');
    t.end();
});

test('bad render options', function(t) {
    var map = new mbgl.Map();
    t.throws(function() {
        map.render(null, function() {});
    }, /First argument must be an options object/, 'throws error');
    t.end();
});

test('bad render callback', function(t) {
    var map = new mbgl.Map();
    t.throws(function() {
        map.render(null, null);
    }, /Callback must be a function/, 'throws error');
    t.end();
});

test('good render args', function(t) {
    var map = new mbgl.Map();
    // map.render will segfault if called before map.load
    map.load({});
    t.doesNotThrow(function() {
        map.render({}, function() {});
    }, 'does not throw error');
    t.end();
});

test('empty', function(t) {
    var map = new mbgl.Map();
    map.load({});
    map.render({}, function(err, image) {
        t.error(err);
        t.equal(image.length, 4668);
        t.equal(image.toString().slice(1, 4), 'PNG');
        t.end();
    });
});
