express = require 'express'
app = express()
routes = require './routes.json'
errors = require './error.coffee'
passport = require 'passport'
SupinfoStrategy = require('passport-supinfo').Strategy
flash = require 'connect-flash'
session = require 'express-session'
cookieParser = require 'cookie-parser'
bodyParser = require 'body-parser'
port = process.env.PORT || 3000
users = {}

app.set 'view engine', 'jade'

app.use require('compression')()
app.use require('morgan')('tiny')
app.use cookieParser()
app.use bodyParser.urlencoded(extended: true)
app.use bodyParser.json()

if process.env.NODE_ENV isnt 'production'
	app.use require('connect-livereload')(port: 35729)

app.use flash()
app.use session(
	secret: 'nsa is watching you'
)
app.use passport.initialize()
app.use passport.session()

options =
	returnURL: 'http://arkanovoid.leops.me/auth/supinfo/return'
	realm: 'http://arkanovoid.leops.me/'
	identifierField: 'username'

passport.use new SupinfoStrategy(options,
	(identifier, profile, done) ->
		process.nextTick ->
			done null, profile
)

passport.serializeUser (user, done) ->
	console.log "ID:", user.boosterID
	users[user.boosterID] = user
	done null, user.boosterID

passport.deserializeUser (id, done) ->
	done null, users[id]

ensureAuthenticated = (req, res, next) ->
	if req.isAuthenticated()
		return next()
	res.redirect '/connexion'

app.post '/auth/supinfo', passport.authenticate 'supinfo'

app.get '/auth/supinfo/return', passport.authenticate 'supinfo', {successRedirect: '/', failureRedirect: '/connexion', failureFlash: true}

app.get '/logout', (req, res) ->
	req.logout()
	res.redirect '/'

handler = (req, res) ->
	args = routes[req.route.path].args
	args.error = req.flash 'error'
	args.user = req.user
	res.render routes[req.route.path].template, args
	return

for i of routes
	if routes[i].auth
		app.get i, ensureAuthenticated, handler
	else
		app.get i, handler

app.use express.static('dist')
app.use '/bootstrap', express.static('bower_components/bootstrap/dist')
app.use '/jquery', express.static('bower_components/jquery/dist')
app.use errors.send404
app.use errors.log
app.use errors.format
app.use errors.xhr
app.use errors.show

app.listen port, ->
	console.log 'Listening on port ' + port
