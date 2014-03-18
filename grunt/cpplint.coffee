module.exports =
	files: [
		'server/*.cc'
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
