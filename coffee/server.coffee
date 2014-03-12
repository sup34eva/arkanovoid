server = document.querySelector("embed")
if server is null
	console.warn 'NaCl is null, switching to WebWorker ...'
	path = document.location.href.split("/")
	delete path[path.length - 1]
	path = path.join("/")

	xhr = new XMLHttpRequest()
	xhr.open 'GET', path + 'js/worker.js'
	xhr.onreadystatechange = ->
		if xhr.readyState is 4
			console.log xhr, path + 'js/worker.js'
			server = new Worker(window.URL.createObjectURL(new Blob([xhr.response],
				type: "application/javascript"
			)))
			server.addEventListener "message", (e) ->
				game = e.data.split ":"
			, false
	xhr.send null
else
	server.addEventListener "message", ((e) ->
		game = e.data.split ":"
	), false
