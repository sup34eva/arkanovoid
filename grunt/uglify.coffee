module.exports =
	options:
		banner: '/*! <%= package.title || package.name %> - v<%= package.version %> -
			<%= grunt.template.today("dd/mm/yyyy") %> -
			<%= package.homepage %>\n
			* Copyright (c) <%= package.author %> <%= grunt.template.today("yyyy") %>;
			 Licensed <%= package.license %> */\n'
		beautify: true
		mangle: false
	dist:
		files:
			'dist/js/<%= package.name %>.min.js': '.tmp/<%= package.name %>.js'
