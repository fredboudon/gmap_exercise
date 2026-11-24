Cartes Combinatoires
======================

Le canevas du TP se trouve dans `src/`. Pour le compiler, il faut un compilateur C++11 et CMake 3.16 ou plus récent.

```sh
cmake -S . -B build
cmake --build build
```

Les spécifications se trouvent dans `src/gmap.hpp`. Les fonctions à compléter se trouvent dans `src/gmap.cpp` et les fonctions fournies dans `src/gmap_helper.cpp`. Lancez une question en passant son numéro à l’exécutable, par exemple `./build/gmap 1a`. Dans la suite, `gmap X` désigne la commande à lancer si l’exécutable est dans le PATH. Sinon, utilisez `./build/gmap X` depuis la racine du dépôt. Sous Windows, l’exécutable s’appelle `gmap.exe` et peut se trouver dans `build/Debug/` selon le générateur CMake.


1/ La structure de 2-G-carte.
------------------------------------------
Cette GMap sera encodée en représentant chaque brin par un identifiant.
Chaque relation alpha (0, 1 et 2) sera codée par un dictionnaire (std::unordered_map). 
Etudiez et completez si necessaire la structure suivante

```c++
class GMap {
public:

    // id type
    typedef unsigned long int id_t;
    // degree type. should be in [0,1,2]
    typedef unsigned char degree_t;

    // a triplet of alpha value
    struct alpha_container_t {
        alpha_container_t(id_t a0 = 0, id_t a1 = 0, id_t a2 = 0) {
            values[0] = a0; values[1] = a1; values[2] = a2; 
        }
        id_t& operator[](degree_t index) { return values[index]; }
        const id_t& operator[](degree_t index) const { return values[index]; }

        alpha_container_t flip() const { return alpha_container_t(values[2],values[1],values[0]); }

        id_t values[3];
    };

    // a type for list of ids. usefull for processing
    typedef std::vector<id_t> idlist_t;
    // a type for set of ids. usefull to check if darts have already been processed. 
    typedef std::unordered_set<id_t> idset_t;
    // a type of list of degrees
    typedef std::vector<degree_t> degreelist_t;
    // a type that maps a dart id to some alpha values.
    typedef std::unordered_map<id_t, alpha_container_t> idalphamap_t;

    ...

protected:
    // use to generate dart ids
    id_t maxid; 
    /* a map with key corresponding to dart id and value a alpha_container_t 
       that contains the 3 value of alpha for the given dart. 
      example : { 0 : [0,1,2] }.
    */
    idalphamap_t alphas; 
};

```

Complétez les fonctions suivantes dans le fichier ```gmap.cpp```

```c++
    /* 
        Create a new dart and return its id. 
        Set its alpha_i to itself (fixed points) 
    */
    id_t add_dart();

```
Testez avec la fonction `question1a` de `gmap_main.cpp` ou la commande `gmap 1a`.

```c++
    // Return the application of the alpha_deg on dart
    id_t alpha(degree_t degree, id_t dart) const;

    // Return the application of a composition of alphas on dart. Apply degrees in reversed order.
    id_t alpha(degreelist_t degrees, id_t dart) const;

    
    //  Test if dart is free for alpha_degree (if it is a fixed point) 
    bool is_free(degree_t degree, id_t dart) const;
```

Testez avec la fonction `question1b` de `gmap_main.cpp` ou la commande `gmap 1b`.

```c++
    // Link the two darts with a relation alpha_degree if they are both free.
    bool link_darts(degree_t degree, id_t dart1, id_t dart2); 
```

Testez avec la fonction `question1c` de `gmap_main.cpp` ou la commande `gmap 1c`.

```c++
    /*
        Test the validity of the structure. 
        Check if alpha_0 and alpha_1 are involutions with no fixed points.
        Check if alpha_2 is an involution.
        Check if alpha_0 o alpha_2 is an involution
    */
    bool is_valid() const;

```

Testez avec la fonction `question1d` de `gmap_main.cpp` ou la commande `gmap 1d`.


2/ Les parcours de la structure
------------------------------------------

Complétez les fonctions de parcours par calcul d'orbite.

