module.exports =
	options:
		limit: 4
	build: ['make', 'coffeescript', 'imagemin', 'stylus', 'copy:img']
	test: ['coffeelint', 'jsonlint', 'cpplint']
