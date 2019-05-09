--- Ce projet a été effectué et testé sur Debian 9 avec CMake ---
from src/cmakebuild , execute cmake ../ && make to build.

--- Known Bugs ---

-Le tir des roquettes de gauche ont une position inverse

-Les bounding boxes ne compute pas comme il faut (les ennemies sont au fond de la scene)
 to fix: La hiérarchie des ennemies n'est pas la même, il faut remonter d'un niveau
 Note: les collisions fonctionnent comme prévu lorsque l'on force des boundingbox qui sont plus grosses

-Le bout de Fighter2 (pyramid point) ne pointe pas dans le bon sens avant la rotation