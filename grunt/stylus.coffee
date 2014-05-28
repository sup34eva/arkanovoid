module.exports =
	compile:
		options:
			urlfunc: 'embedurl'
		files: [
			expand: true
			src: 'styles/**/*.styl'
			dest: 'dist/css/'
			ext: '.css'
		]