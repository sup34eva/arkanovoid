class Brick
	constructor: (two, @x, @y) ->
		@rectangle = two.makeRectangle percent(@x*12.5, two.width) + two.width/16, percent(@y*10, two.height) + two.height/20, two.width/8, two.height/10
		@rectangle.visible = false
		return
	type: (t) ->
		@rectangle.visible = t > 0
		@rectangle.fill = ['#000', '#fff', '#a8a8a8', '#696767', '#ff0'][t]
		return
