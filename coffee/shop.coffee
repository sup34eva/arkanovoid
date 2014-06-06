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
    getXML "http://#{window.serverAddress}/?userid=#{user}", 'GET', (code, xml) ->
        callback xml2JS xml

addItemForPerson = (user, item) ->   
    getXML "http://#{window.serverAddress}/?userid=#{user}&itemid=#{item}", 'POST', (code) ->
        console.log code

useItemForPerson = (user, item) ->   
    getXML "http://#{window.serverAddress}/?userid=#{user}&itemid=#{item}", 'DELETE', (code) ->
        console.log code

getItems = (callback) ->    
    getXML "http://#{window.serverAddress}/", 'GET', (code, xml) ->
        callback xml2JS xml
        
getItem = (item, callback) ->    
    getXML "http://#{window.serverAddress}/?itemid=#{item}", 'GET', (code) ->
        console.log code


$('.thumbnail').each((i) ->
    console.log this
    $(this).attr('data-amount', Math.floor(Math.random() * 10))
)

$('form').submit (e) ->
    e.preventDefault()
    itemid = Number($(this).attr('data-item'))
    alltime = Number($(this).find('input[type="checkbox"]').prop('checked'))
    if not Number.isNaN alltime
        itemid += alltime
    for i in [0...$(this).find('input[type="number"]').val()]
        addItemForPerson localStorage.username, itemid
        console.log localStorage.username, itemid
