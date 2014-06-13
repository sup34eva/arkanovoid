module.exports =
	build: ['make', 'coffeescript', 'imagemin', 'stylus', 'copy:img', 'png2tex']
	test: ['coffeelint', 'jsonlint', 'cpplint']
