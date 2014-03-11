module.exports = {
    compile: {
        options: {
            urlfunc: 'embedurl'
        },
        files: {
            'dist/css/style.css': ['stylus/**/*.styl', 'stylus/**/*.stylus']
        }
    }
}

