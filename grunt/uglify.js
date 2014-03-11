module.exports = {
  options: {
    banner: '/*! <%= package.name %> <%= grunt.template.today("dd-mm-yyyy") %> */\n'
  },
  dist: {
    files: {
      'dist/<%= package.name %>.min.js': 'dist/<%= package.name %>.js'
    }
  }
};
