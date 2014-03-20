server = document.querySelector("embed")
server.addEventListener "message", ((e) ->
	game = e.data
	if game isnt null
		ball.move game.x, game.y
		paddle.move game.pos, game.size
		if game.brick isnt null
			i = 0
			while i < 8
				j = 0
				while j < 10
					bricks[i][j].type game.brick[(i * 8) + j]
					j++
				i++
		if game.win
			alert 'You win !'
	serverStats.update()
	return
), false
