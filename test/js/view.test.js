'use strict';

/* jshint node: true */

var test = require('tape');
var mbgl = require('../..');

test('View', function(t) {
    t.test('constructor', function(t) {
        t.test('must be called with new', function(t) {
            t.throws(function() {
                mbgl.View();
            }, /Use the new operator to create new View objects/);

            t.end();
        });

        t.test('should require an options object as first parameter', function(t) {
            t.throws(function() {
                new mbgl.View();
            }, /Requires an options object as first argument/);

            t.throws(function() {
                new mbgl.View('options');
            }, /Requires an options object as first argument/);

            t.end();
        });

        t.test('should require the options object to have pixel ratio, width and height attributes', function(t) {
            var options = {};

            t.throws(function() {
                new mbgl.View(options);
            }, /must have a numerical ratio attribute/);

            options.ratio = 'test';
            t.throws(function() {
                new mbgl.View(options);
            }, /must have a numerical ratio attribute/);

            options.ratio = 1.0;
            t.throws(function() {
                new mbgl.View(options);
            }, /must have a numerical width attribute/);

            options.width = 'test';
            t.throws(function() {
                new mbgl.View(options);
            }, /must have a numerical width attribute/);

            options.width = 512;
            t.throws(function() {
                new mbgl.View(options);
            }, /must have a numerical height attribute/);

            options.height = 'test';
            t.throws(function() {
                new mbgl.View(options);
            }, /must have a numerical height attribute/);

            options.height = 512;
            t.doesNotThrow(function() {
                new mbgl.View(options);
            });

            t.end();
        });

        t.end();
    });

    t.test('can be passed to Map constructor', function(t) {
        var fileSource = new mbgl.FileSource();
        fileSource.request = function() {};
        fileSource.cancel = function() {};

        var view = new mbgl.View({ ratio: 1.0, width: 512, height: 512 });


        t.doesNotThrow(function() {
            new mbgl.Map(fileSource, view);
        });

        t.end();
    });

    t.end();
});