```c++
    /* 
        Return the orbit of dart using a list of alpha relation.
        Example of use : gmap.orbit({0,1}, 0).
    */
    idlist_t orbit(const degreelist_t& alphas, id_t dart) const;
    /*
    Canvas:
    {
        idlist_t result;
        idset_t marked;
        idlist_t toprocess = {dart};
        # Tant qu'il y a des elements à traiter
            # prendre un element d à traiter
            # si d n'est pas dans marked
                # rajouter d dans result et dans marked
                # pour chaque degree de list_of_alpha_value
                    # rajouter alpha_degree(d) dans toprocess

        return result;
    }
    */

    /*
        Return the ordered orbit of dart using a list of alpha relations by applying
        repeatingly the alpha relations of the list to dart.
        Example of use. gmap.orderedorbit({0,1}, 0).
        Warning: No fixed point for the given alpha should be contained.
    */
    idlist_t orderedorbit(const degreelist_t& list_of_alpha_value, id_t dart) const;
    /*
    Canvas:
    {
        idlist_t result;
        id_t current_dart = dart;
        unsigned char current_alpha_index = 0;
        size_t n_alpha = list_of_alpha_value.size();
        # Répéter au moins une fois, puis jusqu’au retour au brin de départ
            # ajouter current_dart au resultat
            # prendre le prochain alpha de list_of_alpha_value avec current_alpha_index
            # incrémenter current_alpha_index
            # changer current_dart par alpha_current_alpha(current_dart)

        return result;    
    }    
    */
```

- **`question2a` ou `gmap 2a`** vérifie directement `orbit` sur un carré. L’orbite de sommet contient les brins 0 et 7, l’orbite de face contient les huit brins et l’orbite d’un brin fixé par `alpha_2` contient ce seul brin. L’ordre de retour est libre et chaque brin doit apparaître une seule fois.
- **`question2b` ou `gmap 2b`** vérifie directement `orderedorbit`. Depuis le brin 0 avec `{0,1}`, le parcours attendu est `[0,1,2,3,4,5,6,7]`. Le test change aussi le brin de départ et l’ordre des relations.

Cela permettra de faire marcher la fonction ```elements``` qui détermine les i-cellules de la carte.

```c++
GMap::idlist_t GMap::elements( degree_t degree) {
    idlist_t elements;
    idlist_t vdarts = darts();
    idset_t sdarts (vdarts.begin(), vdarts.end());
 
    degreelist_t  list_of_alpha_value = {0, 1, 2};
    list_of_alpha_value.erase(list_of_alpha_value.begin()+degree);
 
    while (!sdarts.empty()){
        id_t dart = *sdarts.begin();
        sdarts.erase(sdarts.begin());
        idlist_t elementi = orbit(list_of_alpha_value, dart);
        for (id_t d : elementi) sdarts.erase(d);
        elements.push_back(dart);
    }
 
    return elements;
}
```

La fonction `question2c` ou la commande `gmap 2c` vérifie que `elements` trouve 4 sommets, 4 arêtes et 1 face.

3/ Le plongement géométrique
------------------------------------------

La classe `GMap3D` hérite de `GMap` et ajoute le plongement des sommets. Les coordonnées sont stockées dans le tableau `positions`. Le dictionnaire `properties` associe un brin représentant le sommet à un indice dans ce tableau.

Complétez la méthode suivante dans `src/gmap.cpp`, avec les autres fonctions du TP.

```cpp
GMap::id_t GMap3D::get_embedding_dart(id_t dart) const
```

Parcourez l’orbite `{1, 2}` de `dart`. Si un brin de cette orbite possède une entrée dans `properties`, renvoyez ce brin. Si aucun n’en possède, renvoyez `dart`. Les méthodes fournies `get_property` et `set_property` utilisent ce représentant pour lire ou écrire l’indice de la position.

Après recompilation, testez avec la fonction `question3` ou la commande `gmap 3`. Sur le carré, les brins 0 et 7 appartiennent au même sommet et retrouvent la position stockée sur le brin 0.


4/ La couture permettant de lier deux éléments de degrée ‘degree’. 
------------------------------------------------------------------------------------

Si vous liez deux brins par alpha_2, il faut aussi lier leurs images par alpha_0 pour satisfaire la contrainte que alpha_2(alpha_0) est une involution. La même chose pour lier par alpha_0, il faut également lier les images par alpha_2. Si le lien est fait par alpha_1, aucune contrainte s'applique. Il faut juste lier les brins.

