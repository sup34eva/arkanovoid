express = require 'express'
app = express()
routes = require './routes.json'
errors = require './error.coffee'
port = process.env.PORT || 3000

app.set 'view engine', 'jade'

app.use express.compress()
app.use express.logger('tiny')
app.use require('connect-livereload')()
app.use app.router
app.use express.static('dist')
app.use '/bootstrap', express.static('bower_components/bootstrap/dist')
app.use '/jquery', express.static('bower_components/jquery/dist')
app.use '/two', express.static('bower_components/two/build')
app.use '/stats', express.static('bower_components/stats.js/build')
app.use '/lvledit', express.static('lvledit')
app.use errors.send404
app.use errors.log
app.use errors.format
app.use errors.xhr
app.use errors.show

handler = (req, res) -> res.render routes[req.route.path].template, routes[req.route.path].args

`for (var i in routes)
	app.get(i, handler);`

app.listen port, ->
	console.log 'Listening on port ' + port
