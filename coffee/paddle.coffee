class Paddle
	constructor: (two) ->
		@height = percent(1, window.innerHeight)
		@y = window.innerHeight - @height
		@rectangle = two.makeRectangle 0, @y, window.innerWidth/100, @height
		@rectangle.fill = "#fff"
		return
	move: (pos, size) ->
		x = percent(size, window.innerWidth)/2
		@rectangle.translation.set percent(pos, window.innerWidth) + x, @y
		@rectangle.vertices[0].x = @rectangle.vertices[3].x = x
		@rectangle.vertices[1].x = @rectangle.vertices[2].x = -x
		return
