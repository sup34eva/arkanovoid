class Brick
  constructor: (two, @x, @y) ->
    @rectangle = two.makeRectangle percent(@x*12.5, two.width) + two.width/16, percent(@y*10, two.height) + two.height/20, two.width/8, two.height/10
  visible: (v) ->
    @rectangle.visible = v[@x + (8 * @y)] is '1'

