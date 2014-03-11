class Paddle
  constructor: (two) ->
    @rectangle = two.makeRectangle 0, two.height-1, two.width/10, two.height/100
    @rectangle.fill = "#fff"
  move: (pos, size) ->
    @rectangle.translation.set pos, two.height - 1
    #@rectangle.width = size

