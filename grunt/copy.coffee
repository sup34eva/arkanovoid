module.exports =
	server:
		files: [
			expand: true
			flatten: true
			src: ['jeu/newlib/Release/jeu_*.nexe', 'jeu/newlib/Release/jeu.nmf']
			dest: 'dist/jeu/'
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
