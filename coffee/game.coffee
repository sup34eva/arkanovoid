root = {}
container = document.getElementById('listener');
container.addEventListener('message', (e) ->
	score = e.data.match /^score:([0-9]+)$/
	del = e.data.match /^delete:([0-9]+)$/
	if score isnt null
		fbShare score[1]
	else if del isnt null
		useItemForPerson localStorage.username, del[1], $.noop
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
		root = xml.root
		itemlist = for i of xml.root
					if xml.root[i].quantity > 0
						 Number(xml.root[i].item_id)
		console.log itemlist
		if code is 200
			document.querySelector('embed').postMessage itemlist
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
