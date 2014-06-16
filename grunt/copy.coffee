module.exports =
	server:
		files: [
			expand: true
			flatten: true
			src: ['jeu/newlib/Release/server_*.nexe', 'jeu/newlib/Release/server.nmf']
			dest: 'dist/server/'
		]
	img:
		files: [
			expand: true
			flatten: true
			src: ['img/**/*.{svg,raw,tex}']
			dest: 'dist/img/'
		]
	hook:
		src: 'tools/pre-commit'
		dest: '.git/hooks/pre-commit'