```c++
/*
    Sew two elements of degree 'degree' that start at dart1 and dart2.
    Determine first the orbits of dart to sew and heck if they are compatible.
    Sew pairs of corresponding darts
    # and if they have different embedding  positions, merge them. 
*/
bool GMap::sew_dart(degree_t degree, id_t dart1, id_t dart2)
/*
Canvas:
{
    Si degree est égal a 1:
        Simplement lier par alpha_1 les deux brins
    Sinon:
        Si degree == 0: Trouver les orbites par alpha_2 des deux brins.
        Si degree == 2: Trouver les orbites par alpha_0 des deux brins.

        Vérifier que les orbites sont compatibles (meme taille). Sinon retourner false

        Lié deux a deux par alpha_degree les brins des 2 orbites.

        return true
}
*/
```

Testez le cube avec `question4a` ou `gmap 4a`, et le cube troué avec `question4b` ou `gmap 4b`.

5/ Caractéristique d’Euler-Poincaré (S-A+F)
------------------------------------------------------------------------------------

Complétez la fonction qui calcule la caractéristique d'Euler-Poincaré
```c++
// Compute the Euler-Poincare characteristic of the subdivision
int GMap::eulercharacteristic()
/*
Canvas:
{
    return S - A + F
}
```
Testez la caractéristique du cube avec `question5` ou `gmap 5`.


6/ Visualisation
------------------------------------------
La fonction `extract_mesh` de `src/gmap_display.cpp` montre comment convertir la G-carte en un maillage indexé. Elle associe un indice à chaque sommet plongé, puis parcourt chaque face avec `orderedorbit({0, 1}, dart)`. En prenant un brin sur deux dans cette orbite, on obtient les indices du contour dans le bon ordre.

Une fois les orbites, le plongement et la couture implémentés, testez la visualisation du cube avec `question6a` ou `gmap 6a`, et celle du cube troué avec `question6b` ou `gmap 6b`.

```sh
./build/gmap 6a  # Cube
./build/gmap 6b  # Cube troué
```

Chaque commande écrit `gmap-viewer.html` dans le répertoire courant et l’ouvre avec l’application associée aux fichiers HTML. Si l’ouverture automatique échoue, ouvrez le fichier dans un navigateur compatible WebGL 2. Le fichier fonctionne hors ligne et contient la géométrie et le visualiseur. Faites glisser le maillage pour le tourner et utilisez la molette pour zoomer.

Le visualiseur triangule les faces convexes pour le rendu et affiche les contours polygonaux d’origine. La triangulation en éventail convient aux faces des deux exemples.

7/ Le dual d’une 2-G-Carte.
------------------------------------------------------------------------------------

```c++
GMap3D GMap3D::dual()
/*
Canvas:
{
    GMap3D dual_gmap;
    dual_gmap.maxid = maxid;

    Créer un dictionnaire alpha egal a celui de this tel que les valeurs de alphas_2 et alphas_0 soit inversés.
    
    Pour chaque brin correspondant aux faces de this (et donc aux sommets de dual_gmap),
    Calculer le barycentre de la face avec la fonction element_center.
    Associer cette position au brin correspondant de dual_gmap


    return dual_gmap;

}
```

Testez le dual du cube avec `question7` ou `gmap 7`.

8/ Comparaison avec des structures alternatives
------------------------------------------------------------------------------------

Evaluez les changements necessaires pour encoder une simple 2-Carte.
Qu'est ce que vous y gagneriez ? perdriez ?

Comparez cette structure avec une structure Half-Edge ? Avec une simple liste de points indexés ?



Compilation avec le corrigé local
------------------------------------------

Pour compiler le corrigé, clonez le dépôt privé `gmap_exercise-solutions` à côté du dépôt du TP. Depuis la racine de `gmap_exercise`, configurez un répertoire de compilation distinct.

```sh
cmake -S . -B build/solution -DGMAP_SOLUTION=ON
cmake --build build/solution
```

CMake compile directement `../gmap_exercise-solutions/gmap_solution.cpp`. Modifiez ce fichier dans le dépôt privé, puis relancez `cmake --build build/solution`.

Si le corrigé se trouve ailleurs, indiquez son chemin avec `-DGMAP_SOLUTION_FILE=/chemin/vers/gmap_solution.cpp` à la configuration. Pour adapter une ancienne copie, incluez `gmap.hpp` en tête, retirez les blocs conditionnels `GMAP_CORE` en conservant leur contenu et transformez la définition template du plongement en `GMap::id_t GMap3D::get_embedding_dart(id_t dart) const`.
