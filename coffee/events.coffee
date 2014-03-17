$(document).keydown((e) ->
	server.postMessage {name: "+", data: e.which}
	return
).keyup((e) ->
	server.postMessage {name: "-", data: e.which}
	return
).on "mousemove", (e) ->
	movement = (e.originalEvent.movementX or e.originalEvent.webkitMovementX or 0)/(two.width/100)
	server.postMessage {name: "m", data: movement}
	x = Math.abs(e.clientX - window.innerWidth)/window.innerWidth
	y = Math.abs(e.clientY - window.innerHeight)/window.innerHeight
	requestAnimationFrame ->
		$('#viewport').css 'background-position', (x * 15) + '% ' + (y * 15) + '%, ' + (x * 10) + '% ' + (y * 10) + '%, ' + (x * 5) + '% ' + (y * 5) + '%'
	return

