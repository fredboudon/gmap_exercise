#include "gmap.hpp"
#include <stdexcept>

/*------------------------------------------------------------------------*/

/* 
    Create a new dart and return its id. 
    Set its alpha_i to itself (fixed points) 
    Use maxid to determine the new id. Dont forget to increment it.
*/
GMap::id_t GMap::add_dart()
{
    throw std::logic_error("Question 1a : add_dart reste à implémenter.");
}

/*------------------------------------------------------------------------*/


// Return the application of the alpha_deg on dart
GMap::id_t GMap::alpha(degree_t degree, id_t dart) const
{
    throw std::logic_error("Question 1b : alpha (un seul degré) reste à implémenter.");
}

// Return the application of a composition of alphas on dart
GMap::id_t GMap::alpha(degreelist_t degrees, id_t dart) const
{
    throw std::logic_error("Question 1b : alpha (composition) reste à implémenter.");
}


//  Test if dart is free for alpha_degree (if it is a fixed point) 
bool GMap::is_free(degree_t degree, id_t dart) const
{
    return false;
}

/*------------------------------------------------------------------------*/


// Link the two darts with a relation alpha_degree if they are both free.
bool GMap::link_darts(degree_t degree, id_t dart1, id_t dart2)
{
    return false;
}


/*------------------------------------------------------------------------*/

/*
        Test the validity of the structure. 
        Check if alpha_0 and alpha_1 are involutions with no fixed points.
        Check if alpha_2 is an involution.
        Check if alpha_0 o alpha_2 is an involution
*/
bool GMap::is_valid() const
{
    return false;
}

/*------------------------------------------------------------------------*/


/* 
    Return the orbit of dart using a list of alpha relation.
    Example of use : gmap.orbit([0,1],0).
*/
GMap::idlist_t GMap::orbit(const degreelist_t& alphas, id_t dart) const
{
    throw std::logic_error("Question 2a : orbit reste à implémenter.");
}

/*
    Return the ordered orbit of dart using a list of alpha relations by applying
    repeatingly the alpha relations of the list to dart.
    Example of use. gmap.orderedorbit([0,1],0).
    Warning: No fixed point for the given alpha should be contained.
*/
GMap::idlist_t GMap::orderedorbit(const degreelist_t& list_of_alpha_value, id_t dart) const
{
    throw std::logic_error("Question 2b : orderedorbit reste à implémenter.");
}


/*
    Check if a dart of the orbit representing the vertex has already been
    associated with a value in propertydict. If yes, return this dart, else
    return the dart passed as argument.
*/



GMap::id_t GMap3D::get_embedding_dart(id_t dart) const
{
    throw std::logic_error("Question 3 : get_embedding_dart reste à implémenter.");
}

/*
    Sew two elements of degree 'degree' that start at dart1 and dart2.
    Determine first the orbits of dart to sew and heck if they are compatible.
    Sew pairs of corresponding darts
    # and if they have different embedding  positions, merge them. 
*/
bool GMap::sew_dart(degree_t degree, id_t dart1, id_t dart2)
{
    return false;
}

// Compute the Euler-Poincare characteristic of the subdivision
int GMap::eulercharacteristic() const
{
    return 0;
}

/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*/

GMap3D GMap3D::dual()
{
    throw std::logic_error("Question 7 : dual reste à implémenter.");
}


/*------------------------------------------------------------------------*/
