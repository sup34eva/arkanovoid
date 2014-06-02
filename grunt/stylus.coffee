module.exports =
	compile:
		options:
			urlfunc: 'embedurl'
		files: [
			expand: true
			flatten: true
			src: 'styles/**/*.styl'
			dest: 'dist/css/'
			ext: '.css'
		]