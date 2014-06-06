$('input, textarea').on('input', (e) ->
	if $(this).val() isnt ''

		if $(this).attr('type') is 'email' and $(this).val().match(/^(([^<>()[\]\\.,;:\s@\"]+(\.[^<>()[\]\\.,;:\s@\"]+)*)|(\".+\"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/) is null
			$(this).parent().parent().removeClass('has-warning').addClass('has-error').removeClass('has-success')
			$(this).parent().find('.fa').removeClass('fa-exclamation-triangle').removeClass('fa-check').addClass('fa-times')
		else
			$(this).parent().parent().removeClass('has-warning').removeClass('has-error').addClass('has-success')
			$(this).parent().find('.fa').removeClass('fa-exclamation-triangle').addClass('fa-check').removeClass('fa-times')
	else
		$(this).parent().parent().addClass('has-warning').removeClass('has-error').removeClass('has-success')
		$(this).parent().find('.fa').addClass('fa-exclamation-triangle').removeClass('fa-check').removeClass('fa-times')
)
