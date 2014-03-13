module.exports = {
	main: {
		files: [
			{
				expand: true,
				src: ['server/server.pexe', 'server/server.nmf'],
				dest: 'dist/'
			}
		]
	}
};
