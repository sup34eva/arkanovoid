module.exports =
	files: [
		'server/*.{c,h}'
	]
	reporter: 'spec'
	verbosity: 1
	filters:
		'whitespace':
			'tab': false
		'readability':
			'todo': false
		'build':
			'include': false
		'runtime':
			'threadsafe_fn': false
