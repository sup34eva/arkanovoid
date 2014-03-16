module.exports =
	options:
		join: true
		sourceMap: true
	dist:
		src: [
			'coffee/util.coffee'
			'coffee/ball.coffee'
			'coffee/paddle.coffee'
			'coffee/brick.coffee'
			'coffee/init.coffee'
			'coffee/draw.coffee'
			'coffee/server.coffee'
			'coffee/events.coffee'
		]
		dest: '.tmp/<%= package.name %>.js'
	worker:
		src: [
			'coffee/util.coffee'
			'coffee/worker.coffee'
		]
		dest: 'dist/js/worker.js'
