getXML = (url, callback) ->
	xhr = new XMLHttpRequest()
	xhr.open 'GET', url, true
	xhr.onreadystatechange = ->
		if xhr.readyState is XMLHttpRequest.DONE and xhr.status is 200
			callback xhr.responseXML
	xhr.send null

getXML 'http://192.168.202.134/', (xml) ->
	console.log xml