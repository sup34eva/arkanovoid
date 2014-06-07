module.exports =
	build: ['make', 'coffeescript', 'imagemin', 'stylus', 'copy:img']
	test: ['coffeelint', 'jsonlint', 'cpplint']
