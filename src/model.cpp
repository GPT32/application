#include "model.hpp"

Model& Model::Instance() {
    static Model instance;
    return instance;
}
