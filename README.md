Cartes Combinatoires
####################

Le canevas du fichier décrivant la structure de données GMap peut être téléchargé ici. Le fichier de test des différentes fonctions peut lui être téléchargé ici. Pour lancer les fonctions de test. 


1/ Implémenter une structure de 2-G-carte.
------------------------------------------
Chaque relation alpha (0, 1 et 2) sera codée par un dictionnaire (dict). Completer  la structure suivante

```
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

    typedef std::vector<id_t> idlist_t;
    typedef std::unordered_set<id_t> idset_t;
    typedef std::vector<degree_t> degreelist_t;
    typedef std::unordered_map<id_t, alpha_container_t> idalphamap_t;

    ...

protected:
    id_t maxid; // use to generate dart id
    idalphamap_t alphas; // a map with key corresponding to dart id and value a alpha_container_t that contains the 3 value of alpha for the given dart. example : { 0 : [0,1,2] }.
};

```

Une fois completer, vous pouvez tester cette structure avec la function question1 du fichier tp2.py.

2/ Encoder les parcours de la structure
class GMap : (continued ..)
    def orbit(self, dart, list_of_alpha_value):
        """ 
        Return the orbit of dart using a list of alpha relation.
        Example of use : gmap.orbit(0,[0,1]).
        """
        result = []
        marked = set([])
        toprocess = [dart]
        # Tant qu'il y a des elements à traiter
            # prendre un element d à traiter
            # si d n'est pas dans marked
                # rajouter d dans result et dans marked
                # pour chaque degree de list_of_alpha_value
                    # rajouter alpha_degree(d) dans toprocess
Cela permettra de déterminer par exemple tous les éléments de dégrée donné de la structure

class GMap : (continued ..)
    def elements(self, degree):
        """ 
        Return one dart per element of degree. For this, consider all darts as initial set S. 
        Take the first dart d, remove from the set all darts of the orbit starting from d and 
        corresponding to element of degree degree. Take then next element from set S and do the 
        same until S is empty. 
        Return all darts d that were used. 
        """
        
        elements = []
        darts = set(self.darts())
        list_of_alpha_value = range(3)
        list_of_alpha_value.remove(degree)
        while len(darts) > 0:
            dart = darts.pop()
            elementi = self.orbit(dart, list_of_alpha_value)
            darts -= set(elementi)
            elements.append(dart)
        return elements
Vous pouvez tester cette structure avec la function question2 du fichier tp2.py.

3/ Encoder le plongement géométrique
Implementer la fonction qui permet de determiner le brin qui contient l'information de plongement pour une i-cellule donné. Pour cela, il vous faut parcourir l'orbite de la i-cellule et vérifier si chaque brin n'a pas de valeur dans le dictionnaire de valeur de plongement.  Grace a cela, les fonctions get_position et set_position permettront d'associer une position à une 0-cellule.
class GMap : (continued ..)
  
    def get_embedding_dart(self, dart, propertydict ):
        """ 
        Check if a dart of the orbit representing the vertex has already been 
        associated with a value in propertydict. If yes, return this dart, else
        return the dart passed as argument.
        """

    def get_position(self, dart):
        """
        Retrieve the coordinates associated to the vertex <alpha_1, alpha_2>(dart) 
        """
        return self.positions.get(self.get_embedding_dart(dart,self.positions))

    def set_position(self, dart, position) :
        """
        Associate coordinates with the vertex <alpha_1,alpha_2>(dart)
        """
        self.positions[self.get_embedding_dart(dart,self.positions)] = position
Vous pourrez tester cela avec la fonction question3 du fichier tp2.py qui associe des coordonnées aux sommets d'un carré.

4/ Coder la fonction de couture qui permet de lier deux éléments de degrée ‘degree’. 
En effet si vous liez deux brins par alpha_2, il faut aussi lier leurs images par alpha_0 pour satisfaire la contrainte que alpha_2(alpha_0) est une involution. La même chose pour lier par alpha_0, il faut également lier les images par alpha_2.

class GMap : (continued ..)
      def sew_dart(self, degree, dart1, dart2, merge_attribute = True):
        """
        Sew two elements of degree 'degree' that start at dart1 and dart2.
        Determine first the orbits of dart to sew and heck if they are compatible.
        Sew pairs of corresponding darts, and if they have different embedding 
        positions, merge them. 
        """
Vous pouvez maintenant tester la création d'un cube et d'un cube troué (holeshape) avec les fonction question4a et question4b du fichier tp2.py.

%run tp2.py 4a
%run tp2.py 4b
5/ Visualiser les objets avec PlantGL.
Voici comment afficher une 2-G-Carte comme un ensemble de brins en plantgl (Note: pour pouvoir utiliser le viewer de plantgl, il faut utiliser ipython avec l’option –gui=qt4). Compléter la fonction permettant de visualiser la carte simplement comme un ensemble de faces.
class GMap : (continued ..)
    def display(self, color = (190,205,205), add = False):
    """
    Display the 2-cells of a 2-G-Map using the ordered orbit of its darts in PlantGL.
    For each face element, retrieve the position of its ordered face darts and add a FaceSet PlantGL object to the scene.
    Example : s += pgl.Shape(pgl.FaceSet( [[0,0,0],[1,0,0],[1,1,0],[0,1,0]], [[0,1,2,3]]) , pgl.Material((0,100,0))) # for a green square
    """
Pour cela, vous aurez besoin de coder une fonction d’orbite ordonnée qui alterne les différents alpha_degree

class GMap : (continued ..)
      def orderedorbit(self, dart, list_of_alpha_value):
        """
        Return the ordered orbit of dart using a list of alpha relations by applying
        repeatingly the alpha relations of the list to dart.
        Example of use. gmap.orderedorbit(0,[0,1]).
        Warning: No fixed point for the given alpha should be contained.
        """
Vous pourrez alors tester la visualization du cube et du cube troué avec question5a et question5b. A noter que vous pourrez également visualiser les brins de ces structures avec les fonctions question5c et question5d.

6/ Coder une fonction qui calcule la caractéristique d’Euler-Poincarré (S-A+F)


class GMap : (continued ..)
 def eulercharacteristic(self):
    """
    Compute the Euler-Poincare characteristic of the subdivision
    """
Vous pourrez tester cette fonction sur la structure de cube avec la fonction question6.

7/ Coder une fonction qui calcule le dual d’une 2-G-Carte.
class GMap : (continued ..)
  def dual(self):
    """
    Compute the dual of the object
    Create a new GMap object with the same darts but reversed alpha relations
    Update the positions of the dual 0-cells as the centers of the 2-cells
    """
Vous pourrez tester cette fonction sur la structure de cube avec la fonction question7.