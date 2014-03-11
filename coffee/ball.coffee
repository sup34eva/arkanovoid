class Ball
  constructor: (two) ->
    @circle = two.makeCircle two.height/200, two.width/200, Math.max(two.height, two.width)/200
    @circle.fill = "#fff"
  move: (x, y) ->
    @circle.translation.set x, y

