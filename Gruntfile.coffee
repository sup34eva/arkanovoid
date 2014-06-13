module.exports = (grunt) ->
	require('time-grunt') grunt
	grunt.loadNpmTasks 'node-cpplint'

	grunt.registerMultiTask 'png2tex', 'Convert PNG images to Tabasco textures', ->
		done = @async()
		files = []
		cwd = ""
		@files.forEach (file, i) =>
			file.src.forEach (src) =>
				arr = src.split('/')
				child = grunt.util.spawn
					cmd: "png2tex"
					args: ['.\\' + arr[arr.length - 1]]
					opts:
						cwd: process.cwd() + '\\' + file.orig.cwd.replace('/', '\\')
						stdio: 'inherit'
				, (error, result, code) =>
					grunt.file.copy src.replace('png', 'tex'), file.dest.replace('png', 'tex')
					grunt.file.delete src.replace('png', 'tex')
					if i is @files.length - 1
						done()

	require('load-grunt-config') grunt
	grunt.registerTask 'heroku:development', 'build'
	grunt.registerTask 'nacl:update', 'shell:update'
	grunt.registerTask 'nacl:install', ['shell:chmod', 'nacl:update']
