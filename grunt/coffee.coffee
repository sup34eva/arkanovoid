module.exports =
	options:
		join: true
		sourceMap: true
		banner: "/*! <%= package.title || package.name %> - v<%= package.version %> -
			<%= grunt.template.today(\"dd/mm/yyyy\") %> -
			<%= package.homepage %>\n
			* Copyright (c) <%= package.author %> <%= grunt.template.today(\"yyyy\") %>;
			Licensed <%= package.license %> */\n"
	dist:
		src: [
			'coffee/ball.coffee'
			'coffee/paddle.coffee'
			'coffee/brick.coffee'
			'coffee/init.coffee'
			'coffee/draw.coffee'
			'coffee/server.coffee'
			'coffee/fullscreen.coffee'
			'coffee/events.coffee'
		]
		dest: 'dist/js/<%= package.name %>.js'
	'dist/js/shop/js' : 'coffee/shop.coffee'
