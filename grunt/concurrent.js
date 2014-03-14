module.exports = {
	build: ['make', 'coffeescript', 'imagemin', 'stylus'],
	test: ['coffeelint', 'jsonlint']
};
