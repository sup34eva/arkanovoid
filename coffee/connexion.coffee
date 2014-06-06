$('form').submit (e) ->
	e.preventDefault()
	if $('input').val() isnt ''
		localStorage.username = $('input').val()
		document.location = '/'