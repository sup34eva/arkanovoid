server = document.querySelector("embed")
lastState = null
server.addEventListener "message", ((e) ->
	game = e.data
	if game isnt null and lastState isnt null
		if game.x isnt lastState.x or game.y isnt lastState.y
			ball.move game.x, game.y
		if game.pos isnt lastState.pos or game.size isnt lastState.size
			paddle.move game.pos, game.size
		if game.brick
			i = 0
			while i < 8
					j = 0
					while j < 10
						bricks[i][j].type game.brick[(i * 8) + j]
						j++
				i++
		if(game.win)
			alert 'You win !'
	lastState = game
	serverStats.update()
	if typeof game is 'number'
		console.log game
	return
), false
