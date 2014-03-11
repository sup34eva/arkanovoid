module.exports = {
    express: {
        files:  ['main.js', 'routes.json'],
        tasks:  ['express:server', 'notify:server'],
        options: {
            spawn: false
        }
    },
    coffee: {
        files:  ['coffee/**/*.coffee'],
        tasks:  ['coffeescript', 'notify:coffee'],
        options: {
            spawn: false
        }
    },
    style: {
        files:  ['stylus/**/*.styl', 'stylus/**/*.stylus'],
        tasks:  ['stylus', 'notify:stylus'],
        options: {
            spawn: false
        }
    }
};

