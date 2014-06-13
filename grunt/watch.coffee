module.exports =
	express:
		files: ['*.{coffee,json}']
		tasks: ['express:server', 'notify:express']
		options:
			spawn: false
			livereload: 35729
	jade:
		files: ['*.{jade}']
		tasks: ['notify:jade']
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
		files: ['img/**/*.{png,jpg,gif,svg}']
		tasks: ['imagemin', 'copy:img', 'notify:img']
		options:
			spawn: false
			livereload: 35729
	tex:
		files: ['img/textures/**/*.png}']
		tasks: ['png2tex', 'notify:tex']
		options:
			spawn: false
			livereload: 35729
	server:
		files: ['server/**/*.{c,cc,cpp,h}']
		tasks: ['make', 'notify:server']
		options:
			spawn: false
			livereload: 35729
