module.exports = {
	compile: {
		options: {
			urlfunc: 'embedurl'
		},
		files: {
			'dist/css/style.css': ['styles/**/*.styl', 'styles/**/*.stylus']
		}
	}
}
