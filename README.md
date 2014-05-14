[![Stories in Ready](https://badge.waffle.io/hunsdetroyes/arkanoid.png?label=ready&title=Ready)](https://waffle.io/hunsdetroyes/arkanoid)
# Arkanoid [![Version](https://badge.fury.io/gh/hunsdetroyes%2Farkanoid.png)](http://badge.fury.io/gh/hunsdetroyes%2Farkanoid) [![Dependencies](https://david-dm.org/hunsdetroyes/arkanoid.png?theme=shields.io)](https://david-dm.org/hunsdetroyes/arkanoid) [![Build](https://travis-ci.org/hunsdetroyes/arkanoid.png?branch=master)](https://travis-ci.org/hunsdetroyes/arkanoid) [![Status](http://statusbadge.herokuapp.com/http%3A%2F%2Farkanoid.herokuapp.com%2F)](http://arkanoid.herokuapp.com/)
Remake en HTML5/NaCl du classique Arkanoid, remake japonais du classique Breakout, remake solo du classique Pong, remake vidéoludique du classique ping-pong.

## Compilation
Le jeu nécessite nodejs/npm pour lancer le serveur et effectuer la compilation. Une fois nodejs installé, installer les dépendances avec :
> npm install

npm va installer les dépendances de nodejs. Il faut aussi installer celles de bower avec :
> bower install

Une fois cela fais, il faut installer le SDK NaCl avec la commande :
> grunt shell:update

Enfin, lancer la commande :
> grunt

Celle-ci executer la tache par défaut de grunt, qui compile le code puis lance un serveur.

## Taches grunt

Plusieurs alias sont définis dans `asliases.yaml` pour simplifier les taches que grunt peux executer. Les principales sont :
> grunt build

Compile le coffeescript, le C++, le stylus et compresse les images
> grunt server

Lance un serveur, l'ouvre dans le navigateur pas défaut, et lance une tache watch pour actualiser automatiquement les pages quand le code est modifié.
> grunt test

Lance coffeelint, cpplint et jsonlint pour vérifier la validité et la qualité des différents fichiers (coffeescript, C++ et routes.json)

La commande `grunt default` est un alias de `grunt build` et `grunt server`.

## Architecture du projet

Le repo contient un certain nombre de fichiers et dossier, dont voici la descriptions.

Tout d'abord, les fichiers qu'il vaut mieux ne pas toucher : `.gitignore`, `.travis.yml`, `Gruntfile.coffee`, `Procfile`, `bower.json`, `package.json` et le dossier `grunt`. Tous ces elements sont des fichiers de configuration, il n'y a normalement pas besoin de les modifier.

Le dossier `coffee` : Comme son nom l'indique, ce dossier contient les fichiers coffeescript qui seront compilés en JavaScript.

Le dossier `styles`: Ici aussi le nom est explicite, ce dossier contient les fichiers stylus qui seront compilés en CSS.

Le dossier `img` : Encore une fois, tout est dans le nom. Ce dossier contient les images utilisées par le jeu.

Le dossier `server` : Ce dossier contient le code du serveur en C++, mais aussi le Makefile qui définit la procédure de compilation, un fichier `server.nmf` qui définit l'application NaCl et un fichier `server.pexe` qui est le binaire compilé de l'application.

Le dossier `views` : Ce dossier contient les fichiers jade qui serotn servis aux clients

Les fichiers `main.coffee` et `error.coffee` : Ces fichiers contiennent le code du serveur web qui heberge le site.

Le fichier `routes.json` : Ce fichier contient les routes du projet, associés chacune a un template jade et à différents arguments passé a ce template.

Le fichier `LICENSE` : Une copie de la license MIT

Le fichier `README.md` : Ce fichier.
