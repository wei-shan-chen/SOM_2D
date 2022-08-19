#ifndef SOM_H
#define SOM_H

#include <glm/glm.hpp>

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
extern double n_learning_rate;
extern double neighbor;

// enum Alphabet : int {
//     A,
//     B,
//     C,
// };

// Alphabet::A;

// class A {
// public:
//     static print();
// };
// class B {
// private:
//     B();
// public:
//     static B* Get();
//     static print();
// };

// B* b1 = B::Get();

// static int count = 0;
// void print() {
//     //static int count = 0;
//     count++;
// }

// void print2() {
//     count = 100;
// }

#endif