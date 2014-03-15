game = null
two = new Two(
	width: 960
	height: 960
	autostart: true
	type: Two.Types.webgl
	fullscreen: true
).appendTo(viewport)
ball = new Ball(two)
paddle = new Paddle(two)
bricks = []
i = 0
while i < 8
	bricks[i] = []
	j = 0
	while j < 8
		bricks[i][j] = new Brick(two, j, i)
		j++
	i++
