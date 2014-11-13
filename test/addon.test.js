'use strict';

/* jshint node:true */

var test = require('tape');
var mbgl = require('../index.js');

test('single', function(t) {
    var map = new mbgl.Map();
    map.set(10);
    map.add(1, function(err, result) {
        t.error(err);
        t.equal(result, 11);
        t.end();
    });
});

test('multiple', function(t) {
    var remaining = 5;
    t.plan(remaining * 2);

    for (var i = 0; i < remaining; i++) {
        var map = new mbgl.Map();
        map.set(i);
        map.add(i, (function(i) {
            return function(err, result) {
                t.error(err);
                t.equal(result, i * 2);
            };
        })(i));
    }
});

test('concurrent', function(t) {
    var remaining = 5;
    t.plan(remaining * 3);

    var map = new mbgl.Map();
    for (var i = 0; i < remaining; i++) {
        map.set(i);
        map.add(i, (function(i) {
            return function(err, result) {
                t.error(err);
                t.skip(result);
                t.skip(i * 2);
            };
        })(i));
    }
});
