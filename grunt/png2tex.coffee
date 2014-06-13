module.exports =
	textures:
		files: [{
			expand: true
			cwd: 'img/textures/'
			src: ['**/*.png']
			dest: 'dist/img/textures'
		}]
