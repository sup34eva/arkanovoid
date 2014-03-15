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
	x = Math.abs(e.clientX - window.innerWidth)/window.innerWidth
	y = Math.abs(e.clientY - window.innerHeight)/window.innerHeight
	movement = (e.originalEvent.movementX or e.originalEvent.webkitMovementX or 0)/(two.width/100)
	server.postMessage "m#{movement}"
	requestAnimationFrame ->
		$('#viewport').css 'background-position', (x * 15) + '% ' + (y * 15) + '%, ' + (x * 10) + '% ' + (y * 10) + '%, ' + (x * 5) + '% ' + (y * 5) + '%'
	return

