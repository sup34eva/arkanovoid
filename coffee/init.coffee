game = null
layers = [document.getElementById('layer1'), document.getElementById('layer2'), document.getElementById('layer3')]
bg = document.querySelector('canvas')
bg.width = window.innerWidth/2
bg.height = window.innerHeight/2
bg.style.webkitTransform = "scale(2) translateX(25%) translateY(25%)"
ctx = bg.getContext('2d')
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

