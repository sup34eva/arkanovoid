getXML = (url, method, callback) ->
	xhr = new XMLHttpRequest()
	xhr.open method, url, true
	xhr.onreadystatechange = ->
		if xhr.readyState is XMLHttpRequest.DONE and (xhr.status is 200 or xhr.status is 201)
			callback xhr.status, xhr.responseXML
		return
	xhr.send null
	return

xml2JS = (xml) ->
	result = {}
	if xml.hasChildNodes and xml.hasChildNodes()
		for node in xml.childNodes
			if node.nodeType isnt 3
				result[node.tagName] = xmlToJS node
			else
				result = node.nodeValue
	return result

getPerson = (user, callback) ->
	getXML "http://192.168.202.134/?userid=#{user}", 'GET', (code, xml) ->
		callback xml2JS xml

addItemForPerson = (user, item) ->
	getXML "http://192.168.202.134/?userid=#{user}&itemid=#{item}", 'POST', (code) ->
		console.log code

useItemForPerson = (user, item) ->
	getXML "http://192.168.202.134/?userid=#{user}&itemid=#{item}", 'DELETE', (code) ->
		console.log code

getItems = (callback) ->
	getXML "http://192.168.202.134/", 'GET', (code, xml) ->
		callback xml2JS xml

getItem = (item, callback) ->
	getXML "http://192.168.202.134/?itemid=#{item}", 'GET', (code) ->
		console.log code
