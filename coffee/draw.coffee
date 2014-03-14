two.bind "update", (frameCount) ->
  if game instanceof Array
    ball.move percent(game[0], two.width), percent(game[1], two.height)
    paddle.move percent(game[2], two.width), two.width/(100/game[3])
    i = 0
    while i < 8
      j = 0
      while j < 8
        bricks[i][j].visible game[4]
        j++
      i++
  server.postMessage two.timeDelta/1000
  #console.log two.timeDelta
  return

