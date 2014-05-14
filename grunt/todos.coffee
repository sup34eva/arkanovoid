module.exports =
  options:
    priorities :
      low: /OPTIONAL/
      med: /TODO/
      high: /FIXME/
    reporter: 'markdown'
    verbose: false
  '../arkanoid.wiki/Todo.md': ['server/**/*.{c,cpp,cc,h}', 'coffee/**/*.coffee']
