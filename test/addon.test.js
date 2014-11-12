'use strict';

/* jshint node:true */

var test = require('tape');
var addon = require('../addon.js');

test('single', function(t) {
  var obj = new addon.MyObject();
  obj.set(10);
  obj.add(1, function(err, result) {
    t.ifError(err);
    t.equal(result, 11);
    t.end();
  });
});

test('multiple', function(t) {
  var remaining = 5;
  t.plan(remaining * 2);

  for (var i = 0; i < remaining; i++) {
    var obj = new addon.MyObject();
    obj.set(i);
    obj.add(i, (function(i) {
      return function(err, result) {
        t.ifError(err);
        t.equal(result, i * 2);
      };
    })(i));
  }
});

test('concurrent', function(t) {
  var remaining = 5;
  t.plan(remaining * 3);

  var obj = new addon.MyObject();
  for (var i = 0; i < remaining; i++) {
    obj.set(i);
    obj.add(i, (function(i) {
      return function(err, result) {
        t.ifError(err);
        t.skip(result);
        t.skip(i * 2);
      };
    })(i));
  }
});
