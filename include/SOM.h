#ifndef SOM_H
#define SOM_H


void SOM_Create();
void SOM_IterateOnce();
void SOM_Destroy();

extern glm::fvec2** lattice;
extern glm::fvec2** dataset;
extern bool is_som_finished;
extern const int map_width;
extern const int map_height;
extern int go;
extern bool tmp;
extern int iter;
#endif