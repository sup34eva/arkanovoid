# Enregistre le nom de l'utilisateur dans un localStorage
$('form').submit (e) ->
	e.preventDefault()
	if $('input').val() isnt ''
		localStorage.username = $('input').val()
		document.location = '/'
