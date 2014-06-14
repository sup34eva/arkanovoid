module.exports =
	dynamic:
		files: [{
			expand: true
			cwd: 'img/'
			src: ['**/*.{png,jpg,gif}', '!**/textures/**']
			dest: 'dist/img'
		}]
