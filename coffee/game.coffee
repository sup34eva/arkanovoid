root = {}
# Reception des messages du jeu
container = document.getElementById('listener');
container.addEventListener('message', (e) ->
	score = e.data.match /^score:([0-9]+)$/
	del = e.data.match /^delete:([0-9]+)$/
	# Si il s'agit d'un score, l'envoie a facebook
	if score isnt null
		fbShare score[1]
	# Si c'est un item, le supprimer
	else if del isnt null and window.user
		useItemForPerson window.user, del[1], $.noop
	# sinon, affiche le message
	else
		console.log e.data
, true)

# Barre de progression
$('embed').on('progress', (e) ->
	val = (e.originalEvent.loaded / e.originalEvent.total) * 100
	$('.progress-bar').attr('aria-valuenow', Math.floor(val)).css('width', val + '%')
	.find('span').html Math.floor(val) + '%'
).on('load', (e) ->
	$('#progressbar').hide()
	# Récupère les items de l'utilisateur
	if window.user
		getPerson window.user, (xml, code) ->
			# Conversion en tableau
			root = xml.root
			itemlist = for i of xml.root
						if xml.root[i].quantity > 0
							 Number(xml.root[i].item_id)
			console.log itemlist
			if code is 200
				# Envoie du tableau au C
				document.querySelector('embed').postMessage itemlist
			else
				console.warn "No item for this user"
)

# Partage un score sur facebook
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
