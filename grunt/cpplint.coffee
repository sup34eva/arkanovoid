module.exports =
	files: [
		'jeu/**/*.{cc,c,h}'
	]
	reporter: 'spec'
	verbosity: 1
	filters:
		'whitespace':
			'tab': false
			'labels': false
		'readability':
			'todo': false
			'casting':false
		'build':
			'include': false
		'runtime':
			'threadsafe_fn': false
