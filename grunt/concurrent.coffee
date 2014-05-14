module.exports =
	options:
		limit: 4
	build: ['make', 'coffeescript', 'imagemin', 'stylus']
	heroku: ['coffeescript', 'imagemin', 'stylus']
	test: ['coffeelint', 'jsonlint', 'cpplint']
