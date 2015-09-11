#!/usr/bin/env node
/* jshint node: true, unused: false */
'use strict';

var mbgl = require('..');
var fs = require('fs');
var path = require('path');

var base = path.join(path.dirname(process.mainModule.filename), '../test');

var map = new mbgl.Map({
    request: function(req) {
        fs.readFile(path.join(base, req.url), function(err, data) {
            req.respond(err, { data: data });
        });
    }
});

map.load(require('../test/fixtures/style.json'));

map.render({}, function(err, data) {
    if (err) throw err;

    var wstream = fs.createWriteStream('image.png');
    wstream.on('finish', function() {
        console.warn('Written image.png');
    });

    wstream.write(data.pixels);
    wstream.end();
});
