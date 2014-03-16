module.exports =
	options:
		files: ['package.json', 'bower.json']
		updateConfigs: ['package']
		commit: true
		commitMessage: 'Version %VERSION%'
		commitFiles: ['package.json']
		createTag: true
		tagName: 'v%VERSION%'
		tagMessage: 'Version %VERSION%'
		push: true
		pushTo: 'master'
		gitDescribeOptions: '--tags --always --abbrev=1 --dirty=-d'
