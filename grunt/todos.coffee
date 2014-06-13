module.exports =
  options:
    priorities :
      low: /OPTIONAL/
      med: /TODO/
      high: /FIXME/
    reporter: 'markdown'
    verbose: false
  '../arkanoid.wiki/Todo.md': ['**/*.{c,cpp,cc,h,coffee,styl,jade}']
