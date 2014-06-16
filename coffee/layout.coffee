# Fonctions communes
# Ajoute un bouton de déconnection sur beoin est
if localStorage.username
	$('[href="/connexion"]').click((e) ->
		delete localStorage.username
		document.location = '/'
	).html($('[href="/connexion"]').contents()[0]).append('  Deconnexion').attr('href', '#')

# Affiche un message d'alerte si le site n'est pas utilisé avec chrome
if typeof webkitAudioContext is "undefined"
	alert "Site prévu pour Chrome"
