#include "primitive.h"

namespace Asuka {

    Bounds3 GeometricPrimitive::WorldBound() const {
        return shape->WorldBound();
    }


}