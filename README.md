# Arkanovoid [![Stories in Ready](https://badge.waffle.io/sup3asc2/arkanovoid.png?label=ready&title=Ready)](https://waffle.io/sup3asc2/arkanovoid) [![Version](https://badge.fury.io/gh/sup3asc2%2Farkanovoid.png)](http://badge.fury.io/gh/sup3asc2%2Farkanovoid) [![Dependencies](https://david-dm.org/sup3asc2/arkanovoid.png?theme=shields.io)](https://david-dm.org/sup3asc2/arkanovoid) [![Build](https://travis-ci.org/sup3asc2/arkanovoid.png?branch=master)](https://travis-ci.org/sup3asc2/arkanovoid) [![Status](http://statusbadge.herokuapp.com/http%3A%2F%2Farkanovoid.herokuapp.com%2F)](http://arkanovoid.herokuapp.com/)
Remake en HTML5/NaCl du classique Arkanovoid, remake japonais du classique Breakout, remake solo du classique Pong, remake vidéoludique du classique ping-pong, remake miniature du tennis.

## Prérequis
Pour pouvoir compiler et lancer le jeu, les programmes suivants doivent être installés et présents dans le PATH:

 - Python 2.x (Utilisé par le SDK NaCl pour la compilation)
 - Git (Utilisé par bower pour installer les dépendences)
 - Node.JS / npm (Utilisé par le serveur et l'outil de compilation Grunt)

## Compilation
Le fichier package.json contient une série de hooks éxécuant automatiquement l'installation avec la commande :
> npm install

Cette commande va installer les dépendances npm mais aussi bower et ses dépendances, grunt, coffeescript et le SDK NaCl

Si toutefois une erreur survenait pendant l'installation, voici la liste des commandes a executer :

- `npm i -g grunt-cli bower coffee-script`

- `bower install`

- `grunt --force install`

Une fois l'installation terminée, lancer  :
> grunt

Cette commande va executer la tache par défaut de grunt, qui compile le code puis lance un serveur.

## Taches grunt

Plusieurs alias sont définis dans `aliases.yaml` pour simplifier les taches que grunt peux executer. Les principales sont :
> grunt build

Compile le coffeescript, le C, le stylus, compresse les images et convertit les texture.
> grunt server

Lance un serveur, l'ouvre dans le navigateur pas défaut, et lance une tache watch pour actualiser automatiquement les pages quand le code est modifié.
> grunt test

Lance coffeelint, cpplint et jsonlint pour vérifier la validité et la qualité des différents fichiers (coffeescript, C et routes.json)

La commande `grunt default` out `grunt` est un alias de `grunt build` et `grunt server`.

## Architecture du projet

Le projet contient un certain nombre de fichiers et dossier, dont voici la descriptions.

Tout d'abord, les fichiers qu'il vaut mieux ne pas toucher : `.gitignore`, `.travis.yml`, `Gruntfile.coffee`, `Procfile`, `bower.json`, `package.json` et le dossier `grunt`. Tous ces elements sont des fichiers de configuration, il n'y a normalement pas besoin de les modifier.

Le dossier `bin` : Ce dossier contient les utilitaires binaires (l'outil png2tex pour convertir les images en fichiers de textures).

Le dossier `coffee` : Comme son nom l'indique, ce dossier contient les fichiers coffeescript qui seront compilés en JavaScript.

Le dossier `styles`: Ici aussi le nom est explicite, ce dossier contient les fichiers stylus qui seront compilés en CSS.

Le dossier `img` : Encore une fois, tout est dans le nom. Ce dossier contient les images utilisées par le jeu.

Le dossier `img/textures`: Ce dossier contient les textures utilisées par le jeu.

Le dossier `jeu` : Ce dossier contient le code du serveur en C, mais aussi le Makefile qui définit la procédure de compilation. Une fois le code compilé, un dossier `newlib` devrait apparaitre, contenant les binaires.

Le dossier `views` : Ce dossier contient les fichiers jade qui seront affichés au client par le serveur.

Le dossier `tools` : Ce dossier contient diférents utilitaires, comme un hook pre-commit pour verifier le code avant un commit, ou le code source de png2tex.

Les fichiers `main.coffee` et `error.coffee` : Ces fichiers contiennent le code du serveur web qui heberge le site.

Le fichier `routes.json` : Ce fichier contient les routes du projet, associés chacune a un template jade et à différents arguments passé a ce template.

Le fichier `LICENSE` : Une copie de la license MIT

Le fichier `README.md` : Ce fichier.

## Note sur png2tex

L'antivirus Avast! detecte l'utilitaire png2tex comme un virus. Cependant, il s'agit d'un faux positif. Le code de l'utilitaire se trouve dans le dossier `tools` et peut être compilé manuellement si besoin est.
