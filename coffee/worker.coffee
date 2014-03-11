game = [1, 1, 100, 10, "8142241824428100"]
inputs = ""
lastUpdate = Date.now()
velocity = [1, 1]

# 81 42 24 18 24 42 81 00
###
10000001
01000010
00100100
00011000
00100100
01000010
10000001
00000000
###

importScripts '%PATH%js/util.js'

calc = ->
  now = Date.now()
  timeDelta = now - lastUpdate
  lastUpdate = now

  game[0] += timeDelta/10 * velocity[0]
  game[1] += timeDelta/10 * velocity[1]

  if game[0] >= 99
    velocity[0] = -1
  if game[0] <= 1
    velocity[0] = 1
  if game[1] <= 1
    velocity[1] = 1
  if game[1] >= 98
    velocity[1] = -1

  if inputs.indexOf('r') isnt -1
    game[2] += timeDelta/10
  if inputs.indexOf('l') isnt -1
    game[2] -= timeDelta/10
  game[2] = Math.min Math.max(game[2], 0), 100
  postMessage game.join ':'

self.addEventListener 'message', (e) ->
  if e.data[0] is '+'
    inputs += e.data[1]
  else if e.data[0] is 'm'
    game[2] += new Number e.data.substr 1
  else
    inputs = inputs.replace e.data[1], '', 'gi'
  calc()

game[4] = hexToBinary game[4]
lastUpdate = Date.now()

update = ->
  calc()
  setTimeout update, 30

setTimeout update, 0