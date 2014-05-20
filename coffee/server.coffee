server = document.querySelector "embed"
server.addEventListener "message", ((e) ->
	if typeof e.data is 'object'
		game = e.data
		ball.move game.x, game.y
		paddle.move game.pos, game.size
		serverStats.update()
	else if typeof e.data is 'string'
		data = e.data.split ','
		bricks[data[0]][data[1]].type data[2]
		console.log data
	else if typeof e.data is 'number'
		if e.data is 1
			alert 'You win !'
		else
			alert 'You loose !'
		window.location.reload()
	else
		console.warning 'Weird data', e.data
	return
), false
