module.exports = {
	options: {
		limit: 4
	},
	build: ['make', 'coffeescript', 'imagemin', 'stylus'],
	test: ['coffeelint', 'jsonlint']
};
