#include "gmap.hpp"
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>

void question1a(){
    GMap mgmap;
    mgmap.add_dart();
    mgmap.add_dart();
    mgmap.add_dart();
    mgmap.print_alphas();
    assert(mgmap.darts().size() == 3);
    printf("add_dart seems valid\n");
}


void question1b(){
    GMap mgmap;
    GMap::idlist_t vlist;
    for (int i = 0; i < 10; ++i) vlist.push_back(mgmap.add_dart());

    for(GMap::id_t vid : vlist) {
        for(int degree : {0,1,2}){
            assert(mgmap.alpha(degree,vid) == vid);
            assert(mgmap.is_free(degree,vid));
        }
    }
    printf("alpha and is_free seems valid\n");
}

void question1c(){
    GMap mgmap;
    GMap::idlist_t vlist;
    for (int i = 0; i < 4; ++i) vlist.push_back(mgmap.add_dart());
    bool res = mgmap.link_darts(0, vlist[0], vlist[1]);
    assert(res == true);
    res = mgmap.link_darts(1, vlist[1], vlist[2]);
    assert(res == true);
    res = mgmap.link_darts(0, vlist[2], vlist[3]);
    assert(res == true);
    res = mgmap.link_darts(1, vlist[3], vlist[0]);
    assert(res == true);
    res = mgmap.link_darts(1, vlist[3], vlist[2]);
    assert(res == false and "darts are already linked");

    for(GMap::id_t vid : vlist) {
        assert(mgmap.alpha(0,vlist[0]) == vlist[1]);
        assert(mgmap.alpha(0,vlist[1]) == vlist[0]);
        assert(mgmap.alpha(0,vlist[2]) == vlist[3]);
        assert(mgmap.alpha(0,vlist[3]) == vlist[2]);

        assert(mgmap.alpha(1,vlist[0]) == vlist[3]);
        assert(mgmap.alpha(1,vlist[1]) == vlist[2]);
        assert(mgmap.alpha(1,vlist[2]) == vlist[1]);
        assert(mgmap.alpha(1,vlist[3]) == vlist[0]);

        assert(mgmap.alpha(2,vlist[0]) == vlist[0]);
        assert(mgmap.alpha(2,vlist[1]) == vlist[1]);
        assert(mgmap.alpha(2,vlist[2]) == vlist[2]);
        assert(mgmap.alpha(2,vlist[3]) == vlist[3]);

    }
    printf("link_darts  seems valid\n");
}


void question1d(){
    GMap mgmap;
    GMap::idlist_t vlist;
    for (int i = 0; i < 4; ++i) vlist.push_back(mgmap.add_dart());
    assert(!mgmap.is_valid() && "check no point fix for alpha_0");
    mgmap.link_darts(0, vlist[0], vlist[1]);
    assert(!mgmap.is_valid() && "check no point fix for alpha_0");  
    mgmap.link_darts(0, vlist[2], vlist[3]);
    assert(!mgmap.is_valid() && "check no point fix for alpha_1");  
    mgmap.link_darts(1, vlist[1], vlist[2]);
    assert(!mgmap.is_valid() && "check no point fix for alpha_1");  
    mgmap.link_darts(1, vlist[3], vlist[0]);
    assert(mgmap.is_valid());  
    mgmap.alphas[vlist[3]][1] = vlist[2];
    assert(!mgmap.is_valid() && "check if alpha_1 is involution");  
    mgmap.alphas[vlist[3]][1] = vlist[0];
    mgmap.alphas[vlist[0]][0] = vlist[2];
    assert(!mgmap.is_valid() && "check if alpha_0 is involution");  
    printf("is_valid seems valid\n");
}

