# API du shop en Js
# L'adresse est a remplacer par celle de la VM
window.serverAddress = '10.13.16.23'

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
        callback xml2JS(xml), code

addItemForPerson = (user, item, callback) ->
    getXML "http://#{window.serverAddress}/?userid=#{user}&itemid=#{item}", 'POST', (code) ->
        if callback
            callback code

useItemForPerson = (user, item, callback) ->
    getXML "http://#{window.serverAddress}/?userid=#{user}&itemid=#{item}", 'DELETE', (code) ->
        if callback
            callback code

getItems = (callback) ->
    getXML "http://#{window.serverAddress}/", 'GET', (code, xml) ->
        callback xml2JS xml

getItem = (item, callback) ->
    getXML "http://#{window.serverAddress}/?itemid=#{item}", 'GET', (code) ->
        if callback
            callback code
