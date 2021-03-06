# Se familiariser avec l'existant

## A- Exécution

Compilez et lancez le programme.

Allez dans le fichier `tower_sim.cpp` et recherchez la fonction responsable de gérer les inputs du programme.
Fonction : create_keystrokes
Sur quelle touche faut-il appuyer pour ajouter un avion ?
Ajouter un avion : C
Comment faire pour quitter le programme ?
Quitter le programme : X/Q
A quoi sert la touche 'F' ?
F : Mettre en full screen

Ajoutez un avion à la simulation et attendez.
Que est le comportement de l'avion ?
L'avion atterit, attend quelques temps puis repart
Quelles informations s'affichent dans la console ?
La console affiche lorsque l'avion se pose, qu'il attend/a fini d'attendre et quand il décole.

Ajoutez maintenant quatre avions d'un coup dans la simulation.
Que fait chacun des avions ?
3 avions se posent sur la station et un tourne autour en attendant son tour

## B- Analyse du code

Listez les classes du programme à la racine du dossier src/.
AirCraft : Représente un avion
AirPort : Représente un aéroport
AirPortType : Représente les types d'aéroport
Terminal : Représente un point d'atterissage de la tour
Tower : Représente une tour de contrôle
TowerSimulation : Représente un environnement capable de simuler l'activité d'une tour de contrôle
Waypoint : Représente un point d'atterrissage pour un avion
Pour chacune d'entre elle, expliquez ce qu'elle représente et son rôle dans le programme.

Pour les classes `Tower`, `Aircaft`, `Airport` et `Terminal`, listez leurs fonctions-membre publiques et expliquez précisément à quoi elles servent.
Réalisez ensuite un schéma présentant comment ces différentes classes intéragissent ensemble.
Tower : 
    WaypointQueue get_instructions(Aircraft& aircraft); Renvoie ce que l'avion doit faire : se poser sur un terminal ou continuer der tourner
    void Tower::arrived_at_terminal(const Aircraft& aircraft); Le terminal sur lequel l'avion est posé le sert
AirCraft :
    const std::string& get_flight_num(); Renvoie le numéro de vol
    float distance_to(const Point3D& p); Renvoie la distance par rapport a un point
    void display(); Fonction d'affichage
    void move(); Fonction de changement
AirPort :
    Tower& get_tower(); Renvoie la tour de contrôle associé à l'aéroport
    void display() ; Fonction d'affichage
    void move(); Fonction de changement
Terminal :
    bool in_use(); Renvoie si le terminal est utilisé
    bool is_servicing(); Renvoie si le terminal est entrain de servir
    void assign_craft(const Aircraft& aircraft); Assign un avion au terminal
    void start_service(const Aircraft& aircraft); Commence le service d'un avion
    void finish_service(); Termine le service d'un avion
    void move(); Fonction de changement
Tower Utilise : Aircraft, Airport, Terminal
AirCraft Utilise : Tower
AirPort Utilise : Terminal Tower
Terminal Utilise : Aircraft
Quelles classes et fonctions sont impliquées dans la génération du chemin d'un avion ?
La classe Tower et ses méthodes get_instructions et get_circle
Quel conteneur de la librairie standard a été choisi pour représenter le chemin ?
deque<WayPoint>
Expliquez les intérêts de ce choix.
???

## C- Bidouillons !

1) Déterminez à quel endroit du code sont définies les vitesses maximales et accélération de chaque avion.
Dans le fichier aircraft_types
Le Concorde est censé pouvoir voler plus vite que les autres avions.
Modifiez le programme pour tenir compte de cela.

2) Identifiez quelle variable contrôle le framerate de la simulation.
La variable DEFAULT_TICKS_PER_SEC
Ajoutez deux nouveaux inputs au programme permettant d'augmenter ou de diminuer cette valeur.
Essayez maintenant de mettre en pause le programme en manipulant ce framerate. Que se passe-t-il ? Fixez le problème.
Le programme plante si lorsque le framerate arrive a 0

3) Identifiez quelle variable contrôle le temps de débarquement des avions et doublez-le.
C'est SERVICE_CYCLES

4) Lorsqu'un avion décolle, celui-ci n'est pas retiré du programme.
Faites en sorte qu'il le soit.
//TROUVER ON QU ON VA AVEC UN BREAK POINT

5) Lorsqu'un objet de type `Displayable` est créé, il faut ajouter celui-ci manuellement dans la liste des objets à afficher.
Il faut également penser à le supprimer de cette liste avant de le détruire.
Faites en sorte que l'ajout et la suppression de `display_queue` soit "automatiquement gérée" lorsqu'un `Displayable` est créé ou détruit.
Pourquoi n'est-il pas spécialement pertinent d'en faire de même pour `DynamicObject` ?

6) La tour de contrôle a besoin de stocker pour tout `Aircraft` le `Terminal` qui lui est actuellement attribué, afin de pouvoir le libérer une fois que l'avion décolle.
Cette information est actuellement enregistrée dans un `std::vector<std::pair<const Aircraft*, size_t>>` (size_t représentant l'indice du terminal).
Cela fait que la recherche du terminal associé à un avion est réalisée en temps linéaire, par rapport au nombre total de terminaux.
Cela n'est pas grave tant que ce nombre est petit, mais pour préparer l'avenir, on aimerait bien remplacer le vector par un conteneur qui garantira des opérations efficaces, même s'il y a beaucoup de terminaux.\
Modifiez le code afin d'utiliser un conteneur STL plus adapté. Normalement, à la fin, la fonction `find_craft_and_terminal(const Aicraft&)` ne devrait plus être nécessaire.

## D- Théorie

1) Comment a-t-on fait pour que seule la classe `Tower` puisse réserver un terminal de l'aéroport ?

2) En regardant le contenu de la fonction `void Aircraft::turn(Point3D direction)`, pourquoi selon-vous ne sommes-nous pas passer par une réference ?
Pensez-vous qu'il soit possible d'éviter la copie du `Point3D` passé en paramètre ?

## E- Bonus

Le temps qui s'écoule dans la simulation dépend du framerate du programme.
La fonction move() n'utilise pas le vrai temps. Faites en sorte que si.
Par conséquent, lorsque vous augmentez le framerate, la simulation s'exécute plus rapidement, et si vous le diminuez, celle-ci s'exécute plus lentement.

Dans la plupart des jeux ou logiciels que vous utilisez, lorsque le framerate diminue, vous ne le ressentez quasiment pas (en tout cas, tant que celui-ci ne diminue pas trop).
Pour avoir ce type de résultat, les fonctions d'update prennent généralement en paramètre le temps qui s'est écoulé depuis la dernière frame, et l'utilise pour calculer le mouvement des entités.

Recherchez sur Internet comment obtenir le temps courant en C++ et arrangez-vous pour calculer le dt (delta time) qui s'écoule entre deux frames.
Lorsque le programme tourne bien, celui-ci devrait être quasiment égale à 1/framerate.
Cependant, si le programme se met à ramer et que la callback de glutTimerFunc est appelée en retard (oui oui, c'est possible), alors votre dt devrait être supérieur à 1/framerate.

Passez ensuite cette valeur à la fonction `move` des `DynamicObject`, et utilisez-la pour calculer les nouvelles positions de chaque avion.
Vérifiez maintenant en exécutant le programme que, lorsque augmentez le framerate du programme, vous n'augmentez pas la vitesse de la simulation.

Ajoutez ensuite deux nouveaux inputs permettant d'accélérer ou de ralentir la simulation.
