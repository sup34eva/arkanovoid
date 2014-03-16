module.exports =
	dist:
		files:
			src: ['coffee/*.coffee']
		reporter: require('coffeelint-stylish').reporter
		options:
			'no_tabs':
				'level': 'ignore'
			'max_line_length':
				'level': 'warn'
				'value': 130
			indentation:
				'level': 'warn'
				'value': 1
