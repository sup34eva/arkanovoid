container = document.getElementById('listener');
container.addEventListener('message', (e) ->
	console.log e.data
, true)

$('embed').on('progress', (e) ->
	val = (e.originalEvent.loaded / e.originalEvent.total) * 100
	$('.progress-bar').attr('aria-valuenow', Math.floor(val)).css('width', val + '%')
	.find('span').html Math.floor(val) + '%'
).on('load', (e) ->
	$('#progressbar').hide()
)
