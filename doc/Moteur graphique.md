Moteur graphique
================

La plateforme Native Client ayant un fonctionnement très spécifique, du à son fonctionnement a l'intérieur d'un navigateur, le portage d'un moteur existant est inenvisageable, car il s'agit d'un code trop complexe et trop lourd. Coder notre propre moteur en utilisant l'API Pepper etait en fait une solution plus simple et plus rapide.

# Fonctionnement
Le code de rendu est en fait divié en plusieurs parties, correspondant a différent niveaux d'abstraction.

Tout d'abord, ce code ne part pas de rien mais se base sur l'API Graphics2D de NaCl. Cette API permet d'accéder a un contexte, qui correspond a un cadre dans le navigateur. A chaque frame, le programme crée un buffer et y écris la nouvelle image, puis envoie ce buffer a Chrome afin de l'afficher dans la page.

Le buffer est un image, symbolisée de la manière la plus simple qui soit : un tableau, dans lequel chaque case correspond a un pixel. Ou plus précisement, la couleur de ces pixel, encodée sur 32 bits (Le rouge, le vert, le bleau et la tranparence, chacun encodés sur 6 bits).

Plutot que d'écrire directement dans ce buffer, un nouveau niveau d'abstration est ajouté dans rendu.cc avec les fonctions DrawRect, DrawCircle et DrawTexture. Ces fonctions permettent, comme leur nom l'indique, de dessiner des rectangles, des cercles et des textures dans un contexte. Ces fonctions peuvent être appelées depuis le code de rendu du jeu pour le simplifier.

Les textures sont en fait des structures contenant 4 informations : la hauteur et la largeur de cette image, la nombre de canaux (3 canaux signifie que l'image n'utilise pas le canal alpha et n'a donc pas de transparence), et un buffer. Ce buffer contient les données de couleur de l'image, cependant pour faciliter l'application des textures (notament la gestion de la transparence) les cases du tableau ne font que 6 bits et n'encodent donc qu'un seul canal. Au moment du dessin de la texture, il faut donc lire 3 ou 4 cases de ce tableau pour faire un pixel, en fonction du nombre de canaux.

Afin de faciliter le chargement des textures, un format de fichier a été créé. Les texture sont a la base des fichiers PNG, converties en fichier .tex par l'utilitaire png2tex dont le code se trouve dans le dossier tools. Celui-ci enregistre simplement la structure au format binaire dans le fichier, qui est ensuite chargé de la même manière dans le jeu par la fonction LoadTexture. Cette fonction monte le protocole HTTP comme un système de fichier afin de pouvoir accéder aux fichiers du serveur avec un simple fopen, et effectue ensuite l'opération inverse de png2tex. Un fois les textures chargées, elles sont stockées dans un tableau faisant partie de la structure principale du jeu, state.

# Utilisation
Ces fonctions utilitaires sont ensuite utilisées dan title.cc (fonctions TitleDraw, PauseDraw et ScoreDraw) et game.cc (fonction GameDraw). Ces différentes fonctions sont appelées 60 fois par secondes pour actualier l'image, et n'ont en faire qu'a appeler ces sous-fonctions en déterminant simplement a quelle position dessiner quelle texture.