void question2a()
{
    const GMap square = GMap::square();

    auto vertex = square.orbit({1, 2}, 0);
    std::sort(vertex.begin(), vertex.end());
    if (vertex != GMap::idlist_t{0, 7}) {
        throw std::logic_error("Question 2a : l’orbite de sommet {1,2} depuis 0 doit contenir 0 et 7 une seule fois.");
    }

    auto face = square.orbit({0, 1}, 0);
    std::sort(face.begin(), face.end());
    if (face != GMap::idlist_t{0, 1, 2, 3, 4, 5, 6, 7}) {
        throw std::logic_error("Question 2a : l’orbite de face {0,1} depuis 0 doit contenir les huit brins une seule fois.");
    }

    if (square.orbit({2}, 3) != GMap::idlist_t{3}) {
        throw std::logic_error("Question 2a : alpha_2 fixe le brin 3 du carré. Son orbite doit contenir seulement 3.");
    }
    std::puts("Question 2a réussie : orbites du carré.");
}

void question2b()
{
    const GMap square = GMap::square();
    if (square.orderedorbit({0, 1}, 0) != GMap::idlist_t{0, 1, 2, 3, 4, 5, 6, 7}) {
        throw std::logic_error("Question 2b : parcours {0,1} depuis 0 attendu [0,1,2,3,4,5,6,7].");
    }
    if (square.orderedorbit({1, 0}, 3) != GMap::idlist_t{3, 4, 5, 6, 7, 0, 1, 2}) {
        throw std::logic_error("Question 2b : parcours {1,0} depuis 3 attendu [3,4,5,6,7,0,1,2].");
    }
    std::puts("Question 2b réussie : orbites ordonnées du carré.");
}

void question2c()
{
    const GMap square = GMap::square();
    const std::size_t expected[] = {4, 4, 1};
    for (GMap::degree_t degree : {0, 1, 2}) {
        const auto elements = square.elements(degree);
        if (elements.size() != expected[degree]) {
            throw std::logic_error("Question 2c : degré " + std::to_string(degree) +
                                   ", attendu " + std::to_string(expected[degree]) +
                                   ", obtenu " + std::to_string(elements.size()) + ".");
        }
        std::printf("Cellules de degré %d :", degree);
        for (auto dart : elements) {
            std::printf(" %lu", dart);
        }
        std::puts("");
    }
}

GMap3D question3(){
    GMap3D mgmap = GMap3D::square();
    mgmap.print_alphas();
    return mgmap;
}

GMap3D question4a(){
    GMap3D mgmap = GMap3D::cube();
    mgmap.print_alphas();
    return mgmap;
}

GMap3D question4b(){
    GMap3D mgmap = GMap3D::holeshape();
    mgmap.print_alphas();
    assert(mgmap.is_valid());
    return mgmap;
}


void question5(){
    GMap3D mgmap = GMap3D::cube();
    printf("Euler characteristic : %i\n", mgmap.eulercharacteristic());
}


int question6a() {
    GMap3D mgmap = question4a();
    return display(mgmap);
}

int question6b() {
    GMap3D mgmap = question4b();
    return display(mgmap);
}



void question7(){
    GMap3D mgmap = GMap3D::cube();
    mgmap.print_alphas();
    mgmap = mgmap.dual();
    mgmap.print_alphas();
    //display(mgmap);
}

#define CHECK(I, target) \
            if (strcmp(selection,I) == 0){ \
                printf("Selected question :%s\n",selection); \
                target(); \
            } \

int main(int argc, char ** argv){
    const char * selection = "1";
    if(argc == 2) {
        selection = argv[1];
    }

    try {
        CHECK("1a", question1a)
        else CHECK("1b", question1b)
        else CHECK("1c", question1c)
        else CHECK("1d", question1d)
        else CHECK("2a", question2a)
        else CHECK("2b", question2b)
        else CHECK("2c", question2c)
        else CHECK("3", question3)
        else CHECK("4a", question4a)
        else CHECK("4b", question4b)
        else CHECK("5", question5)
        else if (strcmp(selection, "6a") == 0) return question6a();
        else if (strcmp(selection, "6b") == 0) return question6b();
        else CHECK("7", question7)
        else {
            printf("Default question 1a. Unknown option :%s\n",selection);
            question1a();
        }
    } catch (const std::logic_error& error) {
        std::fprintf(stderr, "%s\n", error.what());
        return 1;
    }
    return 0;
}
