var binary = require('node-pre-gyp');
var path = require('path');
var binding_path = binary.find(path.resolve(path.join(__dirname,'./package.json')));
module.exports = require(binding_path);

module.exports.Map = function() {
};

module.exports.Map.prototype.load = function(style) {
    this.style = style;
    return this;
};

module.exports.Map.prototype.render = function(options, callback) {
    module.exports.render(this.style, options, callback);
};
