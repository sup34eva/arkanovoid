module.exports =
	options:
		bare: true
		join: true
		sourceMap: true
		banner: "/*! <%= package.title || package.name %> - v<%= package.version %> -
			<%= grunt.template.today(\"dd/mm/yyyy\") %> -
			<%= package.homepage %>\n
			* Copyright (c) <%= package.author %> <%= grunt.template.today(\"yyyy\") %>;
			Licensed <%= package.license %> */\n"
	dist:
		expand: true
		flatten: true
		src: ['coffee/**/*.coffee']
		dest: 'dist/js/'
		ext: '.js'
