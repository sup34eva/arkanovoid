window.serverAddress = '192.168.1.27'

getXML = (url, method, callback) ->
    xhr = new XMLHttpRequest()
    xhr.open method, url, true 
    xhr.onreadystatechange = ->
        if xhr.readyState is XMLHttpRequest.DONE
            callback xhr.status, xhr.responseXML
        return
    xhr.send null
    return

xml2JS = (xml) ->
    result = {}
    if xml.hasChildNodes and xml.hasChildNodes()
        for node in xml.childNodes
            if node.nodeType isnt 3
                result[node.tagName] = xml2JS node
            else
                result = node.nodeValue
    return result

getPerson = (user, callback) ->
    getXML "http://#{window.serverAddress}/?userid=#{user}", 'GET', (code, xml) ->
        console.log arguments
        callback xml2JS xml

addItemForPerson = (user, item, callback) ->   
    getXML "http://#{window.serverAddress}/?userid=#{user}&itemid=#{item}", 'POST', (code) ->
        console.log arguments
        if callback
            callback code

useItemForPerson = (user, item, callback) ->   
    getXML "http://#{window.serverAddress}/?userid=#{user}&itemid=#{item}", 'DELETE', (code) ->
        console.log arguments
        if callback
            callback code

getItems = (callback) ->    
    getXML "http://#{window.serverAddress}/", 'GET', (code, xml) ->
        console.log arguments
        callback xml2JS xml
        
getItem = (item, callback) ->    
    getXML "http://#{window.serverAddress}/?itemid=#{item}", 'GET', (code) ->
        console.log arguments
        if callback
            callback code
    	
getPerson localStorage.username, (xml) ->
    console.log xml.root
    for i of xml.root
        item = xml.root[i]
        $("[data-item='#{item.item_id}']").parent().parent().attr 'data-amount', item.quantity

$('form').submit (e) ->
    e.preventDefault()
    itemid = Number($(this).attr('data-item'))
    alltime = Number($(this).find('input[type="checkbox"]').prop('checked'))
    form = $(this)
    if not Number.isNaN alltime
        itemid += alltime
    for i in [0...$(this).find('input[type="number"]').val()]
        addItemForPerson localStorage.username, itemid, (code) ->
            form.parent().parent().attr('data-amount', Number(form.parent().parent().attr('data-amount')) + 1) unless code isnt 201
