module.exports =
	server:
		files: [
			expand: true
			flatten: true
			src: ['server/newlib/Release/server_*.nexe', 'server/newlib/Release/server.nmf']
			dest: 'dist/server/'
		]
	img:
		files: [
			expand: true
			flatten: true
			src: ['img/**/*.{svg,raw,tex}']
			dest: 'dist/img/'
		]
