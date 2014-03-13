module.exports = {
	dynamic: {
		files: [{
			expand: true,
			cwd: 'img/',
			src: ['**/*.{png,jpg,gif}'],
			dest: 'dist/img'
		}]
	}
};
