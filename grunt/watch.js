module.exports = {
	express: {
		files: ['main.js', 'routes.json'],
		tasks: ['express:server', 'notify:server'],
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
		files: ['stylus/**/*.styl', 'stylus/**/*.stylus'],
		tasks: ['stylus', 'notify:stylus'],
		options: {
			spawn: false,
			livereload: true
		}
	}
};
