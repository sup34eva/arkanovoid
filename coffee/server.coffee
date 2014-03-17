server = document.querySelector("embed")
server.addEventListener "message", ((e) ->
	game = e.data
	#console.log e.data
), false
window.server = server
