container = document.getElementById('listener');
container.addEventListener('message', (e) ->
	console.log e.data
, true)

$('embed').on('progress', (e) ->
	val = (e.originalEvent.loaded / e.originalEvent.total) * 100
	$('.progress-bar').attr('aria-valuenow', val).css('width', val + '%')
	.find('span').html val + '%'
	console.log val
).on('load', (e) ->
	$('.progress').hide()
)
