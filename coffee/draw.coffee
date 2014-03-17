two.bind "update", (frameCount) ->
	if not (game is null)
		ball.move game.x, game.y
		paddle.move game.pos, game.size
		i = 0
		while i < 8
			j = 0
			while j < 8
				bricks[i][j].type game.brick[i][j]
				j++
			i++
	server.postMessage two.timeDelta/1000
	return

