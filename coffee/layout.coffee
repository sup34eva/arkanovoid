if localStorage.username
	$('[href="/connexion"]').click((e) ->
		delete localStorage.username
		document.location = '/'
	).html($('[href="/connexion"]').contents()[0]).append('  Deconnexion').attr('href', '#')

if typeof webkitAudioContext is "undefined"
	alert "Site prévu pour Chrome"