module.exports = {
	express: {
		files: ['*.{js,json}'],
		tasks: ['express:server', 'notify:express'],
		options: {
			spawn: false,
			livereload: true
		}
	},
	coffee: {
		files: ['coffee/**/*.coffee'],
		tasks: ['coffeescript', 'notify:coffee'],
		options: {
			spawn: false,
			livereload: true
		}
	},
	style: {
		files: ['styles/**/*.{styl,stylus}'],
		tasks: ['stylus', 'notify:stylus'],
		options: {
			spawn: false,
			livereload: true
		}
	},
	img: {
		files: ['img/**/*.{png,jpg,gif}'],
		tasks: ['imagemin', 'notify:img'],
		options: {
			spawn: false,
			livereload: true
		}
	},
	server: {
		files: ['server/**/*.{c,cc,cpp}'],
		tasks: ['make', 'notify:server'],
		options: {
			spawn: false,
			livereload: true
		}
	}
};
