errmsg =
	500: 'Le serveur a rencontré une erreur'
	404: 'Page non trouvée'
	418: "I'm a teapot"

class ServerError extends Error
	constructor: (@code, @message) ->
		if @code instanceof Error
			@message = @code.message
			@code = @code.code
		@message = @message || errmsg[500]
		@code = Number(@code)
		@code = 500 if Number.isNaN(@code)
		return

exports.send404 = (req, res) ->
	console.log 'Path:', req.path
	throw new ServerError 404, errmsg[404]

exports.log = (err, req, res, next) ->
	console.error 'Error', err
	next err

exports.format = (err, req, res, next) ->
	next new ServerError(err)

exports.xhr = (err, req, res, next) ->
	if req.xhr
		res.send err.code,
			error: err.message
	else
		next err

exports.show = (err, req, res, next) ->
	res.render "error",
		code: err.code
		title: err.code + " " + err.message
		message: errmsg[err.code]
		path: req.path
	, (error, html) ->
		if error
			return res.send err.code
		else
			res.send err.code, html
