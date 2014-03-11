$(document).keydown((e) ->
  switch e.which
    when 37
      input = "l"
    when 39
      input = "r"
  server.postMessage "-#{input}"
  return
).keyup((e) ->
  switch e.which
    when 37
      input = "l"
    when 39
      input = "r"
  server.postMessage "-#{input}"
  return
).on "mousemove", (e) ->
  bgDr =
    x : e.screenX - window.innerWidth
    y : e.screenY - window.innerHeight
    h: window.innerHeight
    w: window.innerWidth
  movement = (e.originalEvent.movementX or e.originalEvent.webkitMovementX or 0)/(two.width/100)
  server.postMessage "m#{movement}"
  requestAnimationFrame ->
    layers.forEach (img, i) ->
      i =(i + 1) * 3
      ctx.drawImage img, (bgDr.x/bgDr.w) * i, (bgDr.y/bgDr.h) * i, (bgDr.w/2) + i, (bgDr.h/2) + i
  return

