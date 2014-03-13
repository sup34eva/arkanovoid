module.exports = {
	server: {
		files: [
			{
				expand: true,
				src: ['server/server.pexe', 'server/server.nmf'],
				dest: 'dist/'
			}
		]
	},
	img: {
		files: [
			{
				expand: true,
				src: ['img/*.{png,jpg,gif}'],
				dest: 'dist/'
			}
		]
	}
};
