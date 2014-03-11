updateViewsize = ->
  bricks.forEach (a, i) ->
    a.forEach (b, j) ->
      poly = two.makeRectangle(two.width/16+(j*two.width/8), two.width/20+(i*two.height/10), two.width/8, two.height/10)
      bricks[i][j].remove()
      bricks[i][j] = poly
      return
    return
  return

fullscreenChange = ->
  if document.webkitFullscreenElement is elem
    elem.requestPointerLock = elem.requestPointerLock or elem.webkitRequestPointerLock
    elem.requestPointerLock()
    updateViewsize()
  return

elem = viewport.firstElementChild

document.addEventListener "fullscreenchange", fullscreenChange, false
document.addEventListener "webkitfullscreenchange", fullscreenChange, false
$(document).click ->
  elem.requestFullscreen = elem.requestFullscreen or elem.webkitRequestFullscreen
  elem.requestFullscreen()
  return

$(window).resize updateViewsize
