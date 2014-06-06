module.exports =
	server:
		files: [
			expand: true
			src: ['server/server.pexe', 'server/server.nmf']
			dest: 'dist/'
		]
	img:
		files: [
			expand: true
			flatten: true
			src: ['img/**/*.svg']
			dest: 'dist/img/'
		]