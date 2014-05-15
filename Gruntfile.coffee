module.exports = (grunt) ->
	require('time-grunt') grunt
	grunt.loadNpmTasks 'node-cpplint'
	require('load-grunt-config') grunt
	grunt.registerTask 'heroku:development', 'build'
