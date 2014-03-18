two.bind "update", (frameCount) ->
	#server.postMessage two.timeDelta/1000
	clientStats.update()
	return
