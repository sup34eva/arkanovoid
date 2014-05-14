express = require 'express'
serveStatic = require 'serve-static'
app = express()
routes = require './routes.json'
errors = require './error.coffee'
port = process.env.PORT || 3000

app.set 'view engine', 'jade'

app.use require('compression')()
app.use require('morgan')('tiny')
app.use require('connect-livereload')()

handler = (req, res) ->
	res.render routes[req.route.path].template, routes[req.route.path].args
	return

app.get i, handler for i of routes

app.use serveStatic('dist')
app.use '/bootstrap', serveStatic('bower_components/bootstrap/dist')
app.use '/jquery', serveStatic('bower_components/jquery/dist')
app.use '/two', serveStatic('bower_components/two/build')
app.use '/stats', serveStatic('bower_components/stats.js/build')
app.use errors.send404
app.use errors.log
app.use errors.format
app.use errors.xhr
app.use errors.show

app.listen port, ->
	console.log 'Listening on port ' + port
