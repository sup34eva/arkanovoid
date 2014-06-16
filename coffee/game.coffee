container = document.getElementById('listener');
container.addEventListener('message', (e) ->
	score = e.data.match /^score:([0-9]+)$/
	if score isnt null
		fbShare score[1]
	else
		console.log e.data
, true)

$('embed').on('progress', (e) ->
	val = (e.originalEvent.loaded / e.originalEvent.total) * 100
	$('.progress-bar').attr('aria-valuenow', Math.floor(val)).css('width', val + '%')
	.find('span').html Math.floor(val) + '%'
).on('load', (e) ->
	$('#progressbar').hide()
	getPerson localStorage.username, (xml, code) ->
		if code is 200
			document.querySelector('embed').postMessage(xml.root[i].quantity > 0 for i of xml.root)
		else
			console.warn "No item for this user"
)

fbShare = (score) ->
	FB.ui(
		method: "share_open_graph"
		action_type: "arkanovoid:score"
		action_properties: JSON.stringify(
			game: "http://arkanoid.herokuapp.com/"
			score: score
		)
		, (response) -> console.log response
	)
