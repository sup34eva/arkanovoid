module.exports =
	express:
		files: ['*.{coffee,json}']
		tasks: ['express:server', 'notify:express']
		options:
			spawn: false
			livereload: 35729
	coffee:
		files: ['coffee/**/*.coffee']
		tasks: ['coffeescript', 'notify:coffee']
		options:
			spawn: false
			livereload: 35729
	style:
		files: ['styles/**/*.{styl,stylus}']
		tasks: ['stylus', 'notify:stylus']
		options:
			spawn: false
			livereload: 35729
	img:
		files: ['img/**/*.{png,jpg,gif}']
		tasks: ['imagemin', 'notify:img']
		options:
			spawn: false
			livereload: 35729
	server:
		files: ['server/**/*.{c,cc,cpp,h}']
		tasks: ['make', 'notify:server']
		options:
			spawn: false
			livereload: 35729
