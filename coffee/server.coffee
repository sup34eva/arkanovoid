server = document.querySelector "embed"
server.addEventListener "message", ((e) ->
	game = e.data
	ball.move game.x, game.y
	paddle.move game.pos, game.size
	for i in [0 ... 10]
		for j in [0 ... 8]
			bricks[i][j].type game.brick[(i * 8) + j]
	serverStats.update()
	return
), false
