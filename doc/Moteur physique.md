Moteur physique
===============

# Fonctionnement des balles
Tous les éléments du jeu sont stockés dans une structure nommée state. Il en va de même pour les balles, la raquette et les briques.

Les données des balles présentes a l'écran (4 maximum) sont stockées dans un tableau. Ces données sont : la position de la balle, son rayon, sa vitesse (en pixel par secondes) et sa vélocité. La vélocité correspond a la vitesse de la balle, mais sur les axes x et y. Pour actualiser la position de la balle, il suffit donc simplement d'ajouter sa vélocité a sa position.

# Rebond contre la raquette
Lorsque la balle se trouve en bas de l'écran et est alignée avec la paddle, la fonction PaddleCollision (calc.cc:81) est appelée. Cette fonction recalcule la vélocité de la balle en fonction de sa vitesse et de sa position par rapport a la raquette. Un angle est tout d'abord déterminé, allant de 45° a l'extémité droite de la raquette jusqu'a 135° a l'extrémité gauche. Cette fonction est ensuite convertie en radians grace a la formule rad = deg / 180 * pi. Enfin, cet angle est converti en vecteur avec les fonctions sinus et cosinus, vecteur qui est ensuite multiplié par la vitesse.

# Rebond sur les murs
Pour faire rebondir la balle contre un bord de l'écran, il suffit en fait simplement d'inverser sa vélocité sur l'axe normal de ce bord (axe x pour les bord gauche et droite, axe y pour le bord supérieur).

# Rebond contre les briques
A chaque image (fonction GameCalc), le programme itère sur toutes les briques et detecte celles entrant en collision avec la balle. Il determine ensuite la distance séparant le centre de la balle du centre de la brique. La distance la plus grande correspond a l'axe sur lequel la balle doit rebondir, et le signe de cette distance est le nouveau sens de la balle sur cet axe. A nouveau, il suffit simplement d'inverser la vélocité de la balle sur cet axe.